#!/bin/bash
sudo sh -c '
touch /etc/systemd/system/my_daemon.service
chmod 664 /etc/systemd/system/my_daemon.service

cat >/etc/systemd/system/my_daemon.service <<EOL
[Unit]
Description=arp eth0 info
After=network.target
[Service]
Type=forking
User=root
ExecStart=$(pwd)/daemon
Restart=always
[Install]
WantedBy=multi-user.target
EOL

systemctl daemon-reload
systemctl start my_daemon.service
systemctl status my_daemon.service
systemctl enable my_daemon.service
'
