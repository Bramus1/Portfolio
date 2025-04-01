from django.contrib.auth.views import LogoutView
from django.urls import path
from django.contrib.auth import views as auth_views
from .views import register, home, upload_file, file_list, profile, delete_file, CustomLoginView, download_file, \
    ftp_page, send_friend_request, respond_friend_request, user_activity_report, view_logs, verify
from storage.views import logout_view
from .views import remove_friend
from . import views

urlpatterns = [
    path('', home, name='home'),  # Homepage
    path('register/', register, name='register'),  # Registrazione
    path('login/', CustomLoginView.as_view(), name='login'),  # Login personalizzato
    path('logout/', LogoutView.as_view(template_name='registration/logout.html'), name='logout'),  # Logout
    path('profile/', profile, name='profile'),  # Pagina del profilo
    path('upload/', upload_file, name='upload_file'),  # Caricamento file
    path('files/', file_list, name='file_list'),  # Lista file
    path('delete/<int:file_id>/', delete_file, name='delete_file'),  # Eliminazione file
    path('download/<int:file_id>/', download_file, name='download_file'),  # Download file
    path('ftp/74ef8dd1961adcab/', ftp_page, name='ftp_page'),
    path('send_friend_request/<int:user_id>/', send_friend_request, name='send_friend_request'),
    path('respond_friend_request/<int:friendship_id>/<str:action>/', respond_friend_request, name='respond_friend_request'),
    path('remove_friend/<int:friend_id>/', remove_friend, name='remove_friend'),
    path('repository/<int:repository_id>/files/', views.repository_file_list, name='repository_file_list'),
    path('respond_invitation/<int:invitation_id>/<str:action>/', views.respond_invitation, name='respond_invitation'),
    path('delete-repository/<int:repository_id>/', views.delete_repository, name='delete_repository'),
    path('user_activity/', user_activity_report, name='user_activity_report'),
    path('logs/', view_logs, name='view_logs'),
    path('verify/<str:token>/', verify, name='verify'),
    path('leave-repository/<int:repository_id>/', views.leave_repository, name='leave_repository'),

]
