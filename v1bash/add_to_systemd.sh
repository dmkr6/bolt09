#!/bin/bash
sudo sh -c '
touch /etc/systemd/system/my_daemon_bash.service
chmod 664 /etc/systemd/system/my_daemon_bash.service

cat >/etc/systemd/system/my_daemon_bash.service <<EOL
[Unit]
Description=my bash daemon
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
systemctl start my_daemon_bash.service
systemctl status my_daemon_bash.service
systemctl enable my_daemon_bash.service
'
