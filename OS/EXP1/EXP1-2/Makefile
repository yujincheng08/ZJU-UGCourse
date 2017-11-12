KDIR := /lib/modules/$(shell uname -r)/build
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
TARGET=lstask
PWD=$(shell pwd)
obj-m += $(TARGET).o
default:
	make -C $(KDIR) M=$(PWD) modules
	g++ lstask_user.cpp -o lstask_user
