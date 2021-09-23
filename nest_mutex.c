#include <linux/module.h>/*module init and exit*/
#include <linux/kernel.h>/*Flags KERN_...*/
#include <linux/kthread.h> /*all the kthread functions*/
#include <linux/delay.h>/*sleep*/ 
#include <linux/sched.h>/*task_struct    893 struct pid			*thread_pid;    */
#include <linux/mutex.h> /*for mutex*/

MODULE_LICENSE("GPL");/*authorization*/
MODULE_AUTHOR("Akshay");
MODULE_DESCRIPTION(" mutex");
MODULE_VERSION("1.0.0");
//static atomic_t rds_ib_allocation = ATOMIC_INIT(0);
static struct task_struct *char_thread1;/*passing the structs to create the threads 1 & 2*/
static struct task_struct *char_thread2;
//static task_t *char_thread1;
//static task_t *char_thread2;
int mydata =0;
int my_thread_fun1(void *p);/*operating on the global variable inside the thread functions*/
int my_thread_fun2(void *p);
struct mutex my_mutex;


int my_thread_fun1(void *p)
{	//mutex_lock(&my_mutex);
	while(!kthread_should_stop())
	{		
		mutex_lock(&my_mutex);/*locking mechanisms*/
		mydata++;
		printk(KERN_SOH "my_thread_fun1  %p->%d\n",&mydata, mydata);
		printk("%ld flag1",char_thread1->thread_info.flags); 
		printk("%d pid",char_thread1->thread_pid); 
		printk("%d t_pid",char_thread1->pid); 
		mutex_unlock(&my_mutex);
		msleep(1000);
		
	}
	//mutex_unlock(&my_mutex);
	return 0;	
}

int my_thread_fun2(void *p)
{	//mutex_lock(&my_mutex);
	while(!kthread_should_stop())
	{
		mutex_lock(&my_mutex);
		mydata++;
		printk(KERN_SOH "my_thread_fun2  %p->%d\n",&mydata, mydata);
		printk("%ld flag2",char_thread2->thread_info.flags); 
		printk("%d pid",char_thread2->thread_pid);
		printk("%d t_pid",char_thread2->pid);
		mutex_unlock(&my_mutex);
		msleep(1000);
	}
	//mutex_unlock(&my_mutex);
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_SOH"My sample driver start");

	mutex_init(&my_mutex); // Mutex init 

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


		**********************tried to print thread ID,flag and address of shared variable*****************
	
			************** t_id thread id,pid process id *******************************
	
	
[10466.575001] my_thread_fun1  0000000096324a59->shared var address
[10466.575021] 16384 flag1 flag from nested struct 
[10466.575028] 142007168 pid
[10466.575033] 23816 t_pid
[10466.575040] my_thread_fun2  0000000096324a59->shared var address
[10466.575059] 16384 flag2
[10466.575065] 142005632 pid
[10466.575070] 23817 t_pid
	
	******************************************************************************************************************	
		
[10465.563509] My sample driver start
[10465.563687]  create the thread1
[10465.563716] my_thread_fun1  0000000096324a59->1
[10465.563720] 16384 flag1
[10465.563722]  create the thread2
[10465.563722] 142007168 pid
[10465.563723] 23816 t_pid
[10465.563725] my_thread_fun2  0000000096324a59->2
[10465.563727] 16384 flag2
[10465.563728] 142005632 pid
[10465.563729] 23817 t_pid
[10466.575001] my_thread_fun1  0000000096324a59->3
[10466.575021] 16384 flag1
[10466.575028] 142007168 pid
[10466.575033] 23816 t_pid
[10466.575040] my_thread_fun2  0000000096324a59->4
[10466.575059] 16384 flag2
[10466.575065] 142005632 pid
[10466.575070] 23817 t_pid
[10467.598887] my_thread_fun2  0000000096324a59->5
[10467.598906] 16384 flag2
[10467.598912] 142005632 pid
[10467.598918] 23817 t_pid
[10467.598925] my_thread_fun1  0000000096324a59->6
[10467.598950] 16384 flag1
[10467.598957] 142007168 pid
























 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit mutex.c
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make




*******************warnings********************************



akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod mutex.ko
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head
Module                  Size  Used by
mutex                  16384  0
rfcomm                 81920  4
ccm                    20480  6
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
bnep                   24576  2
snd_hda_codec_hdmi     61440  1
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ 






