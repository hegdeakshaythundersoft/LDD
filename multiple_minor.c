//#include <linux/kernel.h>
//#include <linux/module.h>
//#include <linux/init.h>
//#include <linux/fs.h>

//#include <linux/kdev_t.h>
//#include <linux/cdev.h>
//#include <linux/device.h>
//#include <linux/fcntl.h> /*Helps fix O_ACCMODE*/
//#include <linux/sched.h> /*Helps fix TASK_UNINTERRUPTIBLE */
//#include <linux/fs.h> /*Helps fix the struct intializer */

#include 	<linux/kernel.h>
#include	<linux/module.h>
#include	<linux/init.h>
#include 	<linux/slab.h>        /* kmalloc() */
#include	<linux/fs.h>
#include	<linux/mm.h>
#include	<linux/cdev.h>
#include	<linux/errno.h>
#include	<linux/types.h>
#include	<linux/interrupt.h>
#include	<linux/delay.h>
#include    <linux/errno.h>
#include    <linux/sched.h>
#include    <linux/semaphore.h>
//#include    <linux/system.h>
#include	<linux/uaccess.h>
//#include	<asm/arch/irqs.h>
#include 	<linux/io.h>
#include 	<linux/version.h>
//#include    <asm/hardware.h>

//#include <sys/types.h> 
dev_t dev =0;


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johny");
MODULE_DESCRIPTION(" major and minor  driver");
MODULE_VERSION("1.0.0");

static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer =NULL;
#define MEM_SIZE        1024           //Memory Size

static int      __init my_driver_init(void);
static void     __exit my_driver_exit(void);
static int      my_open(struct inode *inode, struct file *file);
static int      my_release(struct inode *inode, struct file *file);
static ssize_t  my_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  my_write(struct file *filp, const char *buf, size_t len, loff_t * off);



static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = my_read,
    .write      = my_write,
    .open       = my_open,
    .release    = my_release,
};


