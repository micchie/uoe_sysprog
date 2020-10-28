.PHONY: module app clean hello.c hello.o
obj-m += hello.o
PROG = module app
EXTRA_CFLAGS = -g

all: $(PROG)
module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules \
		EXTRA_CFLAGS=$(EXTRA_CFLAGS)
clean:
	(rm -rf *.o *.ko modules.order hello.mod.c Module.symvers hello.mod )