*********************without mutex*******************************





[ 1625.089762] My sample driver start
[ 1625.089808]  create the thread
[ 1625.089810] my_thread_fun  0
[ 1625.089846]  create the thread1
[ 1625.089847] my_thread_fun2  0
[ 1626.098355] my_thread_fun2  0
[ 1626.098357] my_thread_fun  0
[ 1627.122135] my_thread_fun2  0
[ 1627.122135] my_thread_fun  0
[ 1628.145898] my_thread_fun2  0
[ 1628.145952] my_thread_fun  0
[ 1629.169661] my_thread_fun  0
[ 1629.169661] my_thread_fun2  0
[ 1630.193459] my_thread_fun  0
[ 1630.193461] my_thread_fun2  0
[ 1631.217216] my_thread_fun2  0
[ 1631.217228] my_thread_fun  0
[ 1632.241016] my_thread_fun  0
[ 1632.241023] my_thread_fun2  0
[ 1633.264855] my_thread_fun  0
[ 1633.264855] my_thread_fun2  0
[ 1634.288609] my_thread_fun  0
[ 1634.288609] my_thread_fun2  0
[ 1635.312391] my_thread_fun  0
[ 1635.312400] my_thread_fun2  0
[ 1636.336189] my_thread_fun  0
[ 1636.336190] my_thread_fun2  0
[ 1637.360013] my_thread_fun  0
[ 1637.360015] my_thread_fun2  0
[ 1638.383812] my_thread_fun  0
[ 1638.383812] my_thread_fun2  0
[ 1639.407590] my_thread_fun  0
[ 1639.407592] my_thread_fun2  0
[ 1640.431394] my_thread_fun  0
[ 1640.431395] my_thread_fun2  0
[ 1641.455199] my_thread_fun  0
[ 1641.455203] my_thread_fun2  0
[ 1642.479008] my_thread_fun  0
[ 1642.479009] my_thread_fun2  0
[ 1643.502801] my_thread_fun  0
[ 1643.502802] my_thread_fun2  0
[ 1644.526638] my_thread_fun  0
[ 1644.526640] my_thread_fun2  0
[ 1645.550414] my_thread_fun  0
[ 1645.550422] my_thread_fun2  0
[ 1646.574238] my_thread_fun  0
[ 1646.574242] my_thread_fun2  0
[ 1647.598042] my_thread_fun  0
[ 1647.598042] my_thread_fun2  0
[ 1648.621858] my_thread_fun  0
[ 1648.621866] my_thread_fun2  0
[ 1649.645691] my_thread_fun  0
[ 1649.645692] my_thread_fun2  0
[ 1650.669520] my_thread_fun  0
[ 1650.669523] my_thread_fun2  0
[ 1651.693341] my_thread_fun  0
[ 1651.693342] my_thread_fun2  0
[ 1652.717133] my_thread_fun  0
[ 1652.717156] my_thread_fun2  0
[ 1653.741016] my_thread_fun  0
[ 1653.741014] my_thread_fun2  0
[ 1654.764789] my_thread_fun  0
[ 1654.764789] my_thread_fun2  0
[ 1655.788411] my_thread_fun2  0
[ 1655.788420] my_thread_fun  0
[ 1656.812174] my_thread_fun  0
[ 1656.812176] my_thread_fun2  0
My samle driver exit






****** with mutex********








[12145.625345] My sample driver start
[12145.625398]  create the thread
[12145.625401] my_thread_fun  1
[12145.625435]  create the thread1
[12145.625438] my_thread_fun2  2
[12146.643095] my_thread_fun2  3
[12146.643124] my_thread_fun  4
[12147.667124] my_thread_fun2  5
[12147.667144] my_thread_fun  6
[12148.691144] my_thread_fun2  7
[12148.691164] my_thread_fun  8
[12149.715164] my_thread_fun  9
[12149.715184] my_thread_fun2  10
[12150.739188] my_thread_fun2  11
[12150.739249] my_thread_fun  12
[12151.763196] my_thread_fun2  13
[12151.763215] my_thread_fun  14
[12152.787199] my_thread_fun  15
[12152.787219] my_thread_fun2  16
[12153.811219] my_thread_fun  17
[12153.811238] my_thread_fun2  18
[12154.835046] my_thread_fun2  19
[12154.835055] my_thread_fun  20

*/





