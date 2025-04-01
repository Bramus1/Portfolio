import uuid

from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from django.db import models
from django.contrib.auth.models import AbstractUser
from cryptography.fernet import Fernet
from django.conf import settings
from storage.utils import load_key_name_file, load_key
from django_otp.models import Device
from django.utils import timezone
from datetime import timedelta


class Directory(models.Model):
    name = models.CharField(max_length=255)
    owner = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)
    parent = models.ForeignKey('self', on_delete=models.CASCADE, null=True, blank=True, related_name="subdirectories")
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.name


class File(models.Model):
    original_name = models.CharField(max_length=255, blank=True, null=True)  
    name = models.CharField(max_length=255) 
    encrypted_content = models.BinaryField()
    owner = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)
    directory = models.ForeignKey(
        'Directory', on_delete=models.CASCADE, null=True, blank=True, related_name="files"
    )
    uploaded_at = models.DateTimeField(auto_now_add=True)
    repository = models.ForeignKey(
        'Repository', on_delete=models.CASCADE, null=True, blank=True, related_name="files"
    )



    def save(self, *args, **kwargs):
        if not self.original_name:  
            self.original_name = self.name
        key = load_key_name_file()  
        fernet = Fernet(key)
        self.name = fernet.encrypt(self.name.encode()).decode()  
        super().save(*args, **kwargs)

    def get_decrypted_name(self):
        
        key = load_key_name_file()
        return Fernet(key).decrypt(self.name.encode()).decode()

    def __str__(self):
        return self.original_name or self.name


class Friendship(models.Model):
    sender = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE, related_name='friend_requests_sent')
    receiver = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE, related_name='friend_requests_received')
    accepted = models.BooleanField(default=False)
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.sender.username} -> {self.receiver.username} ({'Accepted' if self.accepted else 'Pending'})"


class Message(models.Model):
    sender = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE, related_name='messages_sent')
    receiver = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE, related_name='messages_received')
    encrypted_content = models.TextField()
    sent_at = models.DateTimeField(auto_now_add=True)

    def save(self, *args, **kwargs):
        
        key = load_key()  # Load encryption key from a secure file
        fernet = Fernet(key)
        self.encrypted_content = fernet.encrypt(self.encrypted_content.encode()).decode()
        super().save(*args, **kwargs)

    def get_decrypted_content(self):
        # Decrypt the message content for display
        key = load_key()
        fernet = Fernet(key)
        return fernet.decrypt(self.encrypted_content.encode()).decode()


class Repository(models.Model):
    name = models.CharField(max_length=255)
    owner = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)
    friends = models.ManyToManyField(settings.AUTH_USER_MODEL, blank=True, related_name='repositories_shared')
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.name


class RepositoryInvitation(models.Model):
    inviter = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE, related_name="invitations_sent")
    invitee = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE, related_name="invitations_received")
    repository = models.ForeignKey(Repository, on_delete=models.CASCADE, related_name="invitations")
    status = models.CharField(
        max_length=20,
        choices=[('pending', 'Pending'), ('accepted', 'Accepted'), ('declined', 'Declined')],
        default='pending'
    )
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"Invitation from {self.inviter.username} to {self.invitee.username} for {self.repository.name}"


class OTPDevice(Device):
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)


class CustomUser(AbstractUser):
    is_verified = models.BooleanField(default=False)  # Add custom attributes here
    verification_token = models.CharField(max_length=255, null=True, blank=True)
    token_expiration = models.DateTimeField(null=True, blank=True)


    def set_verification_token(self):
        self.verification_token = uuid.uuid4().hex
        self.token_expiration = timezone.now() + timedelta(hours=24)


class UserActivity(models.Model):
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)
    action = models.CharField(max_length=255)
    additional_info = models.TextField(blank=True)
    timestamp = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.user.username} performed {self.action} on {self.timestamp}"
