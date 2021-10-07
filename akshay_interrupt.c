      
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include<linux/spinlock.h>

#define SHARED_IRQ 19
/*static DEFINE_SPINLOCK(xxx_lock);
unsigned long flags;
*/
int assign_1(void);
int assign_2(void);
static int irq = SHARED_IRQ, my_dev_id1,my_dev_id2, irq_counter = 0;
int int_flag1,int_flag2;
module_param (irq, int, S_IRUGO);
spinlock_t lock;

static irqreturn_t akshay_hegde (int irq, void *dev_id)
{
    irq_counter++;
    printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
    return IRQ_NONE;            /* we return IRQ_NONE because we are just observing */
}

static int __init my_init (void)
{
   
   	if(assign_1()<0)
   	return -1;
   	else if(assign_2()<0)
   	return -1;
       
    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;

	/*static DEFINE_SPINLOCK(xxx_lock);

	unsigned long flags;

	spin_lock_irqsave(&xxx_lock, flags);
	... critical section here ..
	spin_unlock_irqrestore(&xxx_lock, flags);*/


}

int assign_1(void){


	spin_trylock(&lock);
	if(spin_is_locked(&lock))
	printk(KERN_SOH"TRY LOCK ACQUIRED");
	
	
	
	printk(KERN_SOH"Inside Critical Section ONE");
	
	
	
		
	if(request_irq(irq, akshay_hegde, IRQF_SHARED, "akshay_hegde", &my_dev_id1)){
	spin_unlock(&lock);
	return -1;
	}
	else{
	spin_unlock_irq(&lock);
	printk(KERN_SOH"Outside Critical Section ONE");
	return 0;
	}

}

int assign_2(void){


	spin_lock_bh(&lock);
	if(spin_is_locked(&lock))
	printk(KERN_SOH"BH LOCK ACQUIRED");
	printk(KERN_SOH"2 Inside Critical Section TWO");		
	if(request_irq(irq,akshay_hegde, IRQF_SHARED, "hegde_akshay", &my_dev_id2)){
	spin_unlock_bh(&lock);
	return -1;
	}
	
	else{
	spin_unlock_bh(&lock);
	printk(KERN_SOH"Outside Critical Section TWO");
	return 0;
	}
   	


}

static void __exit my_exit (void)
{
    synchronize_irq (irq);
    free_irq (irq, &my_dev_id1);
    free_irq (irq, &my_dev_id2);
    printk (KERN_SOH "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("johny");
MODULE_DESCRIPTION ("Interrupt Handler Test");
MODULE_LICENSE ("GPL");







/***


akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make
make -C /lib/modules/5.11.0-37-generic/build  M=/home/akshay/linux_device_driver modules 
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-37-generic'
  CC [M]  /home/akshay/linux_device_driver/akshay_interrupt.o
  MODPOST /home/akshay/linux_device_driver/Module.symvers
  CC [M]  /home/akshay/linux_device_driver/akshay_interrupt.mod.o
  LD [M]  /home/akshay/linux_device_driver/akshay_interrupt.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-37-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod akshay_interrupt.ko
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ lsmod | head 
Module                  Size  Used by
akshay_interrupt       16384  0




akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ cat /proc/interrupts  | head 
            CPU0       CPU1       CPU2       CPU3      
   0:          8          0          0          0  IR-IO-APIC    2-edge      timer
   1:          0          0         11          0  IR-IO-APIC    1-edge      i8042
   8:          0          0          0          1  IR-IO-APIC    8-edge      rtc0
   9:          0         12          0          0  IR-IO-APIC    9-fasteoi   acpi
  12:          0        423          0          0  IR-IO-APIC   12-edge      i8042
  17:          2          0          0          0  IR-IO-APIC   17-fasteoi   i801_smbus
  19:          0          0          0          0  IR-IO-APIC   19-fasteoi   akshay_hegde, hegde_akshay
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ dmesg | tail
[ 3704.231041] blk_update_request: I/O error, dev sdb, sector 956001648 op 0x0:(READ) flags 0x80700 phys_seg 1 prio class 0
[ 4113.927554] perf: interrupt took too long (3143 > 3136), lowering kernel.perf_event_max_sample_rate to 63500
[ 5424.115118] Successfully unloading 
[ 5439.147535] TRY LOCK ACQUIRED
[ 5439.147538] Inside Critical Section ONE
[ 5439.147554] Outside Critical Section ONE
[ 5439.147555] BH LOCK ACQUIRED
[ 5439.147555]  Inside Critical Section TWO
[ 5439.147558] Outside Critical Section TWO
[ 5439.147560] Successfully loading ISR handler
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ 




****/





    

