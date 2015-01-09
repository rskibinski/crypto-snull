sysctl -w net.ipv4.ip_forward=1
insmod crypto_snull.ko local_address="10.0.0.10" remote_address="10.0.0.11" tun_device="eth1"
ifconfig cryp0 10.0.0.10
