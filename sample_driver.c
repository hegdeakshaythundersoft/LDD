#include<linux/init.h>/*kernal headers for init,exit*/
#include<linux/module.h>
MODULE_LICENSE("GPL");/*authorization*/
MODULE_LICENSE("GPL v2");
MODULE_LICENSE("Dual BSD/GPL");

MODULE_AUTHOR("akshay_hegde");
MODULE_DESCRIPTION("LDD session 1");
//MODULE_VERSION("1:0.0");


static int __init sample_module_init(void){/*__init macro makes the space for the base kernal to load*/
	printk(KERN_INFO"sample module initiated");
	/*constructor, as we have main function called at first in user space this function is called first since initiation static int is preferred for return value*/


	return 0;
}
static void __exit sample_module_exit(void){/*__exit macro makes the space for the base kernal to load*/

	printk(KERN_INFO"sample module exit");
	return;
}


module_init(sample_module_init);
module_exit(sample_module_exit);
