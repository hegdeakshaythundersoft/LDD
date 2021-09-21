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

	char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread1");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}

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



************************For 2 threads*******************************




[  352.674604] SET BIT      1
[  352.674605] SET BIT      3
[  352.674606] SET BIT      7
[  352.674606] CLEAR BIT      5
[  352.674607] CHANGE BIT      4
[  352.674608] TEST AND SET    0
[  352.674609] TEST AND SET      6
[  352.674609] TEST AND CLEAR    1
[  352.674610] TEST AND CLEAR      4
[  352.674611] TEST BIT    1
[  353.688335] my_thread_fun after inc  20
[  353.688337] my_thread_fun after inc  21
[  353.688350] my_thread_fun after add   35
[  353.688351] my_thread_fun after add   49
[  353.688357] my_thread_fun after sub   47
[  353.688360] my_thread_fun after sub   45
[  353.688363] my_thread_fun after dec   44
[  353.688369] my_thread_fun after dec   43
[  353.688370] my_thread_fun after set    14
[  353.688375] my_thread_fun after set    14
[  353.688377] my_thread_fun after sub and test    0
[  353.688383] my_thread_fun after sub and test    0
[  353.688385] my_thread_fun after dec and test    0
[  353.688389] my_thread_fun after dec and test    7
[  353.688391] my_thread_fun after dec and test    0
[  353.688397] my_thread_fun after add neg    0
[  353.688398] my_thread_fun after dec and test    14
[  353.688404] my_thread_fun after add neg     14
[  353.688406] my_thread_fun after add neg    0
[  353.688411] my_thread_fun after add neg     22
[  353.688412] my_thread_fun after inc and test     0
[  353.688416] my_thread_fun after inc and test      23
[  353.688418] my_thread_fun after inc and test     0
[  353.688422] my_thread_fun after inc and test      24
[  353.688424] *****************ATOMIC BIT WSIE OPERATORS *************
[  353.688429] SET BIT      1
[  353.688430] *****************ATOMIC BIT WSIE OPERATORS *************
[  353.688435] SET BIT      1
[  353.688437] SET BIT      3
[  353.688442] SET BIT      7
[  353.688445] SET BIT      3
[  353.688449] CLEAR BIT      5
[  353.688452] SET BIT      7
[  353.688457] CHANGE BIT      4
[  353.688459] CLEAR BIT      5
[  353.688464] TEST AND SET    0
[  353.688467] CHANGE BIT      4
[  353.688472] TEST AND SET    0
[  353.688473] TEST AND SET      6
[  353.688477] TEST AND CLEAR    1
[  353.688479] TEST AND SET      6
[  353.688484] TEST AND CLEAR      4
[  353.688486] TEST AND CLEAR    1
[  353.688491] TEST AND CLEAR      4
[  353.688492] TEST BIT    1
[  353.688498] TEST BIT    1
[  354.712311] my_thread_fun after inc  25
[  354.712311] my_thread_fun after inc  26
[  354.712314] my_thread_fun after add   40
[  354.712315] my_thread_fun after add   54
[  354.712316] my_thread_fun after sub   52
[  354.712317] my_thread_fun after sub   50
[  354.712317] my_thread_fun after dec   49
[  354.712318] my_thread_fun after set    14
[  354.712319] my_thread_fun after dec   13
[  354.712319] my_thread_fun after sub and test    0
[  354.712321] my_thread_fun after set    14
[  354.712321] my_thread_fun after dec and test    0
[  354.712322] my_thread_fun after dec and test    13
[  354.712323] my_thread_fun after sub and test    0
[  354.712323] my_thread_fun after add neg    0
[  354.712324] my_thread_fun after add neg     18
[  354.712325] my_thread_fun after dec and test    0
[  354.712326] my_thread_fun after inc and test     0
[  354.712327] my_thread_fun after inc and test      18
[  354.712327] my_thread_fun after dec and test    18
[  354.712328] *****************ATOMIC BIT WSIE OPERATORS *************
[  354.712329] my_thread_fun after add neg    0
[  354.712330] SET BIT      1
[  354.712331] my_thread_fun after add neg     26
[  354.712331] SET BIT      3
[  354.712332] SET BIT      7
[  354.712332] my_thread_fun after inc and test     0
[  354.712333] CLEAR BIT      5
[  354.712334] my_thread_fun after inc and test      27
[  354.712334] CHANGE BIT      4
[  354.712335] TEST AND SET    0
[  354.712335] *****************ATOMIC BIT WSIE OPERATORS *************
[  354.712337] TEST AND SET      6
[  354.712336] SET BIT      1
[  354.712338] SET BIT      3
[  354.712338] TEST AND CLEAR    1
[  354.712339] TEST AND CLEAR      4
[  354.712339] SET BIT      7
[  354.712340] CLEAR BIT      5
[  354.712340] TEST BIT    1
[  354.712341] CHANGE BIT      4
[  354.712342] TEST AND SET    0
[  354.712343] TEST AND SET      6
[  354.712344] TEST AND CLEAR    1
[  354.712345] TEST AND CLEAR      4
[  354.712345] TEST BIT    1
[  355.736408] my_thread_fun after inc  28
[  355.736408] my_thread_fun after inc  29
[  355.736425] my_thread_fun after add   43
[  355.736431] my_thread_fun after add   57
[  355.736436] my_thread_fun after sub   55
[  355.736443] my_thread_fun after sub   53
[  355.736446] my_thread_fun after dec   52
[  355.736451] my_thread_fun after dec   51
[  355.736455] my_thread_fun after set    14
[  355.736460] my_thread_fun after set    14
[  355.736465] my_thread_fun after sub and test    0
[  355.736469] my_thread_fun after sub and test    0
[  355.736474] my_thread_fun after dec and test    0
[  355.736479] my_thread_fun after dec and test    0
[  355.736484] my_thread_fun after dec and test    6
[  355.736485] my_thread_fun after dec and test    6
[  355.736491] my_thread_fun after add neg    0
[  355.736492] my_thread_fun after add neg    0
[  355.736497] my_thread_fun after add neg     22
[  355.736498] my_thread_fun after add neg     22
[  355.736504] my_thread_fun after inc and test     0
[  355.736504] my_thread_fun after inc and test     0
[  355.736510] my_thread_fun after inc and test      24
[  355.736511] my_thread_fun after inc and test      24
[  355.736516] *****************ATOMIC BIT WSIE OPERATORS *************
[  355.736517] *****************ATOMIC BIT WSIE OPERATORS *************
[  355.736522] SET BIT      1
[  355.736523] SET BIT      1
[  355.736529] SET BIT      3
[  355.736532] SET BIT      3
[  355.736538] SET BIT      7
[  355.736540] SET BIT      7
[  355.736545] CLEAR BIT      5
[  355.736546] CLEAR BIT      5
[  355.736551] CHANGE BIT      4
[  355.736552] CHANGE BIT      4
[  355.736558] TEST AND SET    0
[  355.736559] TEST AND SET    0
[  355.736564] TEST AND SET      6
[  355.736565] TEST AND SET      6
[  355.736570] TEST AND CLEAR    1
[  355.736571] TEST AND CLEAR    1
[  355.736576] TEST AND CLEAR      4
[  355.736577] TEST AND CLEAR      4
[  355.736582] TEST BIT    1
[  355.736583] TEST BIT    1
[  356.760309] my_thread_fun after inc  25
[  356.760324] my_thread_fun after add   39
[  356.760329] my_thread_fun after sub   37
[  356.760333] my_thread_fun after dec   36
[  356.760337] my_thread_fun after set    14
[  356.760341] my_thread_fun after sub and test    0
[  356.760345] my_thread_fun after dec and test    0
[  356.760349] my_thread_fun after dec and test    10
[  356.760354] my_thread_fun after add neg    0
[  356.760358] my_thread_fun after add neg     18
[  356.760362] my_thread_fun after inc and test     0
[  356.760367] my_thread_fun after inc and test      19
[  356.760371] *****************ATOMIC BIT WSIE OPERATORS *************
[  356.760375] SET BIT      1
[  356.760381] SET BIT      3
[  356.760386] SET BIT      7
[  356.760389] CLEAR BIT      5
[  356.760393] CHANGE BIT      4
[  356.760397] TEST AND SET    0
[  356.760401] TEST AND SET      6
[  356.760406] TEST AND CLEAR    1
[  356.760410] TEST AND CLEAR      4
[  356.760414] TEST BIT    1
[  356.764423] my_thread_fun after inc  20
[  356.764442] my_thread_fun after add   34
[  356.764448] my_thread_fun after sub   32
[  356.764453] my_thread_fun after dec   31
[  356.764457] my_thread_fun after set    14
[  356.764463] my_thread_fun after sub and test    0
[  356.764480] my_thread_fun after dec and test    0
[  356.764485] my_thread_fun after dec and test    10
[  356.764490] my_thread_fun after add neg    0
[  356.764503] my_thread_fun after add neg     18
[  356.764516] my_thread_fun after inc and test     0
[  356.764529] my_thread_fun after inc and test      19
[  356.764555] *****************ATOMIC BIT WSIE OPERATORS *************
[  356.764568] SET BIT      1
[  356.764581] SET BIT      3
[  356.764593] SET BIT      7
[  356.764604] CLEAR BIT      5
[  356.764616] CHANGE BIT      4
[  356.764629] TEST AND SET    0
[  356.764641] TEST AND SET      6
[  356.764654] TEST AND CLEAR    1
[  356.764668] TEST AND CLEAR      4
[  356.764681] TEST BIT    1
[  357.784448] my_thread_fun after inc  20
[  357.784451] my_thread_fun after inc  21
[  357.784461] my_thread_fun after add   35
[  357.784463] my_thread_fun after add   49
[  357.784468] my_thread_fun after sub   47
[  357.784472] my_thread_fun after sub   45
[  357.784477] my_thread_fun after dec   44
[  357.784478] my_thread_fun after dec   43
[  357.784483] my_thread_fun after set    14
[  357.784485] my_thread_fun after set    14
[  357.784490] my_thread_fun after sub and test    0
[  357.784491] my_thread_fun after sub and test    0
[  357.784497] my_thread_fun after dec and test    0
[  357.784499] my_thread_fun after dec and test    0
[  357.784505] my_thread_fun after dec and test    6
[  357.784506] my_thread_fun after dec and test    6
[  357.784511] my_thread_fun after add neg    0
[  357.784512] my_thread_fun after add neg    0
[  357.784517] my_thread_fun after add neg     22
[  357.784519] my_thread_fun after add neg     22
[  357.784524] my_thread_fun after inc and test     0
[  357.784527] my_thread_fun after inc and test     0
[  357.784532] my_thread_fun after inc and test      24
[  357.784534] my_thread_fun after inc and test      24
[  357.784540] *****************ATOMIC BIT WSIE OPERATORS *************
[  357.784542] *****************ATOMIC BIT WSIE OPERATORS *************
[  357.784547] SET BIT      1
[  357.784549] SET BIT      1
[  357.784554] SET BIT      3
[  357.784555] SET BIT      3
[  357.784560] SET BIT      7
[  357.784562] SET BIT      7
[  357.784567] CLEAR BIT      5
[  357.784568] CLEAR BIT      5
[  357.784572] CHANGE BIT      4
[  357.784574] CHANGE BIT      4
[  357.784579] TEST AND SET    0
[  357.784582] TEST AND SET    0
[  357.784587] TEST AND SET      6
[  357.784589] TEST AND SET      6
[  357.784593] TEST AND CLEAR    1
[  357.784595] TEST AND CLEAR    1
[  357.784600] TEST AND CLEAR      4
[  357.784602] TEST AND CLEAR      4
[  357.784607] TEST BIT    1
[  357.784609] TEST BIT    1
[  358.808474] my_thread_fun after inc  25
[  358.808482] my_thread_fun after inc  26
[  358.808491] my_thread_fun after add   40
[  358.808497] my_thread_fun after sub   38
[  358.808502] my_thread_fun after add   52
[  358.808503] my_thread_fun after dec   51
[  358.808508] my_thread_fun after set    14
[  358.808511] my_thread_fun after sub   12
[  358.808515] my_thread_fun after sub and test    0
[  358.808519] my_thread_fun after dec   8
[  358.808524] my_thread_fun after dec and test    0
[  358.808525] my_thread_fun after set    14
[  358.808531] my_thread_fun after sub and test    0
[  358.808532] my_thread_fun after dec and test    11
[  358.808537] my_thread_fun after add neg    0
[  358.808539] my_thread_fun after dec and test    0
[  358.808544] my_thread_fun after add neg     18
[  358.808546] my_thread_fun after dec and test    18
[  358.808551] my_thread_fun after inc and test     0
[  358.808553] my_thread_fun after add neg    0
[  358.808558] my_thread_fun after add neg     27
[  358.808559] my_thread_fun after inc and test      27
[  358.808565] *****************ATOMIC BIT WSIE OPERATORS *************
[  358.808567] my_thread_fun after inc and test     0
[  358.808573] SET BIT      1
[  358.808574] my_thread_fun after inc and test      28
[  358.808579] *****************ATOMIC BIT WSIE OPERATORS *************
[  358.808582] SET BIT      3
[  358.808587] SET BIT      7
[  358.808588] SET BIT      1
[  358.808593] CLEAR BIT      5
[  358.808598] SET BIT      3
[  358.808601] CHANGE BIT      4
[  358.808605] SET BIT      7
[  358.808608] TEST AND SET    0
[  358.808614] CLEAR BIT      5
[  358.808615] TEST AND SET      6
[  358.808619] TEST AND CLEAR    1
[  358.808621] CHANGE BIT      4
[  358.808626] TEST AND CLEAR      4
[  358.808628] TEST AND SET    0
[  358.808633] TEST BIT    1
[  358.808636] TEST AND SET      6
[  358.808642] TEST AND CLEAR    1
[  358.808647] TEST AND CLEAR      4
[  358.808651] TEST BIT    1
[  359.832507] my_thread_fun after inc  29
[  359.832508] my_thread_fun after inc  30
[  359.832521] my_thread_fun after add   44
[  359.832524] my_thread_fun after add   58
[  359.832529] my_thread_fun after sub   56
[  359.832534] my_thread_fun after sub   54
[  359.832536] my_thread_fun after dec   53
[  359.832540] my_thread_fun after set    14
[  359.832546] my_thread_fun after dec   13
[  359.832547] my_thread_fun after sub and test    0
[  359.832552] my_thread_fun after dec and test    0
[  359.832557] my_thread_fun after set    14
[  359.832559] my_thread_fun after dec and test    14
[  359.832564] my_thread_fun after add neg    0
[  359.832568] my_thread_fun after sub and test    0
[  359.832570] my_thread_fun after add neg     19
[  359.832576] my_thread_fun after inc and test     0
[  359.832578] my_thread_fun after dec and test    0
[  359.832583] my_thread_fun after dec and test    19
[  359.832584] my_thread_fun after inc and test      19
[  359.832590] my_thread_fun after add neg    0
[  359.832591] *****************ATOMIC BIT WSIE OPERATORS *************
[  359.832597] my_thread_fun after add neg     27
[  359.832596] SET BIT      1
[  359.832603] my_thread_fun after inc and test     0
[  359.832607] SET BIT      3
[  359.832611] my_thread_fun after inc and test      28
[  359.832614] SET BIT      7
[  359.832620] *****************ATOMIC BIT WSIE OPERATORS *************
[  359.832622] CLEAR BIT      5
[  359.832627] SET BIT      1
[  359.832629] CHANGE BIT      4
[  359.832634] TEST AND SET    0
[  359.832635] SET BIT      3
[  359.832641] TEST AND SET      6
[  359.832643] SET BIT      7
[  359.832647] TEST AND CLEAR    1
[  359.832651] CLEAR BIT      5
[  359.832656] CHANGE BIT      4
[  359.832657] TEST AND CLEAR      4
[  359.832662] TEST BIT    1
[  359.832663] TEST AND SET    0
[  359.832668] TEST AND SET      6
[  359.832673] TEST AND CLEAR    1
[  359.832677] TEST AND CLEAR      4
[  359.832681] TEST BIT    1
[  360.856529] my_thread_fun after inc  29
[  360.856529] my_thread_fun after inc  30
[  360.856541] my_thread_fun after add   44
[  360.856548] my_thread_fun after add   58
[  360.856551] my_thread_fun after sub   56
[  360.856556] my_thread_fun after sub   54
[  360.856558] my_thread_fun after dec   53
[  360.856563] my_thread_fun after dec   52
[  360.856564] my_thread_fun after set    14
[  360.856570] my_thread_fun after set    14
[  360.856572] my_thread_fun after sub and test    0
[  360.856577] my_thread_fun after sub and test    0
[  360.856581] my_thread_fun after dec and test    0
[  360.856586] my_thread_fun after dec and test    0
[  360.856587] my_thread_fun after dec and test    6
[  360.856593] my_thread_fun after dec and test    6
[  360.856596] my_thread_fun after add neg    0
[  360.856601] my_thread_fun after add neg    0
[  360.856603] my_thread_fun after add neg     22
[  360.856608] my_thread_fun after inc and test     0
[  360.856610] my_thread_fun after add neg     23
[  360.856615] my_thread_fun after inc and test     0
[  360.856616] my_thread_fun after inc and test      24
[  360.856621] *****************ATOMIC BIT WSIE OPERATORS *************
[  360.856624] my_thread_fun after inc and test      24
[  360.856629] SET BIT      1
[  360.856631] *****************ATOMIC BIT WSIE OPERATORS *************
[  360.856636] SET BIT      1
[  360.856637] SET BIT      3
[  360.856642] SET BIT      3
[  360.856644] SET BIT      7
[  360.856650] SET BIT      7
[  360.856651] CLEAR BIT      5
[  360.856655] CHANGE BIT      4
[  360.856656] CLEAR BIT      5
[  360.856661] CHANGE BIT      4
[  360.856662] TEST AND SET    0
[  360.856668] TEST AND SET    0
[  360.856671] TEST AND SET      6
[  360.856676] TEST AND SET      6
[  360.856678] TEST AND CLEAR    1
[  360.856683] TEST AND CLEAR    1
[  360.856685] TEST AND CLEAR      4
[  360.856690] TEST AND CLEAR      4
[  360.856692] TEST BIT    1
[  360.856697] TEST BIT    1









	*************two threads using different thread fun****************************




[ 1103.531939] My sample driver start
[ 1103.531985]  create the thread
[ 1103.531987] my_thread_fun after inc  1
[ 1103.531989] my_thread_fun after add   15
[ 1103.531990] my_thread_fun after sub   13
[ 1103.531991] my_thread_fun after dec   12
[ 1103.531993] my_thread_fun after set    14
[ 1103.531995] my_thread_fun after sub and test    0
[ 1103.531997] my_thread_fun after dec and test    0
[ 1103.531998] my_thread_fun after dec and test    10
[ 1103.531999] my_thread_fun after add neg    0
[ 1103.532000] my_thread_fun after add neg     18
[ 1103.532001] my_thread_fun after inc and test     0
[ 1103.532002] my_thread_fun after inc and test      19
[ 1103.532004] *****************ATOMIC BIT WSIE OPERATORS *************
[ 1103.532004] SET BIT      1
[ 1103.532006] SET BIT      3
[ 1103.532007] SET BIT      7
[ 1103.532008] CLEAR BIT      5
[ 1103.532009] CHANGE BIT      4
[ 1103.532010] TEST AND SET    0
[ 1103.532012] TEST AND SET      6
[ 1103.532013] TEST AND CLEAR    1
[ 1103.532015] TEST AND CLEAR      4
[ 1103.532016]  create the thread1
[ 1103.532017] TEST BIT    1
[ 1103.532017] my_thread_fun2  20
[ 1104.567257] my_thread_fun2  21
[ 1104.567257] my_thread_fun after inc  22
[ 1104.567272] my_thread_fun after add   36
[ 1104.567291] my_thread_fun after sub   34
[ 1104.567298] my_thread_fun after dec   33
[ 1104.567304] my_thread_fun after set    14
[ 1104.567311] my_thread_fun after sub and test    0
[ 1104.567318] my_thread_fun after dec and test    0
[ 1104.567325] my_thread_fun after dec and test    10
[ 1104.567332] my_thread_fun after add neg    0
[ 1104.567338] my_thread_fun after add neg     18
[ 1104.567345] my_thread_fun after inc and test     0
[ 1104.567352] my_thread_fun after inc and test      19
[ 1104.567359] *****************ATOMIC BIT WSIE OPERATORS *************
[ 1104.567363] SET BIT      1
[ 1104.567374] SET BIT      3
[ 1104.567380] SET BIT      7
[ 1104.567386] CLEAR BIT      5
[ 1104.567392] CHANGE BIT      4
[ 1104.567398] TEST AND SET    0
[ 1104.567405] TEST AND SET      6
[ 1104.567411] TEST AND CLEAR    1
[ 1104.567417] TEST AND CLEAR      4
[ 1104.567424] TEST BIT    1
[ 1105.591372] my_thread_fun2  20
[ 1105.591372] my_thread_fun after inc  21
[ 1105.591387] my_thread_fun after add   35
[ 1105.591392] my_thread_fun after sub   33
[ 1105.591396] my_thread_fun after dec   32
[ 1105.591401] my_thread_fun after set    14
[ 1105.591405] my_thread_fun after sub and test    0
[ 1105.591411] my_thread_fun after dec and test    0
[ 1105.591415] my_thread_fun after dec and test    10
[ 1105.591430] my_thread_fun after add neg    0
[ 1105.591439] my_thread_fun after add neg     18
[ 1105.591446] my_thread_fun after inc and test     0
[ 1105.591453] my_thread_fun after inc and test      19
[ 1105.591460] *****************ATOMIC BIT WSIE OPERATORS *************
[ 1105.591465] SET BIT      1
[ 1105.591475] SET BIT      3
[ 1105.591481] SET BIT      7
[ 1105.591487] CLEAR BIT      5
[ 1105.591493] CHANGE BIT      4
[ 1105.591500] TEST AND SET    0
[ 1105.591506] TEST AND SET      6
[ 1105.591512] TEST AND CLEAR    1
[ 1105.591518] TEST AND CLEAR      4
[ 1105.591525] TEST BIT    1










*/


