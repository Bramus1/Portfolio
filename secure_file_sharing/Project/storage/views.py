
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives.serialization import load_pem_private_key, load_pem_public_key
from pyexpat.errors import messages
from django.contrib.auth.decorators import login_required, user_passes_test
from . import models
from .models import File, Directory, Message, Repository, RepositoryInvitation
from .forms import FileUploadForm, CustomAuthenticationForm
from cryptography.fernet import Fernet
from django.conf import settings
from django.http import HttpResponse
import os
from storage.models import CustomUser
from django.http import JsonResponse
from typing import cast
from django.contrib.auth.views import LoginView
from django.contrib.auth import logout
from django.http import Http404, HttpResponseForbidden
from django.db.models import Q

from django.core.mail import send_mail
from django.contrib.auth.decorators import login_required
from django.shortcuts import render, redirect, get_object_or_404

from .forms import CustomUserCreationForm
from storage.models import Friendship
from django.shortcuts import redirect, get_object_or_404
from .models import Repository
from .models import UserActivity
from django.contrib import messages
from django.utils import timezone
import logging
from django.contrib.auth import get_user_model
logger = logging.getLogger(__name__)

User = get_user_model()

def sanitize_input(input_string):
    forbidden_patterns = ["--", ";", "/*", "*/", " OR ", " AND ", " UNION "]
    for pattern in forbidden_patterns:
        if pattern.lower() in input_string.lower():
            raise ValueError("Input non valido.")
    return input_string


class CustomLoginView(LoginView):
    form_class = CustomAuthenticationForm

    def form_valid(self, form):
        user = form.get_user()
        remember_me = self.request.POST.get('remember_me')

        if remember_me:
            self.request.session.set_expiry(2592000)  
        else:
            self.request.session.set_expiry(0)  

        if not user.is_verified:
            logger.warning(f"User {user.username} attempted to log in without verification.")
            return HttpResponse("You must verify your account to log in.", status=403)
        logger.info(f"User {user.username} successfully logged in.")
        return super().form_valid(form)

def register(request):
    if request.method == 'POST':
        form = CustomUserCreationForm(request.POST)
        if form.is_valid():
            user = form.save(commit=False)
            user.is_verified = False
            user.set_verification_token()
            user.save()
            logger.info(f"New user registered: {user.username}, email: {user.email}")
            log_user_activity(user, 'REGISTER')

            verification_link = f"https://localhost:8000/verify/{user.verification_token}/"
            try:
                send_mail(
                    'Confirm your registration',
                    f'Click the link to verify your account: {verification_link}',
                    settings.DEFAULT_FROM_EMAIL,
                    [user.email],
                    fail_silently=False,
                )
                logger.info(f"Verification email sent to {user.email}")
            except Exception as e:
                logger.error(f"Failed to send verification email to {user.email}: {e}")

            return HttpResponse("Check your email to verify your account.")
    else:
        form = CustomUserCreationForm()
    return render(request, 'registration/register.html', {'form': form})


def home(request):
    
    if request.user.is_authenticated:
        return redirect('profile')

    
    return render(request, 'storage/home.html')


def verify(request, token):
    from django.contrib.auth import get_user_model
    User = get_user_model()
    try:
        user = User.objects.get(verification_token=token)
        if user.is_verified:
            messages.info(request, "Your account is already verified.")
            return redirect('login')
            

        if user.token_expiration < timezone.now():
            messages.error(request, "Verification token has expired.")
            return redirect('register')
            

        user.is_verified = True
        user.verification_token = None
        user.token_expiration = None
        user.save()
        messages.success(request, "Account successfully verified!")
        return redirect('login')
    except User.DoesNotExist:
        messages.error(request, "Invalid or non-existent token.")
        return redirect('register')


@login_required
def upload_file(request):
    if request.method == 'POST':
        form = FileUploadForm(request.POST, request.FILES)
        if form.is_valid():
            uploaded_file = request.FILES['file']
            encrypted_content = encrypt_file(uploaded_file.read())


            
            repository_id = request.POST.get('repository')
            repository = Repository.objects.get(id=repository_id) if repository_id else None
            file = File(
                name=form.cleaned_data['name'],
                encrypted_content=encrypted_content,
                owner=request.user,
                repository=repository,  

            )
            file.save()

            return redirect('file_list')
    else:
        form = FileUploadForm()

    
    user_repositories = Repository.objects.filter(Q(owner=request.user) | Q(friends=request.user))
    return render(request, 'storage/upload_file.html', {'form': form, 'user_repositories': user_repositories})


