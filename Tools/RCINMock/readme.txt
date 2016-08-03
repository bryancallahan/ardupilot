Install module at boot with:

sudo ln -s /path/to/RCINMock /lib/modules/`uname -r`/RCINMock
sudo depmod -a
sudo modprobe rcin_mock

sudo vi /etc/modules
rcin_mock
