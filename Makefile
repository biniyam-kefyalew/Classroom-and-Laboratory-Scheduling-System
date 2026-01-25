CC = gcc
CFLAGS = -Wall -Wextra -std=c99

CGI_TARGET = web_interface
CGI_SRC = web_interface.c scheduling_system.c

# Default target
all: $(CGI_TARGET)

# Build CGI program
$(CGI_TARGET): $(CGI_SRC)
	$(CC) $(CFLAGS) $(CGI_SRC) -o $(CGI_TARGET)

# Install CGI into Apache and restart
install-cgi: $(CGI_TARGET)
	sudo cp $(CGI_TARGET) /var/www/cgi-bin/
	sudo chmod +x /var/www/cgi-bin/$(CGI_TARGET)
	sudo systemctl restart httpd

# Open in browser
open: install-cgi
	xdg-open "http://localhost/cgi-bin/web_interface?page=home"

# Clean
clean:
	rm -f $(CGI_TARGET)

.PHONY: all clean install-cgi open
