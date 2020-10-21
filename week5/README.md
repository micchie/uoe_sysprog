# Kernel Space Programming

Kernel-space programming is quite different from user-space one. 
We have access to all the hardware and other resources, but need extra care
about correctness and concurrency control.
We code in the kernel when writing device drivers or extending kernel
components such as network stack, scheduler and file system, either for
 production or systems research, including networking,
operating system, database and security.
While following the next sections, please also refer to the [slide](5.pdf).

## 1. Loading and unloading a kernel module

Open `hello-kmalloc.c`.
This code implements the simplest possible kernel module.
`hello_init()`, registered to the kernel by `module_init()`, is called when the module is loaded.
It is registered as a pseudo device so that we can access it using regular file operations. Like other (pseudo) devices, we can find it in `/dev/` while loaded.
This module also allocates a memory by `kmalloc()` to store some data, which
lasts while the module is loaded and goes away when the module is unloaded with
`hello_exit()`, using `kfree()`.
Let's compile and load this kernel module:
```
cp hello-kmalloc.c hello.c
make clean; make
sudo insmod ./hello.ko
```
Now you can see `/dev/hello`
```
sudo rmmod hello
sudo dmesg | tail
```

## 2. File abstraction

Open `hello-fops.c`.  This code implements the device-specific (in our case, the hello pseudo device) *backends* of the common file operations such as `open()`, `read()` and `write()`.
This architecture enables file abstractions for any hardware or pseudo devices.
Also open `hello-test.c`. This (user-space) code utilizes the kernel module; it
opens a device file (`/dev/hello`), writes a string `hello` and then reads from the same file
descriptor. You should notice that the code is quite similar to those we used in the last week ([4a.c](../week4/4a.c) and [4b.c](../../week4/4b.c)); this is what the file abstraction enables.
```
cp hello-fops.c hello.c
make clean; make
sudo insmod ./hello.ko
gcc hello-test.c
sudo ./a.out
sudo rmmod ./hello.ko
```

## 3. Locking

`hello-fops.c` defines spinlock and acquires it while writing or reading data in
the buffer `hello_mem`.
This lock is necessary, because multiple clients can access `hello_mem` at the same time, and some clients may read half-written data.
Since access to the code section between `spin_lock(&hello_lock)` and `spin_unlock(&hello_lock)` is serialized, we place the code that accesses to `hello_mem`.
This module thus ensures atomicity, meaning that in this case, only either the entire write or nothing written becomes visible to the concurrent readers.

Let's make some experiment. Remove `spin_unlock()` in `hello_write()` in
`hello-fops.c`, and run the same steps in the previous section. Your virtual
machine will freeze (and you need to reboot the machine), because the `hello_read()` is waiting for spinlock to be
successful, which never happens.
As we have experienced now, we need to be very careful about locking while
programming in the kernel.

## 4. Debugging

Debugging kernel code is harder than user-space code.
Inserting `printk()` is always your friend, but they are many ways that you can
efficiently debug your code.  Let's try one method that uses `gdb` to debug a
NULL pointer dereference bug.

First, you need to install debug symbols of your kernel:
```
sudo apt-get install gdb linux-image-`uname -r`-dbg
```
Then comment out some lines in `hello-fops.c` not to allocate memory:
```
//	hello_mem = (char *)kmalloc(HELLO_MEMSIZ, GFP_ATOMIC|__GFP_ZERO);
//	if (hello_mem == NULL) {
//		printk(KERN_ERR "%s cannot allocate %lu byte memory\n", __FUNCTION__, HELLO_MEMSIZ);
//		return -1;
//	}
```
Then run the same procedure with section 2 until `./a.out`.
You will see something like following:
```
vagrant@buster:~/uoe_sysprog/week5$ sudo ./a.out

Message from syslogd@buster at Oct 21 22:09:08 ...
 kernel:[  802.293944] usercopy: Kernel memory overwrite attempt detected to
null address (offset 0, size 6)!
Segmentation fault
```
You will also see something like following:
```
vagrant@buster:~/uoe_sysprog/week5$ sudo dmesg | tail -n 30
[  802.296098] Call Trace:
[  802.296169]  __check_object_size.cold.2+0x43/0x47
[  802.296227]  hello_write+0x35/0x60 [hello]
[  802.296274]  vfs_write+0xa5/0x1a0
[  802.296313]  ksys_write+0x57/0xd0
[  802.296355]  do_syscall_64+0x53/0x110
[  802.296427]  entry_SYSCALL_64_after_hwframe+0x44/0xa9
[  802.296490] RIP: 0033:0x7f4912559504
[  802.296531] Code: 00 f7 d8 64 89 02 48 c7 c0 ff ff ff ff eb b3 0f 1f 80 00 00
00 00 48 8d 05 f9 61 0d 00 8b 00 85 c0 75 13 b8 01 00 00 00 0f 05 <48> 3d 00 f0
ff ff 77 54 c3 0f 1f 00 41 54 49 89 d4 55 48 89 f5 53
[  802.296708] RSP: 002b:00007fffd5d524c8 EFLAGS: 00000246 ORIG_RAX:
0000000000000001
...
```
It seems something went wrong in `hello_write`.  Let's try to pinpoint the problem further using `gdb`:
```
vagrant@buster:~/uoe_sysprog/week5$ gdb hello.ko --quiet
Reading symbols from hello.ko...done.
(gdb) l *(hello_write+0x35)
0xa5 is in hello_write
(/usr/src/linux-headers-4.19.0-9-common/include/linux/uaccess.h:147).
142	
143	static __always_inline unsigned long __must_check
144	copy_from_user(void *to, const void __user *from, unsigned long n)
145	{
146		if (likely(check_copy_size(to, n, false)))
147			n = _copy_from_user(to, from, n);
148		return n;
149	}
150	
151	static __always_inline unsigned long __must_check
(gdb) 
```
Above `l *(hello_write+0x35)` tries to print the file and line of where the problem occurs (i.e., `hello_write+0x35`).
We find the problem occurs in `__copy_from_user()`, at the line 147 in
uaccess.h, which is in `copy_from_user()` which we indeed call in
`hello_write()`.
Let's take a look this part, and we are indeed passing
a null pointer (`hello_mem`) as the destination of the copy.
In this way we can debug the kernel code.

