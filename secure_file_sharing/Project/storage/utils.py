import os

from django.conf import settings


def load_key():
    key_path = os.path.join("file_key.key") 
    if os.path.exists(key_path):
        with open(key_path, "rb") as key_file:
            key = key_file.read()
            if len(key) != 44:  
                raise ValueError("Fernet key must be 32 url-safe base64-encoded bytes. Key is invalid.")
            return key
    raise FileNotFoundError("Fernet key file not found. Generate it using `generate_key_file()`.")



def load_key_name_file():
    return open(settings.BASE_DIR / "file_name_key.key", "rb").read()