static int my_open(struct inode *inode, struct file *file)
{
	
	if((kernel_buffer = kmalloc(MEM_SIZE,GFP_KERNEL))==0)
	{
		printk(KERN_INFO "Cannot allocate memory in kernel\n");
		return -1;
	}
	printk(KERN_INFO"Driver open  function called .....\n");
	return 0;	
}
static int my_release(struct inode *inode, struct file *file)
{
	kfree(kernel_buffer);
	printk(KERN_INFO"Driver release function called ....\n");
	return 0;
}
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	int ret =0;
	//Copy the data from the kernel space to the user-space
    ret = copy_to_user(buf, kernel_buffer, MEM_SIZE);
	if(ret > 0)
	{
		printk(KERN_INFO"writing data to user-space failed\n");
	}
	printk(KERN_INFO"Driver read function called .....\n");
	return 0;
}
static ssize_t my_write(struct file *filp, const char __user *buf,size_t len,loff_t *off)
{
	int ret = 0;
	//Copy the data to kernel space from the user-space
	copy_from_user(kernel_buffer, buf, len);
	if(ret > 0)
	{
		printk(KERN_INFO"Copy the data to kernel space from the user-space\n");
	}
	printk(KERN_INFO"Driver write function called .... \n");
	return len;
} 
static int __init my_driver_init(void)
{
	if((alloc_chrdev_region(&dev,1,3,"my_Char"))<0)// my_char is my driver name 
	{
		printk(KERN_INFO"cannot allocate major and minor number");
		return -1;
	}
	printk(KERN_INFO"Major =%d Minor = %d \n",MAJOR(dev),MINOR(dev));
	
	//creating cdev structure
	cdev_init(&my_cdev,&fops);
	
	//Adding charriver to system
	if((cdev_add(&my_cdev,dev,1)) <0)
	{
		printk(KERN_INFO"cannot add the device to the system \n");
		goto r_class;
	}

	if((dev_class = class_create(THIS_MODULE,"my_Char_class")) == NULL){
		printk(KERN_INFO"Cannot create the struct class for device");
		goto r_class;
	}
	if((device_create(dev_class,NULL,dev,1,"my_Char_driver"))==NULL) {
		printk(KERN_INFO" cannot create the device\n");
		goto r_device;
	}

	printk(KERN_INFO"character driver init sucess\n");

	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

static void __exit my_driver_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "kernel driver removed  ... done \n");
	

}
module_init(my_driver_init);
module_exit(my_driver_exit);
/*akshay@akshay-ThinkPad-L570-W10DG:~/Team$ gedit major_minor.c 
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ gedit major_minoor_dev.c 
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ make
make -C /lib/modules/5.11.0-34-generic/build M=/home/akshay/Team modules
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
  CC [M]  /home/akshay/Team/major_minoor_dev.o
/home/akshay/Team/major_minoor_dev.c: In function ‘char_dev_init’:
/home/akshay/Team/major_minoor_dev.c:81:41: warning: passing argument 4 of ‘device_create’ makes pointer from integer without a cast [-Wint-conversion]
   81 |  if((device_create(dev_class,NULL,mydev,1,"my_Char_driver1"))==NULL) {
      |                                         ^
      |                                         |
      |                                         int
In file included from ./include/linux/cdev.h:8,
                 from /home/akshay/Team/major_minoor_dev.c:5:
./include/linux/device.h:855:1: note: expected ‘void *’ but argument is of type ‘int’
  855 | device_create(struct class *cls, struct device *parent, dev_t devt,
      | ^~~~~~~~~~~~~
  CC [M]  /home/akshay/Team/major_minor.o
/home/akshay/Team/major_minor.c: In function ‘my_driver_init’:
/home/akshay/Team/major_minor.c:131:39: warning: passing argument 4 of ‘device_create’ makes pointer from integer without a cast [-Wint-conversion]
  131 |  if((device_create(dev_class,NULL,dev,1,"my_Char_driver"))==NULL) {
      |                                       ^
      |                                       |
      |                                       int
In file included from ./include/linux/cdev.h:8,
                 from /home/akshay/Team/major_minor.c:19:
./include/linux/device.h:855:1: note: expected ‘void *’ but argument is of type ‘int’
  855 | device_create(struct class *cls, struct device *parent, dev_t devt,
      | ^~~~~~~~~~~~~
/home/akshay/Team/major_minor.c: In function ‘my_write’:
/home/akshay/Team/major_minor.c:100:2: warning: ignoring return value of ‘copy_from_user’, declared with attribute warn_unused_result [-Wunused-result]
  100 |  copy_from_user(kernel_buffer, buf, len);
      |  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  MODPOST /home/akshay/Team/Module.symvers
  CC [M]  /home/akshay/Team/major_minoor_dev.mod.o
  LD [M]  /home/akshay/Team/major_minoor_dev.ko
  CC [M]  /home/akshay/Team/major_minor.mod.o
  LD [M]  /home/akshay/Team/major_minor.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ sudo insmod major_minor.ko
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ lsmod | head
Module                  Size  Used by
major_minor            16384  0
rfcomm                 81920  4
ccm                    20480  0
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
bnep                   24576  2
nls_iso8859_1          16384  1
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ ls -al /dev/
total 4
drwxr-xr-x  21 root   root        4620 Sep 17 18:14 .
drwxr-xr-x  20 root   root        4096 Apr  9 01:50 ..
crw-r--r--   1 root   root     10, 235 Sep 17 17:24 autofs
drwxr-xr-x   2 root   root         520 Sep 17 17:24 block
drwxr-xr-x   2 root   root          60 Sep 17 17:24 bsg
crw-------   1 root   root     10, 234 Sep 17 17:24 btrfs-control
drwxr-xr-x   3 root   root          60 Sep 17 17:24 bus
drwxr-xr-x   2 root   root        4540 Sep 17 18:14 char
crw--w----   1 root   tty       5,   1 Sep 17 17:24 console
lrwxrwxrwx   1 root   root          11 Sep 17 17:24 core -> /proc/kcore
drwxr-xr-x   2 root   root          60 Sep 17 17:24 cpu
crw-------   1 root   root     10, 123 Sep 17 17:24 cpu_dma_latency
crw-------   1 root   root     10, 203 Sep 17 17:24 cuse
drwxr-xr-x   7 root   root         140 Sep 17 17:24 disk
drwxr-xr-x   2 root   root          60 Sep 17 17:24 dma_heap
drwxr-xr-x   3 root   root         100 Sep 17 17:24 dri
crw-------   1 root   root    235,   0 Sep 17 17:24 drm_dp_aux0
crw-------   1 root   root    235,   1 Sep 17 17:24 drm_dp_aux1
crw-------   1 root   root    235,   2 Sep 17 17:24 drm_dp_aux2
crw-------   1 root   root     10, 126 Sep 17 17:24 ecryptfs
crw-rw----   1 root   video    29,   0 Sep 17 17:24 fb0
lrwxrwxrwx   1 root   root          13 Sep 17 17:24 fd -> /proc/self/fd
crw-rw-rw-   1 root   root      1,   7 Sep 17 17:24 full
crw-rw-rw-   1 root   root     10, 229 Sep 17 17:24 fuse
crw-------   1 root   root    240,   0 Sep 17 18:01 hidraw0
crw-------   1 root   root    240,   1 Sep 17 17:24 hidraw1
crw-------   1 root   root    240,   2 Sep 17 17:24 hidraw2
crw-------   1 root   root     10, 228 Sep 17 17:24 hpet
drwxr-xr-x   2 root   root           0 Sep 17 17:24 hugepages
crw-------   1 root   root     10, 183 Sep 17 17:24 hwrng
crw-------   1 root   root     89,   0 Sep 17 17:24 i2c-0
crw-------   1 root   root     89,   1 Sep 17 17:24 i2c-1
crw-------   1 root   root     89,   2 Sep 17 17:24 i2c-2
crw-------   1 root   root     89,   3 Sep 17 17:24 i2c-3
crw-------   1 root   root     89,   4 Sep 17 17:24 i2c-4
crw-------   1 root   root     89,   5 Sep 17 17:24 i2c-5
crw-------   1 root   root     89,   6 Sep 17 17:24 i2c-6
lrwxrwxrwx   1 root   root          12 Sep 17 17:24 initctl -> /run/initctl
drwxr-xr-x   4 root   root         620 Sep 17 18:01 input
crw-r--r--   1 root   root      1,  11 Sep 17 17:24 kmsg
crw-rw----+  1 root   kvm      10, 232 Sep 17 17:24 kvm
drwxr-xr-x   2 root   root          60 Sep 17 17:24 lightnvm
lrwxrwxrwx   1 root   root          28 Sep 17 17:24 log -> /run/systemd/journal/dev-log
brw-rw----   1 root   disk      7,   0 Sep 17 17:24 loop0
brw-rw----   1 root   disk      7,   1 Sep 17 17:24 loop1
brw-rw----   1 root   disk      7,  10 Sep 17 17:24 loop10
brw-rw----   1 root   disk      7,  11 Sep 17 17:24 loop11
brw-rw----   1 root   disk      7,  12 Sep 17 17:24 loop12
brw-rw----   1 root   disk      7,  13 Sep 17 17:24 loop13
brw-rw----   1 root   disk      7,  14 Sep 17 17:24 loop14
brw-rw----   1 root   disk      7,   2 Sep 17 17:24 loop2
brw-rw----   1 root   disk      7,   3 Sep 17 17:24 loop3
brw-rw----   1 root   disk      7,   4 Sep 17 17:24 loop4
brw-rw----   1 root   disk      7,   5 Sep 17 17:24 loop5
brw-rw----   1 root   disk      7,   6 Sep 17 17:24 loop6
brw-rw----   1 root   disk      7,   7 Sep 17 17:24 loop7
brw-rw----   1 root   disk      7,   8 Sep 17 17:24 loop8
brw-rw----   1 root   disk      7,   9 Sep 17 17:24 loop9
crw-rw----   1 root   disk     10, 237 Sep 17 17:24 loop-control
drwxr-xr-x   2 root   root          60 Sep 17 17:24 mapper
crw-------   1 root   root     10, 227 Sep 17 17:24 mcelog
crw-rw----   1 root   video   239,   0 Sep 17 17:24 media0
crw-------   1 root   root    236,   0 Sep 17 17:24 mei0
crw-r-----   1 root   kmem      1,   1 Sep 17 17:24 mem
drwxrwxrwt   2 root   root          40 Sep 17 17:24 mqueue
crwxrwxrwx   1 root   root    300,   0 Sep 17 17:24 my_Cdev
crw-------   1 root   root    508,   1 Sep 17 18:14 my_Char_driver
drwxr-xr-x   2 root   root          60 Sep 17 17:24 net
crw-rw-rw-   1 root   root      1,   3 Sep 17 17:24 null
crw-r-----   1 root   kmem     10, 144 Sep 17 17:24 nvram
crw-r-----   1 root   kmem      1,   4 Sep 17 17:24 port
crw-------   1 root   root    108,   0 Sep 17 17:24 ppp
crw-------   1 root   root     10,   1 Sep 17 17:24 psaux
crw-rw-rw-   1 root   tty       5,   2 Sep 17 18:15 ptmx
crw-------   1 root   root    246,   0 Sep 17 17:24 ptp0
drwxr-xr-x   2 root   root           0 Sep 17 17:24 pts
crw-rw-rw-   1 root   root      1,   8 Sep 17 17:24 random
crw-rw-r--+  1 root   root     10, 242 Sep 17 17:24 rfkill
lrwxrwxrwx   1 root   root           4 Sep 17 17:24 rtc -> rtc0
crw-------   1 root   root    248,   0 Sep 17 17:24 rtc0
brw-rw----   1 root   disk      8,   0 Sep 17 17:24 sda
brw-rw----   1 root   disk      8,   1 Sep 17 17:24 sda1
brw-rw----   1 root   disk      8,   2 Sep 17 17:24 sda2
brw-rw----   1 root   disk      8,   3 Sep 17 17:24 sda3
brw-rw----   1 root   disk      8,   4 Sep 17 17:24 sda4
brw-rw----   1 root   disk      8,   5 Sep 17 17:24 sda5
brw-rw----   1 root   disk      8,   6 Sep 17 17:24 sda6
brw-rw----   1 root   disk      8,   7 Sep 17 17:24 sda7
brw-rw----   1 root   disk      8,   8 Sep 17 17:24 sda8
crw-rw----   1 root   disk     21,   0 Sep 17 17:24 sg0
drwxrwxrwt   2 root   root          40 Sep 17 18:15 shm
crw-------   1 root   root     10, 231 Sep 17 17:24 snapshot
drwxr-xr-x   3 root   root         300 Sep 17 17:24 snd
lrwxrwxrwx   1 root   root          15 Sep 17 17:24 stderr -> /proc/self/fd/2
lrwxrwxrwx   1 root   root          15 Sep 17 17:24 stdin -> /proc/self/fd/0
lrwxrwxrwx   1 root   root          15 Sep 17 17:24 stdout -> /proc/self/fd/1
crw-rw----   1 tss    root     10, 224 Sep 17 17:24 tpm0
crw-rw-rw-   1 root   tty       5,   0 Sep 17 18:10 tty
crw--w----   1 root   tty       4,   0 Sep 17 17:24 tty0
crw--w----   1 root   tty       4,   1 Sep 17 17:24 tty1
crw--w----   1 root   tty       4,  10 Sep 17 17:24 tty10
crw--w----   1 root   tty       4,  11 Sep 17 17:24 tty11
crw--w----   1 root   tty       4,  12 Sep 17 17:24 tty12
crw--w----   1 root   tty       4,  13 Sep 17 17:24 tty13
crw--w----   1 root   tty       4,  14 Sep 17 17:24 tty14
crw--w----   1 root   tty       4,  15 Sep 17 17:24 tty15
crw--w----   1 root   tty       4,  16 Sep 17 17:24 tty16
crw--w----   1 root   tty       4,  17 Sep 17 17:24 tty17
crw--w----   1 root   tty       4,  18 Sep 17 17:24 tty18
crw--w----   1 root   tty       4,  19 Sep 17 17:24 tty19
crw--w----   1 akshay tty       4,   2 Sep 17 17:24 tty2
crw--w----   1 root   tty       4,  20 Sep 17 17:24 tty20
crw--w----   1 root   tty       4,  21 Sep 17 17:24 tty21
crw--w----   1 root   tty       4,  22 Sep 17 17:24 tty22
crw--w----   1 root   tty       4,  23 Sep 17 17:24 tty23
crw--w----   1 root   tty       4,  24 Sep 17 17:24 tty24
crw--w----   1 root   tty       4,  25 Sep 17 17:24 tty25
crw--w----   1 root   tty       4,  26 Sep 17 17:24 tty26
crw--w----   1 root   tty       4,  27 Sep 17 17:24 tty27
crw--w----   1 root   tty       4,  28 Sep 17 17:24 tty28
crw--w----   1 root   tty       4,  29 Sep 17 17:24 tty29
crw--w----   1 root   tty       4,   3 Sep 17 17:24 tty3
crw--w----   1 root   tty       4,  30 Sep 17 17:24 tty30
crw--w----   1 root   tty       4,  31 Sep 17 17:24 tty31
crw--w----   1 root   tty       4,  32 Sep 17 17:24 tty32
crw--w----   1 root   tty       4,  33 Sep 17 17:24 tty33
crw--w----   1 root   tty       4,  34 Sep 17 17:24 tty34
crw--w----   1 root   tty       4,  35 Sep 17 17:24 tty35
crw--w----   1 root   tty       4,  36 Sep 17 17:24 tty36
crw--w----   1 root   tty       4,  37 Sep 17 17:24 tty37
crw--w----   1 root   tty       4,  38 Sep 17 17:24 tty38
crw--w----   1 root   tty       4,  39 Sep 17 17:24 tty39
crw--w----   1 root   tty       4,   4 Sep 17 17:24 tty4
crw--w----   1 root   tty       4,  40 Sep 17 17:24 tty40
crw--w----   1 root   tty       4,  41 Sep 17 17:24 tty41
crw--w----   1 root   tty       4,  42 Sep 17 17:24 tty42
crw--w----   1 root   tty       4,  43 Sep 17 17:24 tty43
crw--w----   1 root   tty       4,  44 Sep 17 17:24 tty44
crw--w----   1 root   tty       4,  45 Sep 17 17:24 tty45
crw--w----   1 root   tty       4,  46 Sep 17 17:24 tty46
crw--w----   1 root   tty       4,  47 Sep 17 17:24 tty47
crw--w----   1 root   tty       4,  48 Sep 17 17:24 tty48
crw--w----   1 root   tty       4,  49 Sep 17 17:24 tty49
crw--w----   1 root   tty       4,   5 Sep 17 17:24 tty5
crw--w----   1 root   tty       4,  50 Sep 17 17:24 tty50
crw--w----   1 root   tty       4,  51 Sep 17 17:24 tty51
crw--w----   1 root   tty       4,  52 Sep 17 17:24 tty52
crw--w----   1 root   tty       4,  53 Sep 17 17:24 tty53
crw--w----   1 root   tty       4,  54 Sep 17 17:24 tty54
crw--w----   1 root   tty       4,  55 Sep 17 17:24 tty55
crw--w----   1 root   tty       4,  56 Sep 17 17:24 tty56
crw--w----   1 root   tty       4,  57 Sep 17 17:24 tty57
crw--w----   1 root   tty       4,  58 Sep 17 17:24 tty58
crw--w----   1 root   tty       4,  59 Sep 17 17:24 tty59
crw--w----   1 root   tty       4,   6 Sep 17 17:24 tty6
crw--w----   1 root   tty       4,  60 Sep 17 17:24 tty60
crw--w----   1 root   tty       4,  61 Sep 17 17:24 tty61
crw--w----   1 root   tty       4,  62 Sep 17 17:24 tty62
crw--w----   1 root   tty       4,  63 Sep 17 17:24 tty63
crw--w----   1 root   tty       4,   7 Sep 17 17:24 tty7
crw--w----   1 root   tty       4,   8 Sep 17 17:24 tty8
crw--w----   1 root   tty       4,   9 Sep 17 17:24 tty9
crw-------   1 root   root      5,   3 Sep 17 17:24 ttyprintk
crw-rw----   1 root   dialout   4,  64 Sep 17 17:24 ttyS0
crw-rw----   1 root   dialout   4,  65 Sep 17 17:24 ttyS1
crw-rw----   1 root   dialout   4,  74 Sep 17 17:24 ttyS10
crw-rw----   1 root   dialout   4,  75 Sep 17 17:24 ttyS11
crw-rw----   1 root   dialout   4,  76 Sep 17 17:24 ttyS12
crw-rw----   1 root   dialout   4,  77 Sep 17 17:24 ttyS13
crw-rw----   1 root   dialout   4,  78 Sep 17 17:24 ttyS14
crw-rw----   1 root   dialout   4,  79 Sep 17 17:24 ttyS15
crw-rw----   1 root   dialout   4,  80 Sep 17 17:24 ttyS16
crw-rw----   1 root   dialout   4,  81 Sep 17 17:24 ttyS17
crw-rw----   1 root   dialout   4,  82 Sep 17 17:24 ttyS18
crw-rw----   1 root   dialout   4,  83 Sep 17 17:24 ttyS19
crw-rw----   1 root   dialout   4,  66 Sep 17 17:24 ttyS2
crw-rw----   1 root   dialout   4,  84 Sep 17 17:24 ttyS20
crw-rw----   1 root   dialout   4,  85 Sep 17 17:24 ttyS21
crw-rw----   1 root   dialout   4,  86 Sep 17 17:24 ttyS22
crw-rw----   1 root   dialout   4,  87 Sep 17 17:24 ttyS23
crw-rw----   1 root   dialout   4,  88 Sep 17 17:24 ttyS24
crw-rw----   1 root   dialout   4,  89 Sep 17 17:24 ttyS25
crw-rw----   1 root   dialout   4,  90 Sep 17 17:24 ttyS26
crw-rw----   1 root   dialout   4,  91 Sep 17 17:24 ttyS27
crw-rw----   1 root   dialout   4,  92 Sep 17 17:24 ttyS28
crw-rw----   1 root   dialout   4,  93 Sep 17 17:24 ttyS29
crw-rw----   1 root   dialout   4,  67 Sep 17 17:24 ttyS3
crw-rw----   1 root   dialout   4,  94 Sep 17 17:24 ttyS30
crw-rw----   1 root   dialout   4,  95 Sep 17 17:24 ttyS31
crw-rw----   1 root   dialout   4,  68 Sep 17 17:24 ttyS4
crw-rw----   1 root   dialout   4,  69 Sep 17 17:24 ttyS5
crw-rw----   1 root   dialout   4,  70 Sep 17 17:24 ttyS6
crw-rw----   1 root   dialout   4,  71 Sep 17 17:24 ttyS7
crw-rw----   1 root   dialout   4,  72 Sep 17 17:24 ttyS8
crw-rw----   1 root   dialout   4,  73 Sep 17 17:24 ttyS9
crw-rw----   1 root   kvm      10, 124 Sep 17 17:24 udmabuf
crw-------   1 root   root     10, 239 Sep 17 17:24 uhid
crw-------   1 root   root     10, 223 Sep 17 17:24 uinput
crw-rw-rw-   1 root   root      1,   9 Sep 17 17:24 urandom
crw-------   1 root   root     10, 240 Sep 17 17:24 userio
drwxr-xr-x   4 root   root          80 Sep 17 17:24 v4l
crw-rw----   1 root   tty       7,   0 Sep 17 17:24 vcs
crw-rw----   1 root   tty       7,   1 Sep 17 17:24 vcs1
crw-rw----   1 root   tty       7,   2 Sep 17 17:24 vcs2
crw-rw----   1 root   tty       7,   3 Sep 17 17:24 vcs3
crw-rw----   1 root   tty       7,   4 Sep 17 17:24 vcs4
crw-rw----   1 root   tty       7,   5 Sep 17 17:24 vcs5
crw-rw----   1 root   tty       7,   6 Sep 17 17:24 vcs6
crw-rw----   1 root   tty       7, 128 Sep 17 17:24 vcsa
crw-rw----   1 root   tty       7, 129 Sep 17 17:24 vcsa1
crw-rw----   1 root   tty       7, 130 Sep 17 17:24 vcsa2
crw-rw----   1 root   tty       7, 131 Sep 17 17:24 vcsa3
crw-rw----   1 root   tty       7, 132 Sep 17 17:24 vcsa4
crw-rw----   1 root   tty       7, 133 Sep 17 17:24 vcsa5
crw-rw----   1 root   tty       7, 134 Sep 17 17:24 vcsa6
crw-rw----   1 root   tty       7,  64 Sep 17 17:24 vcsu
crw-rw----   1 root   tty       7,  65 Sep 17 17:24 vcsu1
crw-rw----   1 root   tty       7,  66 Sep 17 17:24 vcsu2
crw-rw----   1 root   tty       7,  67 Sep 17 17:24 vcsu3
crw-rw----   1 root   tty       7,  68 Sep 17 17:24 vcsu4
crw-rw----   1 root   tty       7,  69 Sep 17 17:24 vcsu5
crw-rw----   1 root   tty       7,  70 Sep 17 17:24 vcsu6
drwxr-xr-x   2 root   root          60 Sep 17 17:24 vfio
crw-------   1 root   root     10, 127 Sep 17 17:24 vga_arbiter
crw-------   1 root   root     10, 137 Sep 17 17:24 vhci
crw-------   1 root   root     10, 238 Sep 17 17:24 vhost-net
crw-------   1 root   root     10, 241 Sep 17 17:24 vhost-vsock
crw-rw----+  1 root   video    81,   0 Sep 17 17:24 video0
crw-rw----+  1 root   video    81,   1 Sep 17 17:24 video1
crw-rw-rw-   1 root   root      1,   5 Sep 17 17:24 zero
crw-------   1 root   root     10, 249 Sep 17 17:24 zfs
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ gedit major_minoor_dev.c
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ make
make -C /lib/modules/5.11.0-34-generic/build M=/home/akshay/Team modules
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
  CC [M]  /home/akshay/Team/major_minoor_dev.o
/home/akshay/Team/major_minoor_dev.c: In function ‘char_dev_init’:
/home/akshay/Team/major_minoor_dev.c:81:41: warning: passing argument 4 of ‘device_create’ makes pointer from integer without a cast [-Wint-conversion]
   81 |  if((device_create(dev_class,NULL,mydev,1,"my_Char_driver1"))==NULL) {
      |                                         ^
      |                                         |
      |                                         int
In file included from ./include/linux/cdev.h:8,
                 from /home/akshay/Team/major_minoor_dev.c:5:
./include/linux/device.h:855:1: note: expected ‘void *’ but argument is of type ‘int’
  855 | device_create(struct class *cls, struct device *parent, dev_t devt,
      | ^~~~~~~~~~~~~
  MODPOST /home/akshay/Team/Module.symvers
  CC [M]  /home/akshay/Team/major_minoor_dev.mod.o
  LD [M]  /home/akshay/Team/major_minoor_dev.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ sudo insmod major_minoor_dev.ko
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ lsmod | head
Module                  Size  Used by
major_minoor_dev       20480  0
major_minor            16384  0
rfcomm                 81920  4
ccm                    20480  0
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
bnep                   24576  2
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ sudo chmod 777 /dev/my_C
my_Cdev          my_Char_driver   my_Char_driver1  
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ sudo chmod 777 /dev/my_Char_driver
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ sudo chmod 777 /dev/my_Char_driver1
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ gcc Testapp_device.c 
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ ./a.out
****Please Enter the Option******
        1. Write               
        2. Read                 
		3. IOCTL				
        4. Exit                 
*********************************
1
Your Option = 1
Enter the string to write into driver :hello
Data Writing ...Done!
****Please Enter the Option******
        1. Write               
        2. Read                 
		3. IOCTL				
        4. Exit                 
*********************************
2
Your Option = 2
Data Reading ...Done!

Data = 

****Please Enter the Option******
        1. Write               
        2. Read                 
		3. IOCTL				
        4. Exit                 
*********************************
4
Your Option = 4
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ cat /proc/devices 
Character devices:
  1 mem
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
  5 ttyprintk
  6 lp
  7 vcs
 10 misc
 13 input
 21 sg
 29 fb
 81 video4linux
 89 i2c
 99 ppdev
108 ppp
116 alsa
128 ptm
136 pts
180 usb
189 usb_device
204 ttyMAX
216 rfcomm
226 drm
234 my_Char
235 aux
236 mei
237 cec
238 lirc
239 media
240 hidraw
241 ttyDBC
242 vfio
243 bsg
244 watchdog
245 remoteproc
246 ptp
247 pps
248 rtc
249 dma_heap
250 dax
251 dimmctl
252 ndctl
253 tpm
254 gpiochip
508 my_Char
509 my_Char
510 my_Char
511 my_Char

Block devices:
  7 loop
  8 sd
  9 md
 11 sr
 65 sd
 66 sd
 67 sd
 68 sd
 69 sd
 70 sd
 71 sd
128 sd
129 sd
130 sd
131 sd
132 sd
133 sd
134 sd
135 sd
253 device-mapper
254 mdp
259 blkext
akshay@akshay-ThinkPad-L570-W10DG:~/Team$ ls -al /dev/
total 4
drwxr-xr-x  21 root   root        4640 Sep 17 18:16 .
drwxr-xr-x  20 root   root        4096 Apr  9 01:50 ..
crw-r--r--   1 root   root     10, 235 Sep 17 17:24 autofs
drwxr-xr-x   2 root   root         520 Sep 17 17:24 block
drwxr-xr-x   2 root   root          60 Sep 17 17:24 bsg
crw-------   1 root   root     10, 234 Sep 17 17:24 btrfs-control
drwxr-xr-x   3 root   root          60 Sep 17 17:24 bus
drwxr-xr-x   2 root   root        4560 Sep 17 18:16 char
crw--w----   1 root   tty       5,   1 Sep 17 17:24 console
lrwxrwxrwx   1 root   root          11 Sep 17 17:24 core -> /proc/kcore
drwxr-xr-x   2 root   root          60 Sep 17 17:24 cpu
crw-------   1 root   root     10, 123 Sep 17 17:24 cpu_dma_latency
crw-------   1 root   root     10, 203 Sep 17 17:24 cuse
drwxr-xr-x   7 root   root         140 Sep 17 17:24 disk
drwxr-xr-x   2 root   root          60 Sep 17 17:24 dma_heap
drwxr-xr-x   3 root   root         100 Sep 17 17:24 dri
crw-------   1 root   root    235,   0 Sep 17 17:24 drm_dp_aux0
crw-------   1 root   root    235,   1 Sep 17 17:24 drm_dp_aux1
crw-------   1 root   root    235,   2 Sep 17 17:24 drm_dp_aux2
crw-------   1 root   root     10, 126 Sep 17 17:24 ecryptfs
crw-rw----   1 root   video    29,   0 Sep 17 17:24 fb0
lrwxrwxrwx   1 root   root          13 Sep 17 17:24 fd -> /proc/self/fd
crw-rw-rw-   1 root   root      1,   7 Sep 17 17:24 full
crw-rw-rw-   1 root   root     10, 229 Sep 17 17:24 fuse
crw-------   1 root   root    240,   0 Sep 17 18:01 hidraw0
crw-------   1 root   root    240,   1 Sep 17 17:24 hidraw1
crw-------   1 root   root    240,   2 Sep 17 17:24 hidraw2
crw-------   1 root   root     10, 228 Sep 17 17:24 hpet
drwxr-xr-x   2 root   root           0 Sep 17 17:24 hugepages
crw-------   1 root   root     10, 183 Sep 17 17:24 hwrng
crw-------   1 root   root     89,   0 Sep 17 17:24 i2c-0
crw-------   1 root   root     89,   1 Sep 17 17:24 i2c-1
crw-------   1 root   root     89,   2 Sep 17 17:24 i2c-2
crw-------   1 root   root     89,   3 Sep 17 17:24 i2c-3
crw-------   1 root   root     89,   4 Sep 17 17:24 i2c-4
crw-------   1 root   root     89,   5 Sep 17 17:24 i2c-5
crw-------   1 root   root     89,   6 Sep 17 17:24 i2c-6
lrwxrwxrwx   1 root   root          12 Sep 17 17:24 initctl -> /run/initctl
drwxr-xr-x   4 root   root         620 Sep 17 18:01 input
crw-r--r--   1 root   root      1,  11 Sep 17 17:24 kmsg
crw-rw----+  1 root   kvm      10, 232 Sep 17 17:24 kvm
drwxr-xr-x   2 root   root          60 Sep 17 17:24 lightnvm
lrwxrwxrwx   1 root   root          28 Sep 17 17:24 log -> /run/systemd/journal/dev-log
brw-rw----   1 root   disk      7,   0 Sep 17 17:24 loop0
brw-rw----   1 root   disk      7,   1 Sep 17 17:24 loop1
brw-rw----   1 root   disk      7,  10 Sep 17 17:24 loop10
brw-rw----   1 root   disk      7,  11 Sep 17 17:24 loop11
brw-rw----   1 root   disk      7,  12 Sep 17 17:24 loop12
brw-rw----   1 root   disk      7,  13 Sep 17 17:24 loop13
brw-rw----   1 root   disk      7,  14 Sep 17 17:24 loop14
brw-rw----   1 root   disk      7,   2 Sep 17 17:24 loop2
brw-rw----   1 root   disk      7,   3 Sep 17 17:24 loop3
brw-rw----   1 root   disk      7,   4 Sep 17 17:24 loop4
brw-rw----   1 root   disk      7,   5 Sep 17 17:24 loop5
brw-rw----   1 root   disk      7,   6 Sep 17 17:24 loop6
brw-rw----   1 root   disk      7,   7 Sep 17 17:24 loop7
brw-rw----   1 root   disk      7,   8 Sep 17 17:24 loop8
brw-rw----   1 root   disk      7,   9 Sep 17 17:24 loop9
crw-rw----   1 root   disk     10, 237 Sep 17 17:24 loop-control
drwxr-xr-x   2 root   root          60 Sep 17 17:24 mapper
crw-------   1 root   root     10, 227 Sep 17 17:24 mcelog
crw-rw----   1 root   video   239,   0 Sep 17 17:24 media0
crw-------   1 root   root    236,   0 Sep 17 17:24 mei0
crw-r-----   1 root   kmem      1,   1 Sep 17 17:24 mem
drwxrwxrwt   2 root   root          40 Sep 17 17:24 mqueue
crwxrwxrwx   1 root   root    300,   0 Sep 17 17:24 my_Cdev
crwxrwxrwx   1 root   root    508,   1 Sep 17 18:14 my_Char_driver
crwxrwxrwx   1 root   root    508,   2 Sep 17 18:16 my_Char_driver1
*/

