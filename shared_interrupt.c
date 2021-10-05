      
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 19
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param (irq, int, S_IRUGO);

static irqreturn_t akshay_hegde (int irq, void *dev_id)
{
    irq_counter++;
    printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
    return IRQ_NONE;            /* we return IRQ_NONE because we are just observing */
}

static int __init my_init (void)
{
    if (request_irq(irq, akshay_hegde, IRQF_SHARED, "akshay_hegde", &my_dev_id))
        return -1;
    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;
}

static void __exit my_exit (void)
{
    synchronize_irq (irq);
    free_irq (irq, &my_dev_id);
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("akshay");
MODULE_DESCRIPTION ("Interrupt Handler Test");
MODULE_LICENSE ("GPL");




/*


19:          0          0          0          0  IR-IO-APIC   19-fasteoi   my_interrupt, akshay_hegde


*/




/*akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit interrupt.c
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ make
make -C /lib/modules/5.11.0-37-generic/build  M=/home/akshay/linux_device_driver modules 
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-37-generic'
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-37-generic'
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ gedit shared_interrupt.c
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ sudo insmod shared_interrupt.ko
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ cat /proc/interrupts 
            CPU0       CPU1       CPU2       CPU3       
   0:          8          0          0          0  IR-IO-APIC    2-edge      timer
   1:         30          0         14          0  IR-IO-APIC    1-edge      i8042
   8:          0          0          0          1  IR-IO-APIC    8-edge      rtc0
   9:          8         13          0          0  IR-IO-APIC    9-fasteoi   acpi
  12:       1438        180          0          0  IR-IO-APIC   12-edge      i8042
  17:          2          0          0          0  IR-IO-APIC   17-fasteoi   i801_smbus
  
  
  
  
  
  
  
  
  
  19:          0          0          0          0  IR-IO-APIC   19-fasteoi   my_interrupt, akshay_hegde
  
  
  
  
  
  
  
  
  
  
 120:          0          0          0          0  DMAR-MSI    0-edge      dmar0
 121:          0          0          0          0  DMAR-MSI    1-edge      dmar1
 122:          0          0          0          0  IR-PCI-MSI 458752-edge      PCIe PME
 123:          0          0          0          0  IR-PCI-MSI 464896-edge      PCIe PME, aerdrv
 124:          0          0          0          0  IR-PCI-MSI 475136-edge      PCIe PME
 125:          0          0          0          0  IR-PCI-MSI 479232-edge      PCIe PME, aerdrv, pciehp
 126:          0          0          0          0  IR-PCI-MSI 481280-edge      PCIe PME, aerdrv
 127:     799423      12109          0          0  IR-PCI-MSI 376832-edge      ahci[0000:00:17.0]
 128:          0         33          0          0  IR-PCI-MSI 360448-edge      mei_me
 129:       9230      38906     340519      37463  IR-PCI-MSI 327680-edge      xhci_hcd
 130:          0          0          0          0  IR-PCI-MSI 3670016-edge      mmc0
 131:          0          0          0       2382  IR-PCI-MSI 514048-edge      snd_hda_intel:card0
 132:       1213          0     413890    2752407  IR-PCI-MSI 1572864-edge      iwlwifi
 133:       1896    7390144          0        621  IR-PCI-MSI 32768-edge      i915
 134:          0          8      12953          0  IR-PCI-MSI 520192-edge      enp0s31f6
 NMI:         47        255        253        254   Non-maskable interrupts
 LOC:   11172750   12918221   12257336   11087681   Local timer interrupts
 SPU:          0          0          0          0   Spurious interrupts
 PMI:         47        255        253        254   Performance monitoring interrupts
 IWI:     253391    3694361     251971     246050   IRQ work interrupts
 RTR:          0          0          0          0   APIC ICR read retries
 RES:    1187331    1808964    1710514    1149858   Rescheduling interrupts
 CAL:     708204     615321     661674     661137   Function call interrupts
 TLB:     654722     634587     637416     667607   TLB shootdowns
 TRM:          0          0          0          0   Thermal event interrupts
 THR:          0          0          0          0   Threshold APIC interrupts
 DFR:          0          0          0          0   Deferred Error APIC interrupts
 MCE:          0          0          0          0   Machine check exceptions
 MCP:        116        117        117        117   Machine check polls
 ERR:          0
 MIS:          0
 PIN:          0          0          0          0   Posted-interrupt notification event
 NPI:          0          0          0          0   Nested posted-interrupt event
 PIW:          0          0          0          0   Posted-interrupt wakeup event
akshay@akshay-ThinkPad-L570-W10DG:~/linux_device_driver$ 

*/


    

