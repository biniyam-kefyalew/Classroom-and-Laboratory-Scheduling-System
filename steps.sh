gcc -Wall -Wextra -std=c99 \
    web_interface.c scheduling_system.c \
    -o web_interface

sudo cp web_interface /var/www/cgi-bin/
sudo chmod +x /var/www/cgi-bin/web_interface

sudo systemctl restart httpd

xdg-open "http://localhost/cgi-bin/web_interface?page=home"

http://localhost/cgi-bin/web_interface?page=home



# to display the contnet of rooms.dat
sudo hexdump -C /var/www/data/rooms.dat | head
# to display the contnet of schedules.dat
sudo hexdump -C /var/www/data/schedules.dat | head