@login_required
def file_list(request):
    files = File.objects.filter(owner=request.user)
    directories = Directory.objects.filter(owner=request.user)
    return render(request, 'storage/file_list.html', {'files': files, 'directories': directories})


# ici aussi
@login_required
def logout_view(request):
    logout(request)
    return redirect('home')  


def load_key():
    key_path = os.path.join(settings.BASE_DIR, "file_key.key")
    if os.path.exists(key_path):
        with open(key_path, "rb") as key_file:
            return key_file.read()
    raise ValueError("Encryption key not found. Generate it using `generate_key_file`.")


def encrypt_file(file_content):
    key = load_key()
    fernet = Fernet(key)
    return fernet.encrypt(file_content)


def decrypt_file(encrypted_content):
    key = load_key()
    fernet = Fernet(key)
    return fernet.decrypt(encrypted_content)


@login_required
def download_file(request, file_id):
    try:
        
        file = File.objects.get(id=file_id)
        repository = file.repository


        
        if repository:
            
            if request.user != repository.owner and request.user not in repository.friends.all():
                raise Http404("You are not authorized to download this file.")

        
        decrypted_content = decrypt_file(file.encrypted_content)



        

        response = HttpResponse(decrypted_content, content_type='application/octet-stream')
        response['Content-Disposition'] = f'attachment; filename="{file.get_decrypted_name()}"'

        logger.info(f"User {request.user.username} downloaded file: {file.name}")
        log_user_activity(request.user, 'FILE_DOWNLOAD', additional_info=file.name)
        return response

    except File.DoesNotExist:
        raise Http404("File not found.")
    except Exception as e:
        logger.error(f"Error while downloading file {file_id}: {str(e)}")
        raise Http404("An error occurred while processing the file download.")




@login_required
def profile(request):
    user_files = File.objects.filter(owner=request.user)

    
    friends = []
    friendships = Friendship.objects.filter(
        Q(sender=request.user) | Q(receiver=request.user), accepted=True
    )
    for friendship in friendships:
        if friendship.sender == request.user:
            friends.append(friendship.receiver)
        else:
            friends.append(friendship.sender)

    
    friend_requests = Friendship.objects.filter(receiver=request.user, accepted=False)

    
    user_repositories = Repository.objects.filter(
        Q(owner=request.user) |
        Q(invitations__invitee=request.user, invitations__status='accepted') 
    ).distinct()  

    
    user_repositories = user_repositories.filter(
        Q(friends=request.user) | Q(owner=request.user)  
    )

   
    if request.method == 'POST' and 'create_repo' in request.POST:
        repo_name = request.POST.get('repo_name')
        friends_ids = request.POST.getlist('friends')  

        
        repository = Repository.objects.create(name=repo_name, owner=request.user)

        
        friends_to_add = User.objects.filter(id__in=friends_ids)
        repository.friends.set(friends_to_add)

       
        for friend in friends_to_add:
            RepositoryInvitation.objects.create(
                inviter=request.user,
                invitee=friend,
                repository=repository,
                status='pending'  
            )

        repository.save()
        messages.success(request, f"Repository '{repo_name}' created and invitations sent !")
        return redirect('profile')

    
    invitations = RepositoryInvitation.objects.filter(invitee=request.user, status='pending')

    
    if request.method == 'POST' and 'upload' in request.POST:
        form = FileUploadForm(request.POST, request.FILES)
        if form.is_valid():
            uploaded_file = request.FILES.get('file')
            if uploaded_file:
                encrypted_content = encrypt_file(uploaded_file.read())
                new_file = File(
                    name=form.cleaned_data['name'],
                    encrypted_content=encrypted_content,
                    owner=request.user,
                    directory=form.cleaned_data.get('directory')
                )
                new_file.save()
                return redirect('profile')
    else:
        form = FileUploadForm()

    
    return render(request, 'storage/profile.html', {
        'files': user_files,
        'form': form,
        'friends': friends,  
        'friend_requests': friend_requests,
        'invitations': invitations,  
        'user_repositories': user_repositories  
    })


@login_required
def delete_file(request, file_id):
    
    file = get_object_or_404(File, id=file_id, owner=request.user)
    if file.owner != request.user:
        messages.error(request, "You are not authorized to delete this file.")
        return redirect('profile')

    
    if file.encrypted_content:
       
        file.encrypted_content = os.urandom(len(file.encrypted_content))  
        file.save()  

    
    file.delete()

    logger.info(f"User {request.user.username} deleted file: {file.name}")
    log_user_activity(request.user, 'FILE_DELETE', additional_info=file.name)
    messages.success(request, "File deleted successfully.")
    return redirect('profile')

