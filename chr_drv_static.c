#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define MAJORNO 300
#define MINORNO  0
#define CHAR_DEV_NAME "my_cdev"
#define MAX_LENGTH 4000
#define SUCCESS 0

static char char_device_buf[MAX_LENGTH];
struct cdev *my_cdev;
dev_t mydev;
int count=1,inuse=0;


static int char_dev_open(struct inode *inode,
			    struct file  *file)
{
	if(inuse)
	{
		printk(KERN_INFO "\nDevice busy %s\n",CHAR_DEV_NAME);
		return -EBUSY;
	}	
	inuse=1;
	printk(KERN_INFO "Open operation invoked \n");
	return SUCCESS;
		
}

static int char_dev_release(struct inode *inode, struct file *file)
{
	inuse=0;
	return SUCCESS;
}

static ssize_t char_dev_write(struct file *file, const char *buf, size_t lbuf, loff_t *ppos)
{
       int nbytes = lbuf - raw_copy_from_user (char_device_buf + *ppos, buf, lbuf);
       *ppos += nbytes;
       printk (KERN_INFO "\n Rec'vd data from app %s , nbytes=%d\n",char_device_buf,nbytes);

       return nbytes;

}

static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release
};

static int __init char_dev_init(void)
{
	int ret;
	mydev = MKDEV(MAJORNO,MINORNO);
	register_chrdev_region(mydev,count,CHAR_DEV_NAME); 

	my_cdev= cdev_alloc();  // allow allocate cdev instance, 
	cdev_init(my_cdev,&char_dev_fops);  

	ret=cdev_add(my_cdev,mydev,count);// register with VFS layer , count = how many minor no. in use .
	
	if( ret < 0 ) {
		printk("Error registering device driver\n");
		return ret;
	}
	printk(KERN_INFO"\nDevice Registered %s\n",CHAR_DEV_NAME); 
	
	memset(char_device_buf,'\0',MAX_LENGTH);
	return 0;
}

static void __exit char_dev_exit(void)
{
	
	 cdev_del(my_cdev);
	 unregister_chrdev_region(mydev,1);
	 printk(KERN_INFO "\n Driver unregistered \n");
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("akshay_hegde");
MODULE_DESCRIPTION("Static Character Device Driver");
MODULE_LICENSE("GPL");



/* End of code */

    /*akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit chr_drv_static.c 
^[[Aakshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make
make -C /lib/modules/5.11.0-34-generic/build M=/home/akshay/linux_device_driver modules
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
  CC [M]  /home/akshay/linux_device_driver/chr_drv_static.o
  MODPOST /home/akshay/linux_device_driver/Module.symvers
  CC [M]  /home/akshay/linux_device_driver/chr_drv_static.mod.o
  LD [M]  /home/akshay/linux_device_driver/chr_drv_static.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod chr_drv_static.ko
[sudo] password for akshay: 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head
Module                  Size  Used by
chr_drv_static         20480  0
rfcomm                 81920  4
ccm                    20480  6
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
bnep                   24576  2
nls_iso8859_1          16384  1
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo mkn
mknod   mkntfs  
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo mknod  /dev/my_Cdev c 300 0
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ cat /proc/devices 
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
300 my_cdev
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
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ dmesg | tail 
               Device Registered: my_cdrv
[18455.479591] Major number = 508, Minor number = 0
[18546.317044] Number of times open() was called: 1
[18546.317048]  MAJOR number = 508, MINOR number = 0
[18546.317049] Process id of the current process: 23006
[18546.317050] ref=1
[18590.793848] 
                Driver unregistered 
[19707.566431] 
               Device Registered my_cdev

*/
