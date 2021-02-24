#!/usr/bin/env bash

date >> /home/dima/work/ask42/v2bash/daemon.log
/usr/sbin/arp -i eth0 >> /home/dima/work/ask42/v2bash/daemon.log
