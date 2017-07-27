# HMFS-Daisy
This is an integration for PM filesystem and memory system as a part of 863 project.
A final integrated version should be **File System + Memory System + Volume Mode**.

## How to use

**Download codes:** git clone https://github.com/DDST-NVM/HMFS-Daisy.git 

**Install Dependencies:**
sudo apt-get update  
sudo apt-get install build-essential -y  
sudo apt-get install libncurses-dev
sudo apt-get install initramfs-tools -y

**Install modified linux kernel:**
make menuconfig
make  -jN(N is decided by the core number in your computer)
sudo make modules_install  
sudo make install  

**Install File System:**
sudo vim /boot/grub/grub.cfg  
Add content *memmap=2G\$1G* in this line:
*linux /boot/vmlinux-3.11.0*,
which means 2GB memory from 1GB address is reserved  for mounting filesystem.

**Reboot and choose 3.11.0 kernel**

**Test filesystem:**
cd fs/hmfs
make all 
sudo insmod hmfs.ko
mkdir ~/mnt-hmfs
sudo mount -t hmfs -o physaddr=0x40000000,init=2G,gid=1000,uid=1000 none ~/mnt-hmfs
Run `mount` or `df -lh` to certify that file system is mounted successfully or not.
If successful, you can create a file by `mkdir` and something like it in *~/mnt-hmfs* directory.

For more info about hybrid memory file system, you can refor to [this blog](http://blog.csdn.net/sunwukong54/article/details/50899191).

**Test memory system:**
make
./ptest (you can read the test program to know the control logic)
./ptest p 
./ptest m 10
./ptest b
./ptest gbd
./ptest n
./ptest gn
...
