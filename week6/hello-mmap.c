#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michio Honda");
MODULE_DESCRIPTION("Hello World in the kernel.");

#define DEVICE_NAME “hello”
static struct miscdevice *hello_dev;

static const size_t HELLO_MEMSIZ = 8192;
static char *hello_mem;
static DEFINE_SPINLOCK(hello_mem_lock);

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

static ssize_t hello_read(struct file *filp, char *buf, size_t len, loff_t *off)
{
	unsigned long notcopied;

	spin_lock(&hello_mem_lock);
	notcopied = copy_to_user(buf, hello_mem, min(len, HELLO_MEMSIZ));
	spin_unlock(&hello_mem_lock);

	return notcopied;
}

static ssize_t hello_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
	unsigned long notcopied;

	spin_lock(&hello_mem_lock);
	notcopied = copy_from_user(hello_mem, buf, min(len, HELLO_MEMSIZ));
	spin_unlock(&hello_mem_lock);

	return notcopied;
}

static int
hello_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	unsigned long off = (vma->vm_pgoff + vmf->pgoff) << PAGE_SHIFT;
	struct page *page;

	printk(KERN_INFO "%s off %lu vma->vm_pgoff %lu vmf->pgoff %lu\n",
			__FUNCTION__, off, vma->vm_pgoff, vmf->pgoff);
	
	page = virt_to_page(hello_mem + off);
	get_page(page);
	vmf->page = page;
	return 0;
}

static struct vm_operations_struct hello_mmap_ops = {
	.fault = hello_fault,
};

static int
hello_mmap(struct file *f, struct vm_area_struct *vma)
{
	int error = 0;

	printk(KERN_INFO "%s vma->vm_pgoff %lu vma->vm_start %lu vma->vm_end %lu\n",
			__FUNCTION__, vma->vm_pgoff, vma->vm_start, vma->vm_end);
	vma->vm_ops = &hello_mmap_ops;
	return error;
}

/* This structure points to all of the device functions */
static struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.read = hello_read,
	.write = hello_write,
	.mmap = hello_mmap,
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
		printk(KERN_ERR "%s cannot allocate %lu byte memory\n", __FUNCTION__, HELLO_MEMSIZ);
		return -1;
	}
	hello_mem[HELLO_MEMSIZ-1] = 'x';

	hello_dev = &hello_miscdev;
	printk(KERN_INFO "loaded hello\n");
	return 0;
}

static void __exit
hello_exit(void)
{
	if (hello_dev)
		misc_deregister(hello_dev);
	printk(KERN_INFO "%s hello_mem[HELLO_MEMSIZ-1] %c\n", __FUNCTION__, hello_mem[HELLO_MEMSIZ-1]);
	kfree(hello_mem);
	printk(KERN_INFO "unloaded hello\n");
}

module_init(hello_init);
module_exit(hello_exit);
