

obj-m :=ioctl.o
KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o write write.c
	gcc -o read read.c
	gcc -o user user.c	

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm write
	rm read
	rm user
	
