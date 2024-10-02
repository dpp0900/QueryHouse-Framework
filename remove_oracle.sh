#!/bin/bash
sudo /etc/init.d/oracle-free-23ai delete
sudo apt-get remove --purge oracle-database-free-23ai -y
sudo rm -rf /opt/oracle
sudo rm -rf /u01/app/oracle
sudo userdel -r oracle
sudo groupdel dba
sudo apt-get autoremove
sudo apt-get autoclean
