#!/bin/bash
# Install APT packages
sudo apt-get install alien libaio1 wget -y

# Download Oracle Database RPM
RPM_NAME="oracle-database-free-23ai-1.0-1.el9.x86_64.rpm"
DEB_NAME="oracle-database-free-23ai_1.0-2_amd64.deb"
DOWNLOAD_URL="https://download.oracle.com/otn-pub/otn_software/db-free/${RPM_NAME}"

sudo useradd --no-create-home oracle
sudo groupadd oinstall

# Setting shared memory

sudo touch /etc/rc2.d/S01shm_load
sudo chmod 755 /etc/rc2.d/S01shm_load
sudo echo "echo \#!/bin/sh" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "case \"\$1\" in" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "start) mkdir /var/lock/subsys 2>/dev/null" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "touch /var/lock/subsys/listener" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "rm /dev/shm 2>/dev/null" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "mkdir /dev/shm 2>/dev/null" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "mount -t tmpfs shmfs -o size=4096m /dev/shm" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo ";;" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "*) echo error" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "exit 1" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo ";;" | sudo tee -a /etc/rc2.d/S01shm_load
sudo echo "esac" | sudo tee -a /etc/rc2.d/S01shm_load



# Kernel Parameter Settings for Oracle ./Database Free
SYSCTL_CONF="/etc/sysctl.d/97-oracle-database-sysctl.conf"
if [ ! -f "${SYSCTL_CONF}" ]; then
  sudo touch "${SYSCTL_CONF}"
  echo "# Kernel Parameter Settings for Oracle Database Free" | sudo tee -a "${SYSCTL_CONF}"
  echo "kernel.sem = 250 32000 100 128" | sudo tee -a "${SYSCTL_CONF}"
  echo "kernel.shmmax = 4398046511104" | sudo tee -a "${SYSCTL_CONF}"
  echo "kernel.shmmni = 4096" | sudo tee -a "${SYSCTL_CONF}"
  echo "kernel.shmall = 1073741824" | sudo tee -a "${SYSCTL_CONF}"
  echo "fs.file-max = 6815744" | sudo tee -a "${SYSCTL_CONF}"
  echo "kernel.aio-max-nr = 1048576" | sudo tee -a "${SYSCTL_CONF}"
  echo "net.ipv4.ip_local_port_range = 9000 65500" | sudo tee -a "${SYSCTL_CONF}"
  echo "kernel.panic_on_oops = 1" | sudo tee -a "${SYSCTL_CONF}"
  echo "net.core.rmem_default = 262144" | sudo tee -a "${SYSCTL_CONF}"
  echo "net.core.rmem_max = 4194304" | sudo tee -a "${SYSCTL_CONF}"
  echo "net.core.wmem_default = 262144" | sudo tee -a "${SYSCTL_CONF}"
  echo "net.core.wmem_max = 1048576" | sudo tee -a "${SYSCTL_CONF}"
fi

# Apply the changes
sudo sysctl -p

echo "[INFO] Downloading Oracle Database RPM"
if [ ! -f "${RPM_NAME}" ]; then
  wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" "${DOWNLOAD_URL}"
fi

echo "[INFO] Converting Oracle Database RPM to DEB"
if [ ! -f "${DEB_NAME}" ]; then
  sudo alien --scripts -d "${RPM_NAME}"
fi

# Install Oracle Database
echo "[INFO] Installing Oracle Database"
sudo mkdir -p /opt/oracle
sudo chown oracle:oinstall /opt/oracle
sudo dpkg -i "${DEB_NAME}"

# Configure Kernel Parameters
echo "[INFO] Configuring Kernel Parameters"

# Configure Oracle Database
echo "[INFO] Configuring Oracle Database"
export ORACLE_SID=FREE 
export ORAENV_ASK=NO 
echo -e "\n" | sudo /opt/oracle/product/23ai/dbhomeFree/bin/oraenv

(echo "password"; echo "password";) | sudo /etc/init.d/oracle-free-23ai configure
(echo "password"; echo "password";) | sudo /etc/init.d/oracle-free-23ai configure

OCI_URL="https://download.oracle.com/otn_software/linux/instantclient/2350000/instantclient-basic-linux.x64-23.5.0.24.07.zip"
ORACLE_SDK_URL="https://download.oracle.com/otn_software/linux/instantclient/2350000/instantclient-sdk-linux.x64-23.5.0.24.07.zip"

wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" "${OCI_URL}" -O /tmp/OCI.zip
wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" "${ORACLE_SDK_URL}" -O /tmp/SDK.zip

sudo unzip /tmp/OCI.zip -d /opt/oracle/product/23ai/dbhomeFree/lib/OCI
sudo unzip /tmp/SDK.zip -d /opt/oracle/product/23ai/dbhomeFree/lib/SDK