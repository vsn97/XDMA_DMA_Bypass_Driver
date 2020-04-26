
MODULE = driver

obj-m += $(MODULE).o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

all: $(MODULE)

%.o: %.c
	@echo "  CC      $<"
	@$(CC) -c $< -o $@

$(MODULE):
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

