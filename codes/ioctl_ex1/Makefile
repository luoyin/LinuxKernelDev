MODULE_NAME := luo_ioctl_ex1

${MODULE_NAME}-objs := server.o
obj-m := $(MODULE_NAME).o
KDIR := /lib/modules/3.12.53-40-desktop/build/
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers
disclean:
	rm -rf *.o *~ core .depend .*.cmd *.mod.c .tmp_versions Module.symvers

