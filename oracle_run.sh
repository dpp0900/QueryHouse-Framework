#!/bin/bash
cat <<EOF > /tmp/run_oracle.sh
#!/bin/bash
export ORACLE_HOME=/opt/oracle/product/23ai/dbhomeFree
export ORACLE_SID=FREE
export ORACLE_BASE=/opt/oracle
export LD_LIBRARY_PATH=\$ORACLE_HOME/lib:\$LD_LIBRARY_PATH
export PATH=\$ORACLE_HOME/bin:\$PATH

lsnrctl start
sqlplus / as sysdba <<EOF
STARTUP;
exit;
EOF

sudo su - oracle -c "sh /tmp/run_oracle.sh"