#include <linux/module.h>/*module init and exit*/
#include <linux/kernel.h>/*Flags KERN_...*/
#include <linux/kthread.h> /*all the kthread functions*/
#include <linux/delay.h>/*sleep*/ 
#include <linux/sched.h>/*task_struct*/
#include <linux/spinlock.h> /*for spin*/

MODULE_LICENSE("GPL");/*authorization*/
MODULE_AUTHOR("Akshay");
MODULE_DESCRIPTION(" spin");
MODULE_VERSION("1.0.0");
//static atomic_t rds_ib_allocation = ATOMIC_INIT(0);
static struct task_struct *char_thread1;/*passing the structs to create the threads 1 & 2*/
static struct task_struct *char_thread2;
//static task_t *char_thread1;
//static task_t *char_thread2;
int mydata =0;
int my_thread_fun1(void *p);/*operating on the global variable inside the thread functions*/
int my_thread_fun2(void *p);
spinlock_t my_spin;


int my_thread_fun1(void *p)
{	
	while(!kthread_should_stop())
	{		
		spin_lock(&my_spin);/*locking mechanisms*/
		mydata++;
		printk(KERN_SOH "my_thread_fun1 spinlocked 		%d\n",mydata);
		printk(KERN_SOH "  ");
		spin_unlock(&my_spin);
		printk(KERN_SOH "my_thread_fun1 spin unlocked 	%d\n",mydata);
		msleep(1000);
	}
	
	return 0;	
}

int my_thread_fun2(void *p)
{	
	while(!kthread_should_stop())
	{
		spin_lock(&my_spin);
		mydata++;
		printk(KERN_SOH "my_thread_fun2 spinlocked 	        %d\n", mydata);
		printk(KERN_SOH "  ");
		spin_unlock(&my_spin);
		printk(KERN_SOH "my_thread_fun2 spin unlocked       	%d\n", mydata);
		msleep(1000);
	}
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_SOH"My sample driver start");


	char_thread1= kthread_run(my_thread_fun1,NULL,"my char thread");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread1");
		
	}
	char_thread2 = kthread_run(my_thread_fun2,NULL,"my char thread1");
	if(char_thread2)
	{
		printk(KERN_INFO" create the thread2");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread2");
		
	}
	return 0;

}


