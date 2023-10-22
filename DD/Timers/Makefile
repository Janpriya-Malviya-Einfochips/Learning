

#obj-m := Tasklet_demo.o Timmer_demo.o shared_WorkQ_demo.o work_Q_demo.o
obj-m := work_Q_demo.o
KERNEL_DIR=/usr/src/linux-headers-6.2.0-34-generic/

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules


clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean
