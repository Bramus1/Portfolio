from pathlib import Path
import os


from django.conf import settings


SECRET_KEY = os.getenv("DJANGO_SECRET_KEY", "your-fallback-key")
#DEBUG = os.getenv("DJANGO_DEBUG", "False").lower() == "true"
DEBUG = True

EMAIL_BACKEND = 'django.core.mail.backends.console.EmailBackend'
EMAIL_HOST = 'smtp.gmail.com'
EMAIL_PORT = 587
EMAIL_USE_TLS = True
EMAIL_HOST_USER = 'djangohe2be@gmail.com'
EMAIL_HOST_PASSWORD = 'he2be_esi'
DEFAULT_FROM_EMAIL = 'djangohe2be@gmail.com'

# Build paths inside the project like this: BASE_DIR / 'subdir'.
BASE_DIR = Path(__file__).resolve().parent.parent

AUTH_USER_MODEL = 'storage.CustomUser'

ALLOWED_HOSTS = ['127.0.0.1', '172.29.48.1', '192.168.0.132', 'mysecureproject.duckdns.org','*']

SITE_ID = 1
MEDIA_URL = '/media/'
MEDIA_ROOT = BASE_DIR / 'media'
LOGIN_URL = '/login/'
LOGIN_REDIRECT_URL = '/profile/'
LOGOUT_REDIRECT_URL = '/'
SESSION_EXPIRE_AT_BROWSER_CLOSE = True


SESSION_COOKIE_AGE = 1000

SESSION_ENGINE = 'django.contrib.sessions.backends.signed_cookies'



# Application definition
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    #'storage.apps.StorageConfig',
    'django.contrib.sites',
    'django_otp',
    'django_otp.plugins.otp_totp',
    'storage',
    'django_recaptcha',
    'sslserver',


]


MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
    'django_otp.middleware.OTPMiddleware',

]

ROOT_URLCONF = 'Secure_storage.urls'

TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [BASE_DIR / 'templates'],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]

WSGI_APPLICATION = 'Secure_storage.wsgi.application'

# Database
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.sqlite3',
        'NAME': BASE_DIR / 'db.sqlite3',}
}

# Password validation
AUTH_PASSWORD_VALIDATORS = [
    {
        'NAME': 'django.contrib.auth.password_validation.UserAttributeSimilarityValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.MinimumLengthValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.CommonPasswordValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.NumericPasswordValidator',
    },
]

# Internationalization
LANGUAGE_CODE = 'en-us'
TIME_ZONE = 'UTC'
USE_I18N = True
USE_TZ = True

# Static files (CSS, JavaScript, Images)
STATIC_URL = 'static/'

# Default primary key field type
DEFAULT_AUTO_FIELD = 'django.db.models.BigAutoField'


import json

try:
    secret_file_path = BASE_DIR / "secret.json"
    with open(secret_file_path, "r") as f:
        secrets = json.load(f)
except FileNotFoundError:
    raise FileNotFoundError(f"Le fichier {secret_file_path} est introuvable.")
except json.JSONDecodeError:
    raise ValueError(f"Le fichier {secret_file_path} contient un JSON invalide.")

RECAPTCHA_PUBLIC_KEY = "6LcuvaUqAAAAAPsefl3R-ZhLst_xuupAL26BiDEX"
RECAPTCHA_PRIVATE_KEY = secrets.get("RECAPTCHA_PRIVATE_KEY", "")

RECAPTCHA_USE_SSL = True


if not RECAPTCHA_PUBLIC_KEY or not RECAPTCHA_PRIVATE_KEY:
    raise ValueError("Les clés reCAPTCHA ne sont pas correctement définies.")

RECAPTCHA_ERROR_MSG = {
    'required': "Veuillez vérifier que vous n'êtes pas un robot.",
}

SECURE_SSL_REDIRECT = True  
SECURE_HSTS_SECONDS = 3600  
SECURE_HSTS_INCLUDE_SUBDOMAINS = True
SECURE_HSTS_PRELOAD = True

CSRF_COOKIE_SECURE = True
SESSION_COOKIE_SECURE = True
# Logging Configuration


LOGGING = {
    'version': 1,
    'disable_existing_loggers': False,
    'formatters': {
        'verbose': {
            'format': '{levelname} {asctime} {module} {message}',
            'style': '{',
        },
        'simple': {
            'format': '{levelname} {message}',
            'style': '{',
        },
    },
    'handlers': {
        'file': {
            'level': 'INFO',  
            'class': 'logging.FileHandler',
            'filename': os.path.join(BASE_DIR, 'operations.log'),
            'formatter': 'verbose',
        },
    },
    'loggers': {
        'django': {
            'handlers': ['file'],
            'level': 'INFO',  
            'propagate': True,
        },
        'storage': {
            'handlers': ['file'],
            'level': 'DEBUG',  
            'propagate': False,
        },
    },
}