static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My sample driver exit\n");

	kthread_stop(char_thread1);
	kthread_stop(char_thread2);

}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);
/*

akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make
make -C /lib/modules/5.11.0-37-generic/build  M=/home/akshay/linux_device_driver modules 
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-37-generic'
  CC [M]  /home/akshay/linux_device_driver/spin_lock.o
  MODPOST /home/akshay/linux_device_driver/Module.symvers
  CC [M]  /home/akshay/linux_device_driver/spin_lock.mod.o
  LD [M]  /home/akshay/linux_device_driver/spin_lock.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-37-generic'
[2]+  Done                    gedit spin_lock.c
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod spin_lock.ko
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head -n2
Module                  Size  Used by
spin_lock              16384  0
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ dmesg | tail -n350




[17448.217166] 

[17626.524344] My sample driver start
[17626.524412]  create the thread1
[17626.524416] my_thread_fun1 spinlocked 		1
[17626.524420]   
[17626.524422] my_thread_fun1 spin unlocked 	1
[17626.524447]  create the thread2
[17626.524450] my_thread_fun2 spinlocked 	        2
[17626.524452]   
[17626.524453] my_thread_fun2 spin unlocked       	2
[17627.546999] my_thread_fun2 spinlocked 	        3
[17627.547019]   
[17627.547024] my_thread_fun2 spin unlocked       	3
[17627.547047] my_thread_fun1 spinlocked 		4
[17627.547054]   
[17627.547058] my_thread_fun1 spin unlocked 	4
[17628.570967] my_thread_fun2 spinlocked 	        5
[17628.570982]   
[17628.570986] my_thread_fun2 spin unlocked       	5
[17628.570987] my_thread_fun1 spinlocked 		6
[17628.571001]   
[17628.571005] my_thread_fun1 spin unlocked 	6
[17629.594736] my_thread_fun1 spinlocked 		7
[17629.594752]   
[17629.594756] my_thread_fun1 spin unlocked 	7
[17629.598768] my_thread_fun2 spinlocked 	        8
[17629.598783]   
[17629.598789] my_thread_fun2 spin unlocked       	8
[17630.618755] my_thread_fun1 spinlocked 		9
[17630.618772]   
[17630.618777] my_thread_fun1 spin unlocked 	9
[17630.618777] my_thread_fun2 spinlocked 	        10
[17630.618789]   
[17630.618794] my_thread_fun2 spin unlocked       	10
[17631.642816] my_thread_fun2 spinlocked 	        11
[17631.642831]   
[17631.642835] my_thread_fun2 spin unlocked       	11
[17631.646862] my_thread_fun1 spinlocked 		12
[17631.646882]   
[17631.646887] my_thread_fun1 spin unlocked 	12
[17632.666769] my_thread_fun1 spinlocked 		13
[17632.666785]   
[17632.666789] my_thread_fun1 spin unlocked 	13
[17632.666808] my_thread_fun2 spinlocked 	        14
[17632.666813]   
[17632.666816] my_thread_fun2 spin unlocked       	14
[17633.690591] my_thread_fun2 spinlocked 	        15
[17633.690607]   
[17633.690611] my_thread_fun2 spin unlocked       	15
[17633.690629] my_thread_fun1 spinlocked 		16
[17633.690635]   
[17633.690638] my_thread_fun1 spin unlocked 	16
[17634.714683] my_thread_fun1 spinlocked 		17
[17634.714699]   
[17634.714704] my_thread_fun1 spin unlocked 	17
[17634.714723] my_thread_fun2 spinlocked 	        18
[17634.714728]   
[17634.714731] my_thread_fun2 spin unlocked       	18
[17635.742427] my_thread_fun2 spinlocked 	        19
[17635.742448]   
[17635.742453] my_thread_fun2 spin unlocked       	19
[17635.742454] my_thread_fun1 spinlocked 		20
[17635.742475]   
[17635.742480] my_thread_fun1 spin unlocked 	20
[17636.762560] my_thread_fun1 spinlocked 		21
[17636.762576]   
[17636.762580] my_thread_fun1 spin unlocked 	21
[17636.762582] my_thread_fun2 spinlocked 	        22
[17636.762596]   
[17636.762600] my_thread_fun2 spin unlocked       	22
[17637.786526] my_thread_fun2 spinlocked 	        23
[17637.786543]   
[17637.786546] my_thread_fun2 spin unlocked       	23
[17637.786658] my_thread_fun1 spinlocked 		24
[17637.786673]   
[17637.786677] my_thread_fun1 spin unlocked 	24
[17638.810448] my_thread_fun1 spinlocked 		25
[17638.810464]   
[17638.810468] my_thread_fun1 spin unlocked 	25
[17638.810469] my_thread_fun2 spinlocked 	        26
[17638.810488]   
[17638.810493] my_thread_fun2 spin unlocked       	26
[17639.834395] my_thread_fun1 spinlocked 		27
[17639.834410]   
[17639.834415] my_thread_fun1 spin unlocked 	27
[17639.838411] my_thread_fun2 spinlocked 	        28
[17639.838442]   
[17639.838461] my_thread_fun2 spin unlocked       	28
[17640.858178] my_thread_fun2 spinlocked 	        29
[17640.858200]   
[17640.858207] my_thread_fun2 spin unlocked       	29
[17640.858211] my_thread_fun1 spinlocked 		30
[17640.858232]   
[17640.858237] my_thread_fun1 spin unlocked 	30
[17641.882202] my_thread_fun1 spinlocked 		31
[17641.882219]   
[17641.882223] my_thread_fun1 spin unlocked 	31
[17641.886246] my_thread_fun2 spinlocked 	        32
[17641.886266]   
[17641.886271] my_thread_fun2 spin unlocked       	32
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ 


*/





