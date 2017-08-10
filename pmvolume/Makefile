obj-m:=pmvolume.o 
# KERNELDIR:=/lib/modules/3.11.0/build  
PWD:=$(shell pwd) 
KERNELDIR:=/home/kaixin/NVMProject/HMFS-Daisy/linux-3.11/
modules: 
	        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
modules_install:
	        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean: 
	rm -rf *.o *.ko *.mod.c *.markers *.order *.symvers
