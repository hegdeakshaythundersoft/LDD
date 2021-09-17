#include<linux/init.h>/*kernal headers for init,exit*/
#include<linux/module.h>
MODULE_LICENSE("GPL");/*authorization*/
MODULE_LICENSE("GPL v2");
MODULE_LICENSE("Dual BSD/GPL");

MODULE_AUTHOR("akshay_hegde");
MODULE_DESCRIPTION("LDD session 1");
MODULE_VERSION("1:0.0");


static int __init sample_module_init(void){/*__init macro makes the space for the base kernal to load*/
	printk(KERN_INFO"sample module initiated");
	/*constructor, as we have main function called at first in user space this function is called first since initiation static int is preferred for return value*/


	return 0;
}
static void __exit sample_module_exit(void){/*__exit macro frees the space for the base kernal to load*/

	printk(KERN_INFO"sample module exit");
	return;
}


module_init(sample_module_init);
module_exit(sample_module_exit);
/*akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ git pull origin master
From https://github.com/hegdeakshaythundersoft/LDD
 * branch            master     -> FETCH_HEAD
Already up to date.
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit sample_driver.c 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit Makefile 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make
make -C /lib/modules/5.11.0-34-generic/build M=/home/akshay/linux_device_driver modules
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
  CC [M]  /home/akshay/linux_device_driver/sample_driver.o
  MODPOST /home/akshay/linux_device_driver/Module.symvers
  CC [M]  /home/akshay/linux_device_driver/sample_driver.mod.o
  LD [M]  /home/akshay/linux_device_driver/sample_driver.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod sample_driver.ko
[sudo] password for akshay: 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head
Module                  Size  Used by
sample_driver          16384  0
major_minoor_dev       20480  0
major_minor            16384  0
rfcomm                 81920  4
ccm                    20480  6
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo rmmod sample_driver.ko
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head
Module                  Size  Used by
major_minoor_dev       20480  0
major_minor            16384  0
rfcomm                 81920  4
ccm                    20480  6
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
bnep                   24576  2
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make clean
make -C /lib/modules/5.11.0-34-generic/build M=/home/akshay/linux_device_driver clean
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
  CLEAN   /home/akshay/linux_device_driver/Module.symvers
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
*/
