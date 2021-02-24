#!/bin/bash
crontab -l | { cat; echo "* * * * * $(pwd)/daemon.sh"; } | crontab -
sudo systemctl restart cron.service
