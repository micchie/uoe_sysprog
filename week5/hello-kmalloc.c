#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michio Honda");
MODULE_DESCRIPTION("Hello World in the kernel.");

#define DEVICE_NAME "hello"
static struct miscdevice *hello_dev;
#define HELLO_MEMSIZ	1024
static char *hello_mem;

static int
hello_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int
hello_release(struct inode *inode, struct file *file)
{
	return 0;
}

static long
hello_unlocked_ioctl(struct file *file, u_int cmd, u_long data /* arg */)
{
	return 0;
}

/* This structure points to all of the device functions */
static struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.unlocked_ioctl = hello_unlocked_ioctl,
	.release = hello_release
};

static struct miscdevice hello_miscdev = 
{
	MISC_DYNAMIC_MINOR,
	"hello",
	&hello_fops
};

static int __init
hello_init(void)
{
	int error = misc_register(&hello_miscdev);
	if (error) {
		printk(KERN_ERR "%s misc_register() failed\n", __FUNCTION__);
		return -1;
	}
	hello_mem = (char *)kmalloc(HELLO_MEMSIZ, GFP_ATOMIC|__GFP_ZERO);
	if (hello_mem == NULL) {
		printk(KERN_ERR "%s cannot allocate %d byte memory\n", __FUNCTION__, HELLO_MEMSIZ);
		return -1;
	}

	hello_dev = &hello_miscdev;
	printk(KERN_INFO "loaded hello\n");
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

static void __exit
hello_exit(void)
{
	if (hello_dev)
		misc_deregister(hello_dev);
	kfree(hello_mem);
	printk(KERN_INFO "unloaded hello\n");
}

module_init(hello_init);
module_exit(hello_exit);
