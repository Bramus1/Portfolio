from django.contrib import admin

# Register your models here.
from django.contrib import admin
from .models import File, Directory, UserActivity

@admin.register(File)
class FileAdmin(admin.ModelAdmin):
    list_display = ('name', 'owner', 'uploaded_at')
    search_fields = ('name', 'owner__username')

@admin.register(Directory)
class DirectoryAdmin(admin.ModelAdmin):
    list_display = ('name', 'owner')
    search_fields = ('name', 'owner__username')

@admin.register(UserActivity)
class UserActivityAdmin(admin.ModelAdmin):
    list_display = ('user', 'action', 'additional_info', 'timestamp')
    search_fields = ('user__username', 'action')
    list_filter = ('action',)  #  FILE_UPLOAD, FILE_DELETE
    list_per_page = 20

