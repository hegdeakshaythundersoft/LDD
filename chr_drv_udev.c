#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

#define CHAR_DEV_NAME "my_cdrv"
#define MAX_LENGTH 4000
#define SUCCESS 0

static char *char_device_buf;
struct cdev *my_cdev;
dev_t mydev;
int count=1;
static struct class *my_class;

static int char_dev_open(struct inode *inode,
			    struct file  *file)
{
	static int counter = 0;
	counter++;
	printk(KERN_INFO "Number of times open() was called: %d\n", counter);
	printk (KERN_INFO " MAJOR number = %d, MINOR number = %d\n",imajor (inode), iminor (inode));
	printk(KERN_INFO "Process id of the current process: %d\n",current->pid );
	printk (KERN_INFO "ref=%d\n", module_refcount(THIS_MODULE));
	return SUCCESS;
}

static int char_dev_release(struct inode *inode,
		            struct file *file)
{
	return SUCCESS;
}

static ssize_t char_dev_read(struct file *file, 
		            char *buf,
			    size_t lbuf,
			    loff_t *ppos)
{
	int maxbytes; /* number of bytes from ppos to MAX_LENGTH */
	int bytes_to_do; /* number of bytes to read */
	int nbytes; /* number of bytes actually read */

	maxbytes = MAX_LENGTH;//- *ppos;
	
	if( maxbytes > lbuf ) bytes_to_do = lbuf;
	else bytes_to_do = maxbytes;
	
	if( bytes_to_do == 0 ) {
		printk("Reached end of device\n");
		return -ENOSPC; /* Causes read() to return EOF */
	}
	
	nbytes = bytes_to_do - 
		 raw_copy_to_user( buf, /* to */
			       char_device_buf ,//+ *ppos, /* from */
			       bytes_to_do ); /* how many bytes */

	*ppos += nbytes;
	return nbytes;	
}

static ssize_t char_dev_write(struct file *file,
		            const char *buf,
			    size_t lbuf,
			    loff_t *ppos)
{
	int nbytes; /* Number of bytes written */
	int bytes_to_do; /* Number of bytes to write */
	int maxbytes; /* Maximum number of bytes that can be written */

	maxbytes = MAX_LENGTH;//- *ppos;

	if( maxbytes > lbuf ) bytes_to_do = lbuf;
	else bytes_to_do = maxbytes;

	if( bytes_to_do == 0 ) {
		printk("Reached end of device\n");
		return -ENOSPC; /* Returns EOF at write() */
	}

	nbytes = bytes_to_do -
	         raw_copy_from_user( char_device_buf ,//+ *ppos, /* to */
				 buf, /* from */
				 bytes_to_do ); /* how many bytes */
	*ppos += nbytes;
	return nbytes;
}


static loff_t char_dev_lseek (struct file *file, loff_t offset, int orig)
{
    loff_t testpos;
    switch (orig) {

    case 0:                 /* SEEK_SET */
            testpos = offset;
            break;
    case 1:                 /* SEEK_CUR */
            testpos = file->f_pos + offset;
            break;
    case 2:                 /* SEEK_END */
           testpos = MAX_LENGTH + offset;
           break;
    default:
          return -EINVAL;
    }
   
    testpos = testpos < MAX_LENGTH ? testpos : MAX_LENGTH;
    testpos = testpos >= 0 ? testpos : 0;
    file->f_pos = testpos;
    printk (KERN_INFO "Seeking to pos=%ld\n", (long)testpos);
    return testpos;
}


static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.read = char_dev_read,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release,
	.llseek = char_dev_lseek
};

static __init int char_dev_init(void)
{
	int ret;

	if (alloc_chrdev_region (&mydev, 0, count, CHAR_DEV_NAME) < 0) {
            printk (KERN_ERR "failed to reserve major/minor range\n");
            return -1;
    }

        if (!(my_cdev = cdev_alloc ())) {
            printk (KERN_ERR "cdev_alloc() failed\n");
            unregister_chrdev_region (mydev, count);
            return -1;
 	}
	cdev_init(my_cdev,&char_dev_fops);

	ret=cdev_add(my_cdev,mydev,count);
	if( ret < 0 ) {
		printk(KERN_INFO "Error registering device driver\n");
	        cdev_del (my_cdev);
                unregister_chrdev_region (mydev, count); 	
		return -1;
	}

	my_class = class_create (THIS_MODULE, "VIRTUAL11111");
    device_create (my_class, NULL, mydev, NULL, "%s", "my_Char_driver");

	printk(KERN_INFO"\nDevice Registered: %s\n",CHAR_DEV_NAME);
	printk(KERN_INFO "Major number = %d, Minor number = %d\n", MAJOR (mydev),MINOR (mydev));

	char_device_buf =(char *)kmalloc(MAX_LENGTH,GFP_KERNEL);
	return 0;
}

static __exit void  char_dev_exit(void)
{
	 device_destroy (my_class, mydev);
     class_destroy (my_class);
	 cdev_del(my_cdev);
	 unregister_chrdev_region(mydev,1);
	 kfree(char_device_buf);
	 printk(KERN_INFO "\n Driver unregistered \n");
}
module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("akshay_hegde");



MODULE_DESCRIPTION("Character Device Driver");
MODULE_LICENSE("GPL");



/*akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit chr_drv_udev.c 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make
make -C /lib/modules/5.11.0-34-generic/build M=/home/akshay/linux_device_driver modules
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
  CC [M]  /home/akshay/linux_device_driver/chr_drv_udev.o
  MODPOST /home/akshay/linux_device_driver/Module.symvers
  CC [M]  /home/akshay/linux_device_driver/chr_drv_udev.mod.o
  LD [M]  /home/akshay/linux_device_driver/chr_drv_udev.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod chr_drv_udev.ko
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head
Module                  Size  Used by
chr_drv_udev           16384  0
rfcomm                 81920  4
ccm                    20480  6
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
bnep                   24576  2
nls_iso8859_1          16384  1
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo chmod 777 /dev/m
mapper/         mcelog          media0          mei0            mem             mqueue/         my_Char_driver
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo chmod 777 /dev/my_Char_driver 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ cat /proc/d
devices        diskstats      dma            driver/        dynamic_debug/ 
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
508 my_cdrv
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
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit testapp.c 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gcc testapp.c 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ ./a.out
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

Data = hello

****Please Enter the Option******
        1. Write               
        2. Read                 
		3. IOCTL				
        4. Exit                 
*********************************
4
Your Option = 4
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo rmmod chr_drv_udev 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make clean 
make -C /lib/modules/5.11.0-34-generic/build M=/home/akshay/linux_device_driver clean
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
  CLEAN   /home/akshay/linux_device_driver/Module.symvers
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ 

*/