@login_required
def repository_file_list(request, repository_id):
    
    repository = get_object_or_404(Repository, id=repository_id)

    
    if request.user != repository.owner and request.user not in repository.friends.all():
        raise Http404("You are not authorized to view this repository.")

    
    files = File.objects.filter(repository=repository)

    
    if request.method == 'POST':
        form = FileUploadForm(request.POST, request.FILES)
        if form.is_valid():
            uploaded_file = request.FILES.get('file')
            encrypted_content = encrypt_file(uploaded_file.read())

            
            file = File(
                name=form.cleaned_data['name'],
                encrypted_content=encrypted_content,
                owner=request.user,
                repository=repository
            )
            file.save()
            messages.success(request, f"File '{file.name}' uploaded successfully!")
            return redirect('repository_file_list', repository_id=repository.id)
        else:
            messages.error(request, "Upload failed. Please check the form inputs.")

    
    return render(request, 'storage/repository_file_list.html', {
        'repository': repository,
        'files': files,
        'form': FileUploadForm(),
    })


def ftp_page(request):
    if not request.user.is_authenticated or not request.user.is_staff:
        raise Http404("Page not found")
    return render(request, 'storage/ftp_page.html')


@login_required
def friends_list(request):
    friends = Friendship.objects.filter(
        (models.Q(sender=request.user) | models.Q(receiver=request.user)) &
        models.Q(accepted=True)
    )
    return render(request, 'storage/friends_list.html', {'friends': friends})

@login_required
def send_message(request, user_id):
    receiver = get_object_or_404(User, id=user_id)
    if request.method == "POST":
        content = request.POST.get('content')
        if content:
            Message.objects.create(sender=request.user, receiver=receiver, encrypted_content=content)
    return redirect('messages_list', user_id=receiver.id)

@login_required
def messages_list(request, user_id):
    receiver = get_object_or_404(User, id=user_id)
    messages = Message.objects.filter(
        (models.Q(sender=request.user, receiver=receiver) |
         models.Q(sender=receiver, receiver=request.user))
    ).order_by('sent_at')
    return render(request, 'storage/messages_list.html', {'messages': messages, 'receiver': receiver})

@login_required
def send_friend_request(request, user_id):
    if request.method == "POST":
        friend_username = request.POST.get("friend_username")

        if not friend_username:
            messages.error(request, "You must provide a friend's username.")
            return redirect('profile')

        try:
            
            receiver = User.objects.get(username=friend_username)
        except User.DoesNotExist:
            messages.error(request, "The user does not exist.")
            return redirect('profile')

        
        if receiver == request.user:
            messages.error(request, "You cannot send a friend request to yourself.")
            return redirect('profile')

        
        existing_friendship = Friendship.objects.filter(
            (Q(sender=request.user, receiver=receiver) | Q(sender=receiver, receiver=request.user)),
            accepted=True
        ).first()

        if existing_friendship:
            messages.error(request, f"{friend_username} is already your friend.")
            return redirect('profile')

        
        existing_invitation = Friendship.objects.filter(
            (Q(sender=request.user, receiver=receiver) | Q(sender=receiver, receiver=request.user)),
            accepted=False
        ).first()

        if existing_invitation:
            messages.error(request, f"An invitation has already been sent to {friend_username}.")
            return redirect('profile')

        
        Friendship.objects.get_or_create(sender=request.user, receiver=receiver, accepted=False)

        messages.success(request, f"Friend request sent to {receiver.username}.")
        return redirect('profile')  


@login_required
def respond_friend_request(request, friendship_id, action):
    
    friendship = get_object_or_404(Friendship, id=friendship_id, receiver=request.user)

    if action == "accept":
        friendship.accepted = True
        friendship.save()
        messages.success(request, f"Vous avez accepté la demande d'amitié de {friendship.sender.username}.")
    elif action == "decline":
        friendship.delete()
        messages.info(request, f"Vous avez refusé la demande d'amitié de {friendship.sender.username}.")

    
    return redirect('profile')

@login_required
def remove_friend(request, friend_id):
    
    friend = get_object_or_404(User, id=friend_id)

    
    friendship = Friendship.objects.filter(
        (Q(sender=request.user, receiver=friend) | Q(sender=friend, receiver=request.user)),
        accepted=True
    ).first()

    if friendship:
       
        friendship.delete()
        messages.success(request, f"You are no longer friends with {friend.username}.")
    else:
        messages.error(request, "No friendship found to remove.")

    return redirect('profile')

