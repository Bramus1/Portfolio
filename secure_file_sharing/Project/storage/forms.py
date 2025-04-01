from django.contrib.auth import get_user_model
from django.contrib.auth.forms import UserCreationForm

from django import forms
from django.core.validators import RegexValidator


class FileUploadForm(forms.Form):
    name = forms.CharField(max_length=100, required=False, validators=[
        RegexValidator(regex='^[a-zA-Z0-9 _-]*$', message="Invalid name.")
    ])
    file = forms.FileField(required=False)
    directory_name = forms.CharField(max_length=100, required=False, validators=[
        RegexValidator(regex='^[a-zA-Z0-9 _-]*$', message="Invalid name.")
    ])


from django import forms
from django.contrib.auth.forms import UserCreationForm
from django_recaptcha.fields import ReCaptchaField
from django_recaptcha.widgets import ReCaptchaV2Checkbox

class CustomUserCreationForm(UserCreationForm):
    captcha = ReCaptchaField(widget=ReCaptchaV2Checkbox)

    class Meta:
        model = get_user_model()  
        fields = ['username','email', 'password1', 'password2', 'captcha']

    def clean(self):
        cleaned_data = super().clean()
        user = self.instance

        if not cleaned_data.get('captcha'):
            self.add_error('captcha', 'Vous devez valider le reCAPTCHA.')
        return cleaned_data


from django.contrib.auth.forms import AuthenticationForm

class CustomAuthenticationForm(AuthenticationForm):
    captcha = ReCaptchaField(widget=ReCaptchaV2Checkbox)

    def clean(self):
        cleaned_data = super().clean()
        captcha = cleaned_data.get('captcha')
        if not captcha:
            self.add_error('captcha', 'Vous devez valider le reCAPTCHA.')
        return cleaned_data


class FileUploadForm(forms.Form):
        name = forms.CharField(max_length=255, required=True, label="File Name")
        file = forms.FileField(required=True, label="Select File")

