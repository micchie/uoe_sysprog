.PHONY: module app clean hello.c hello.o
obj-m += hello.o
PROG = module app

all: $(PROG)
module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	(rm -rf *.o *.ko modules.order hello.mod.c Module.symvers hello.mod )
