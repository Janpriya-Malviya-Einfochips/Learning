

obj-m := list_demo.o
KERNEL_DIR=/usr/src/linux-headers-6.2.0-34-generic/

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules


clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean
