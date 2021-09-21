#include <linux/module.h>
#include <linux/kernel.h>


#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
//#include <linux/sched.h>               //task_struct
 
atomic_t my_global_variable = ATOMIC_INIT(0);/*unsigned int 32 atomic variable global macro initialized*/


static struct task_struct *char_thread;/*thread creation structure*/
static struct task_struct *char_thread1;

int mydata =0;
int my_thread_fun(void *p);



int my_thread_fun(void *p)
{
	while(!kthread_should_stop())
	{		
		//Atomically add one to v
		atomic_inc(&my_global_variable); // single operation ; no much overhead 
		printk(KERN_INFO "my_thread_fun after inc  %lu\n", atomic_read(&my_global_variable));
		
			
		//atomic_read(&my_global_variable)
		atomic_add(14, &my_global_variable);
		printk(KERN_INFO "my_thread_fun after add   %lu\n", atomic_read(&my_global_variable));

		//Atomically subtract i from v
		atomic_sub(2,&my_global_variable);
		printk(KERN_INFO "my_thread_fun after sub   %lu\n", atomic_read(&my_global_variable));

		//Atomically subtract one from v
		atomic_dec(&my_global_variable);
		printk(KERN_INFO "my_thread_fun after dec   %lu\n", atomic_read(&my_global_variable));
		
		//Atomically set v equal to i
		atomic_set(&my_global_variable, 14);			
		printk(KERN_INFO "my_thread_fun after set    %lu\n", atomic_read(&my_global_variable));
		
		// Function Atomically subtract i from v and return true if the result is zero; otherwise false 
		int res = atomic_sub_and_test(3, &my_global_variable);
		printk(KERN_INFO "my_thread_fun after sub and test    %lu\n", res);

		// Function Atomically dec one from v and return true(1) if the result is zero; otherwise false (0)
		res = atomic_dec_and_test(&my_global_variable);
		printk(KERN_INFO "my_thread_fun after dec and test    %lu\n", res);
		printk(KERN_INFO "my_thread_fun after dec and test    %lu\n", atomic_read(&my_global_variable));
			
		//Atomically add i to v and return true if the result is negative; otherwise false
		res = atomic_add_negative(8,&my_global_variable);
		printk(KERN_INFO "my_thread_fun after add neg    %lu\n", res);
		printk(KERN_INFO "my_thread_fun after add neg     %lu\n", atomic_read(&my_global_variable));

		//Atomically increment v by one and return true if the result is zero; false otherwise
		res = atomic_inc_and_test(&my_global_variable);
		printk(KERN_INFO "my_thread_fun after inc and test     %lu\n", res);
		printk(KERN_INFO "my_thread_fun after inc and test      %lu\n", atomic_read(&my_global_variable));

		printk(KERN_INFO "*****************ATOMIC BIT WSIE OPERATORS *************");
		
		unsigned long word = 0;

		set_bit(0, &word);       /* bit zero is now set (atomically) */
		printk(KERN_INFO "SET BIT      %x\n", word);

		set_bit(1, &word);        /* bit one is now set (atomically) */
		printk(KERN_INFO "SET BIT      %x\n", word);

		set_bit(2, &word);        /* bit two is now set (atomically) */
		printk(KERN_INFO "SET BIT      %x\n", word);

		clear_bit(1, &word);     /* bit one is now unset (atomically) */
		printk(KERN_INFO "CLEAR BIT      %x\n", word);

		change_bit(0, &word);    /* bit zero is flipped; now it is unset (atomically) */
		printk(KERN_INFO "CHANGE BIT      %x\n", word);

		//Atomically set the nr-th bit starting from addr and return the previous value
		res = test_and_set_bit(1, &word);
		printk(KERN_INFO "TEST AND SET    %lu\n", res);
		printk(KERN_INFO "TEST AND SET      %x\n", word);

		//Atomically clear the nr-th bit starting from addr and return the previous value
		res = test_and_clear_bit(1,&word);
		printk(KERN_INFO "TEST AND CLEAR    %lu\n", res);
		printk(KERN_INFO "TEST AND CLEAR      %x\n", word);
		
		//Atomically return the value of the nr-th bit starting from addr
		res = test_bit(2,&word);
		printk(KERN_INFO "TEST BIT    %lu\n", res);  

		msleep(1000);
		
	}
	return 0;	
}

int my_thread_fun1(void *p)
{
	while(!kthread_should_stop())
	{
		
		atomic_inc(&my_global_variable);
		printk(KERN_INFO "my_thread_fun2  %lu\n", atomic_read(&my_global_variable));
		
		msleep(1000);
	}
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_INFO"My sample driver start");

	
	char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)
	{
		printk(KERN_INFO" create the thread");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
#if 0
	char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread1");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
#endif
	return 0;

}


static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My samle driver exit\n");

	kthread_stop(char_thread);
	//kthread_stop(char_thread1);

}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);
MODULE_LICENSE("GPL");/*authorization*/
MODULE_LICENSE("GPL v2");
MODULE_LICENSE("Dual BSD/GPL");

MODULE_AUTHOR("akshay_hegde");
MODULE_DESCRIPTION("LDD session 1");
MODULE_VERSION("1:0.0");
/*


akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make
make -C /lib/modules/5.11.0-34-generic/build  M=/home/akshay/linux_device_driver modules
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-34-generic'
.
.
.          **********WARNINGS**********
.
.
      |                            ^~~~~~~~~~~~
  MODPOST /home/akshay/linux_device_driver/Module.symvers
  CC [M]  /home/akshay/linux_device_driver/Atomic.mod.o
  LD [M]  /home/akshay/linux_device_driver/Atomic.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-34-generic'
.
.
.
.
.       
.
.
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod Atomic.ko
[sudo] password for akshay: 
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head
Module                  Size  Used by
Atomic                 16384  0
rfcomm                 81920  4
ccm                    20480  6
cmac                   16384  5
algif_hash             16384  2
algif_skcipher         16384  2
af_alg                 28672  10 algif_hash,algif_skcipher
bnep                   24576  2
snd_hda_codec_hdmi     61440  1
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ dmesg
[    0.000000] microcode: microcode updated early to revision 0xea, date = 2021-01-25
[    0.000000] Linux version 5.11.0-34-generic (buildd@lgw01-amd64-001) (gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0, GNU ld (GNU Binutils for Ubuntu) 2.34) #36~20.04.1-Ubuntu SMP Fri Aug 27 08:06:32 UTC 2021 (Ubuntu 5.11.0-34.36~20.04.1-generic 5.11.22)
[    0.000000] Command line: BOOT_IMAGE=/boot/vmlinuz-5.11.0-34-generic root=UUID=a70ba1c1-7862-4916-912e-3745fa4b4a3c ro quiet splash vt.handoff=7
.
.
.                             *********dmesg list************
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.

[ 3251.181703] Atomic: loading out-of-tree module taints kernel.
[ 3251.181753] Atomic: module verification failed: signature and/or required key missing - tainting kernel
[ 3251.182064] My sample driver start
[ 3251.182148]  create the thread
[ 3251.182151] my_thread_fun after inc  1
[ 3251.182154] my_thread_fun after add   15
[ 3251.182156] my_thread_fun after sub   13
[ 3251.182157] my_thread_fun after dec   12
[ 3251.182159] my_thread_fun after set    14
[ 3251.182160] my_thread_fun after sub and test    0
[ 3251.182162] my_thread_fun after dec and test    0
[ 3251.182163] my_thread_fun after dec and test    10
[ 3251.182165] my_thread_fun after add neg    0
[ 3251.182166] my_thread_fun after add neg     18
[ 3251.182168] my_thread_fun after inc and test     0
[ 3251.182169] my_thread_fun after inc and test      19
[ 3251.182170] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3251.182172] SET BIT      1
[ 3251.182174] SET BIT      3
[ 3251.182175] SET BIT      7
[ 3251.182176] CLEAR BIT      5
[ 3251.182178] CHANGE BIT      4
[ 3251.182179] TEST AND SET    0
[ 3251.182181] TEST AND SET      6
[ 3251.182182] TEST AND CLEAR    1
[ 3251.182184] TEST AND CLEAR      4
[ 3251.182185] TEST BIT    1
[ 3252.211758] my_thread_fun after inc  20
[ 3252.211778] my_thread_fun after add   34
[ 3252.211786] my_thread_fun after sub   32
[ 3252.211791] my_thread_fun after dec   31
[ 3252.211797] my_thread_fun after set    14
[ 3252.211802] my_thread_fun after sub and test    0
[ 3252.211807] my_thread_fun after dec and test    0
[ 3252.211812] my_thread_fun after dec and test    10
[ 3252.211817] my_thread_fun after add neg    0
[ 3252.211821] my_thread_fun after add neg     18
[ 3252.211825] my_thread_fun after inc and test     0
[ 3252.211829] my_thread_fun after inc and test      19
[ 3252.211834] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3252.211838] SET BIT      1
[ 3252.211844] SET BIT      3
[ 3252.211848] SET BIT      7
[ 3252.211852] CLEAR BIT      5
[ 3252.211856] CHANGE BIT      4
[ 3252.211860] TEST AND SET    0
[ 3252.211864] TEST AND SET      6
[ 3252.211868] TEST AND CLEAR    1
[ 3252.211872] TEST AND CLEAR      4
[ 3252.211877] TEST BIT    1
[ 3253.235616] my_thread_fun after inc  20
[ 3253.235630] my_thread_fun after add   34
[ 3253.235635] my_thread_fun after sub   32
[ 3253.235640] my_thread_fun after dec   31
[ 3253.235643] my_thread_fun after set    14
[ 3253.235648] my_thread_fun after sub and test    0
[ 3253.235652] my_thread_fun after dec and test    0
[ 3253.235657] my_thread_fun after dec and test    10
[ 3253.235661] my_thread_fun after add neg    0
[ 3253.235666] my_thread_fun after add neg     18
[ 3253.235670] my_thread_fun after inc and test     0
[ 3253.235674] my_thread_fun after inc and test      19
[ 3253.235679] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3253.235683] SET BIT      1
[ 3253.235688] SET BIT      3
[ 3253.235692] SET BIT      7
[ 3253.235696] CLEAR BIT      5
[ 3253.235700] CHANGE BIT      4
[ 3253.235704] TEST AND SET    0
[ 3253.235709] TEST AND SET      6
[ 3253.235713] TEST AND CLEAR    1
[ 3253.235717] TEST AND CLEAR      4
[ 3253.235721] TEST BIT    1
[ 3254.259683] my_thread_fun after inc  20
[ 3254.259697] my_thread_fun after add   34
[ 3254.259702] my_thread_fun after sub   32
[ 3254.259706] my_thread_fun after dec   31
[ 3254.259710] my_thread_fun after set    14
[ 3254.259714] my_thread_fun after sub and test    0
[ 3254.259719] my_thread_fun after dec and test    0
[ 3254.259723] my_thread_fun after dec and test    10
[ 3254.259728] my_thread_fun after add neg    0
[ 3254.259732] my_thread_fun after add neg     18
[ 3254.259736] my_thread_fun after inc and test     0
[ 3254.259740] my_thread_fun after inc and test      19
[ 3254.259745] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3254.259749] SET BIT      1
[ 3254.259755] SET BIT      3
[ 3254.259759] SET BIT      7
[ 3254.259763] CLEAR BIT      5
[ 3254.259767] CHANGE BIT      4
[ 3254.259772] TEST AND SET    0
[ 3254.259776] TEST AND SET      6
[ 3254.259780] TEST AND CLEAR    1
[ 3254.259784] TEST AND CLEAR      4
[ 3254.259788] TEST BIT    1
[ 3255.283666] my_thread_fun after inc  20
[ 3255.283681] my_thread_fun after add   34
[ 3255.283686] my_thread_fun after sub   32
[ 3255.283690] my_thread_fun after dec   31
[ 3255.283694] my_thread_fun after set    14
[ 3255.283698] my_thread_fun after sub and test    0
[ 3255.283703] my_thread_fun after dec and test    0
[ 3255.283707] my_thread_fun after dec and test    10
[ 3255.283711] my_thread_fun after add neg    0
[ 3255.283716] my_thread_fun after add neg     18
[ 3255.283720] my_thread_fun after inc and test     0
[ 3255.283725] my_thread_fun after inc and test      19
[ 3255.283729] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3255.283733] SET BIT      1
[ 3255.283739] SET BIT      3
[ 3255.283743] SET BIT      7
[ 3255.283747] CLEAR BIT      5
[ 3255.283751] CHANGE BIT      4
[ 3255.283755] TEST AND SET    0
[ 3255.283759] TEST AND SET      6
[ 3255.283763] TEST AND CLEAR    1
[ 3255.283767] TEST AND CLEAR      4
[ 3255.283771] TEST BIT    1
[ 3256.307817] my_thread_fun after inc  20
[ 3256.307832] my_thread_fun after add   34
[ 3256.307836] my_thread_fun after sub   32
[ 3256.307841] my_thread_fun after dec   31
[ 3256.307845] my_thread_fun after set    14
[ 3256.307849] my_thread_fun after sub and test    0
[ 3256.307854] my_thread_fun after dec and test    0
[ 3256.307858] my_thread_fun after dec and test    10
[ 3256.307862] my_thread_fun after add neg    0
[ 3256.307867] my_thread_fun after add neg     18
[ 3256.307871] my_thread_fun after inc and test     0
[ 3256.307875] my_thread_fun after inc and test      19
[ 3256.307880] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3256.307884] SET BIT      1
[ 3256.307890] SET BIT      3
[ 3256.307894] SET BIT      7
[ 3256.307898] CLEAR BIT      5
[ 3256.307902] CHANGE BIT      4
[ 3256.307906] TEST AND SET    0
[ 3256.307910] TEST AND SET      6
[ 3256.307914] TEST AND CLEAR    1
[ 3256.307918] TEST AND CLEAR      4
[ 3256.307922] TEST BIT    1
[ 3257.331821] my_thread_fun after inc  20
[ 3257.331836] my_thread_fun after add   34
[ 3257.331841] my_thread_fun after sub   32
[ 3257.331845] my_thread_fun after dec   31
[ 3257.331849] my_thread_fun after set    14
[ 3257.331853] my_thread_fun after sub and test    0
[ 3257.331858] my_thread_fun after dec and test    0
[ 3257.331862] my_thread_fun after dec and test    10
[ 3257.331866] my_thread_fun after add neg    0
[ 3257.331871] my_thread_fun after add neg     18
[ 3257.331875] my_thread_fun after inc and test     0
[ 3257.331879] my_thread_fun after inc and test      19
[ 3257.331884] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3257.331888] SET BIT      1
[ 3257.331894] SET BIT      3
[ 3257.331898] SET BIT      7
[ 3257.331902] CLEAR BIT      5
[ 3257.331906] CHANGE BIT      4
[ 3257.331910] TEST AND SET    0
[ 3257.331914] TEST AND SET      6
[ 3257.331919] TEST AND CLEAR    1
[ 3257.331923] TEST AND CLEAR      4
[ 3257.331927] TEST BIT    1
[ 3258.355855] my_thread_fun after inc  20
[ 3258.355869] my_thread_fun after add   34
[ 3258.355874] my_thread_fun after sub   32
[ 3258.355878] my_thread_fun after dec   31
[ 3258.355882] my_thread_fun after set    14
[ 3258.355886] my_thread_fun after sub and test    0
[ 3258.355891] my_thread_fun after dec and test    0
[ 3258.355895] my_thread_fun after dec and test    10
[ 3258.355899] my_thread_fun after add neg    0
[ 3258.355903] my_thread_fun after add neg     18
[ 3258.355908] my_thread_fun after inc and test     0
[ 3258.355912] my_thread_fun after inc and test      19
[ 3258.355916] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3258.355920] SET BIT      1
[ 3258.355926] SET BIT      3
[ 3258.355930] SET BIT      7
[ 3258.355934] CLEAR BIT      5
[ 3258.355938] CHANGE BIT      4
[ 3258.355942] TEST AND SET    0
[ 3258.355947] TEST AND SET      6
[ 3258.355951] TEST AND CLEAR    1
[ 3258.355955] TEST AND CLEAR      4
[ 3258.355959] TEST BIT    1
[ 3259.379865] my_thread_fun after inc  20
[ 3259.379880] my_thread_fun after add   34
[ 3259.379884] my_thread_fun after sub   32
[ 3259.379889] my_thread_fun after dec   31
[ 3259.379892] my_thread_fun after set    14
[ 3259.379897] my_thread_fun after sub and test    0
[ 3259.379901] my_thread_fun after dec and test    0
[ 3259.379905] my_thread_fun after dec and test    10
[ 3259.379910] my_thread_fun after add neg    0
[ 3259.379914] my_thread_fun after add neg     18
[ 3259.379919] my_thread_fun after inc and test     0
[ 3259.379923] my_thread_fun after inc and test      19
[ 3259.379927] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3259.379932] SET BIT      1
[ 3259.379937] SET BIT      3
[ 3259.379942] SET BIT      7
[ 3259.379945] CLEAR BIT      5
[ 3259.379950] CHANGE BIT      4
[ 3259.379954] TEST AND SET    0
[ 3259.379958] TEST AND SET      6
[ 3259.379962] TEST AND CLEAR    1
[ 3259.379966] TEST AND CLEAR      4
[ 3259.379970] TEST BIT    1
[ 3260.403865] my_thread_fun after inc  20
[ 3260.403880] my_thread_fun after add   34
[ 3260.403884] my_thread_fun after sub   32
[ 3260.403888] my_thread_fun after dec   31
[ 3260.403892] my_thread_fun after set    14
[ 3260.403896] my_thread_fun after sub and test    0
[ 3260.403901] my_thread_fun after dec and test    0
[ 3260.403905] my_thread_fun after dec and test    10
[ 3260.403909] my_thread_fun after add neg    0
[ 3260.403914] my_thread_fun after add neg     18
[ 3260.403918] my_thread_fun after inc and test     0
[ 3260.403922] my_thread_fun after inc and test      19
[ 3260.403927] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3260.403931] SET BIT      1
[ 3260.403937] SET BIT      3
[ 3260.403941] SET BIT      7
[ 3260.403945] CLEAR BIT      5
[ 3260.403949] CHANGE BIT      4
[ 3260.403953] TEST AND SET    0
[ 3260.403957] TEST AND SET      6
[ 3260.403961] TEST AND CLEAR    1
[ 3260.403965] TEST AND CLEAR      4
[ 3260.403969] TEST BIT    1
[ 3261.427728] my_thread_fun after inc  20
[ 3261.427747] my_thread_fun after add   34
[ 3261.427753] my_thread_fun after sub   32
[ 3261.427758] my_thread_fun after dec   31
[ 3261.427763] my_thread_fun after set    14
[ 3261.427768] my_thread_fun after sub and test    0
[ 3261.427774] my_thread_fun after dec and test    0
[ 3261.427779] my_thread_fun after dec and test    10
[ 3261.427785] my_thread_fun after add neg    0
[ 3261.427790] my_thread_fun after add neg     18
[ 3261.427796] my_thread_fun after inc and test     0
[ 3261.427801] my_thread_fun after inc and test      19
[ 3261.427807] *****************ATOMIC BIT WSIE OPERATORS *************
[ 3261.427813] SET BIT      1
[ 3261.427820] SET BIT      3
[ 3261.427826] SET BIT      7
[ 3261.427832] CLEAR BIT      5
[ 3261.427838] CHANGE BIT      4
[ 3261.427844] TEST AND SET    0
[ 3261.427851] TEST AND SET      6
[ 3261.427856] TEST AND CLEAR    1
[ 3261.427862] TEST AND CLEAR      4
[ 3261.427867] TEST BIT    1
*/


