# Makefile  Django

# Variables
PYTHON = python3
DJANGO_MANAGE = Project/manage.py
CERTIFICATE = Project/CA/public/sec_pb_cert.pem
PRIVATE_KEY = Project/CA/private/sec_p_key.pem

.PHONY: make
make:
	@echo "Make command is available. Use 'make run' to start the server."

.PHONY: run
run:
	@echo "Starting the Django server with SSL support..."
	$(PYTHON) $(DJANGO_MANAGE) runsslserver --certificate $(CERTIFICATE) --key $(PRIVATE_KEY)

.PHONY: install-linux
install-linux:
	@echo "Installing requirements..."
	pip install -r requirements.txt --break-system-packages

.PHONY: install-windows
install-windows:
	@echo "Installing requirements..."
	pip install -r requirements.txt

