: ==== start ====
TESTNAME=interop-ikev2-racoon-01-noconn

mkdir /tmp/racoon2 /var/run/racoon2
chmod 700 /var/run/racoon2
cp -r /testing/pluto/$TESTNAME/east-racoon/* /tmp/racoon2/
chmod 700 /tmp/racoon2/psk/test.psk /tmp/racoon2/spmd.pwd

#racoon way of starting
/usr/local/racoon2/etc/init.d/spmd start
# no conns are loaded
/usr/local/racoon2/etc/init.d/iked start
sleep 3

echo done
