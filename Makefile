obj-m += simple_driver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod simple_driver.ko

uninstall:
	sudo rmmod simple_driver

test:
	sudo dmesg | tail -20

load: all install test

unload: uninstall
	make clean