ifneq (${KERNELRELEASE},)

	obj-m  = test-irq-01.o
	obj-m += test-irq-02.o
	obj-m += test-irq-03.o
	obj-m += test-irq-04.o
	obj-m += test-irq-05.o
	obj-m += test-irq-06.o

else

	# Uncomment and complete the following lines if you cross-compile the examples.
	# export ARCH          := arm
	# export CROSS_COMPILE := arm-linux-
	# export KERNEL_DIR    := ~/cross/kernel/linux-rpi/

	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	MODULE_DIR := $(shell pwd)

.PHONY: all
all: modules

.PHONY: modules
modules:
	${MAKE} -C ${KERNEL_DIR} SUBDIRS=${MODULE_DIR}  modules

%:%.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS)  -o $@ $<

.PHONY: clean,
clean:
	rm -f *.o *.ko *.mod.c .*.o .*.ko .*.mod.c .*.cmd *~ *.ko.unsigned
	rm -f Module.symvers Module.markers modules.order
	rm -rf .tmp_versions

endif