@login_required
def create_repository(request):
    if request.method == 'POST':
        
        repo_name = request.POST.get('repo_name')
        friends_ids = request.POST.getlist('friends')  

        
        repository = Repository.objects.create(name=repo_name, owner=request.user)

        
        friends_to_add = User.objects.filter(id__in=friends_ids)
        repository.friends.set(friends_to_add)
        repository.save()

       
        for friend in friends_to_add:
            
            RepositoryInvitation.objects.create(
                inviter=request.user,
                invitee=friend,
                repository=repository,
                status='pending'  
            )

        messages.success(request, f"Repository '{repo_name}' créé avec succès et les invitations envoyées !")
        return redirect('profile')

    
    friends = User.objects.filter(
        Q(friend_requests_sent__sender=request.user, friend_requests_sent__accepted=True) |
        Q(friend_requests_received__receiver=request.user, friend_requests_received__accepted=True)
    )

    return render(request, 'storage/profile.html', {'friends': friends})


@login_required
def respond_invitation(request, invitation_id, action):
    invitation = get_object_or_404(RepositoryInvitation, id=invitation_id, invitee=request.user)

    if action == "accept":
        invitation.status = 'accepted'
        invitation.save()
        
        repository = invitation.repository
        repository.friends.add(request.user)
        messages.success(request, f"You have accepted the invitation to {repository.name}!")

    elif action == "decline":
        invitation.status = 'declined'
        invitation.save()
        messages.info(request, f"You have declined the invitation to {invitation.repository.name}.")

    return redirect('profile')

@login_required
def repository_file_list(request, repository_id):
    
    repository = get_object_or_404(Repository, id=repository_id)

    
    if request.user != repository.owner and request.user not in repository.friends.all():
        raise Http404("You are not authorized to view this repository.")

    
    files = File.objects.filter(repository=repository)

    
    if request.method == 'POST' and 'upload' in request.POST:
        form = FileUploadForm(request.POST, request.FILES)
        if form.is_valid():
            uploaded_file = request.FILES.get('file')
            if uploaded_file:
                encrypted_content = encrypt_file(uploaded_file.read())
                new_file = File(
                    name=form.cleaned_data['name'],  
                    encrypted_content=encrypted_content,
                    owner=request.user,
                    repository=repository  
                )
                new_file.save()
                messages.success(request, f"File '{new_file.name}' uploaded successfully!")
                return redirect('repository_file_list', repository_id=repository.id)
        else:
            messages.error(request, "There was an error with your upload. Please try again.")
    else:
        form = FileUploadForm()

    
    return render(request, 'storage/repository_file_list.html', {
        'repository': repository,
        'files': files,
        'form': form,  
        'user': request.user  
    })



@login_required
def delete_repository(request, repository_id):
    repository = get_object_or_404(Repository, id=repository_id)

    
    if request.user == repository.owner:
        repository.delete()
        # logger.info(f"User {request.user.username} deleted file: {repository.name}")
        # log_user_activity(request.user, 'REPOSITORY_DELETE', additional_info=repository.name)
        messages.success(request, f"Repository '{repository.name}' has been deleted.")
    else:
        messages.error(request, "You are not authorized to delete this repository.")

    return redirect('profile')


def is_admin(user):
    return user.is_superuser


@login_required
@user_passes_test(is_admin)
def view_logs(request):
    log_file_path = os.path.join(settings.BASE_DIR, 'operations.log')
    if not os.path.exists(log_file_path):
        return JsonResponse({"error": "Log file not found."}, status=404)

    with open(log_file_path, 'r') as log_file:
        logs = log_file.readlines()

    return JsonResponse({"logs": logs}, safe=False)


def log_user_activity(user, action, additional_info=""):
    UserActivity.objects.create(user=user, action=action, additional_info=additional_info)

@login_required
def user_activity_report(request):
    activities = UserActivity.objects.all().order_by('-timestamp')[:50]
    return render(request, 'storage/user_activity_report.html', {'activities': activities})

@login_required
def leave_repository(request, repository_id):
    repository = get_object_or_404(Repository, id=repository_id)

    
    if request.user in repository.friends.all() or request.user == repository.owner:
        
        repository.friends.remove(request.user)
        messages.success(request, f"You have successfully left the repository '{repository.name}'.")
    else:
        messages.error(request, "You are not a member of this repository.")

    return redirect('profile')






