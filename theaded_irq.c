#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>


/*
IRQ 0 — system timer
IRQ 1 — keyboard controller
IRQ 2 — cascade (interrupt from slave controller)
IRQ 3 — serial port COM2
IRQ 4 — serial port COM1
IRQ 5 — parallel port 2 and 3 or sound card
IRQ 6 — floppy controller
IRQ 7 — parallel port 1
IRQ 8 — RTC timer
IRQ 9 — ACPI
IRQ 10 — open/SCSI/NIC
IRQ 11 — open/SCSI/NIC
IRQ 12 — mouse controller
IRQ 13 — math co-processor
IRQ 14 — ATA channel 1
IRQ 15 — ATA channel 2
*/

#define SHARED_IRQ 1
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param (irq, int, S_IRUGO);

static irqreturn_t my_interrupt (int irq, void *dev_id)
{
    irq_counter++;
    printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
    return IRQ_WAKE_THREAD ;            
}


static irqreturn_t  my_interrupt_thread_fn(int irq, void *dev_id)
{
	
	printk(KERN_INFO "my_interrupt_thread_fn  function called after ISR to handle the interrupt processing.\n"); 
	return IRQ_NONE;
}

static int __init my_init (void)
{
	request_threaded_irq (irq,my_interrupt,my_interrupt_thread_fn,IRQF_SHARED,"my_interrupt", &my_dev_id);

    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;
}

static void __exit my_exit (void)
{
    //wait for pending IRQ handlers (on other CPUs) 
	synchronize_irq (irq);

	// free an interrupt allocated with request_irq 
    free_irq (irq, &my_dev_id);
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("johny");
MODULE_DESCRIPTION ("Interrupt Handler Test");
MODULE_LICENSE ("GPL");
