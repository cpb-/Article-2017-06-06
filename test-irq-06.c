/// \file test-irq-06.c
///
/// \brief Exemples de l'article "[KERNEL] Interruptions et tasklets" (https://www.blaess.fr/christophe/2017/06/06)
///
/// \author Christophe Blaess 2017 (https://www.blaess.fr/christophe)
///
/// \license GPL.

	#include <linux/delay.h>
	#include <linux/fs.h>
	#include <linux/interrupt.h>
	#include <linux/miscdevice.h>
	#include <linux/module.h>
	#include <linux/sched.h>


	static void irq_test_tasklet_function(unsigned long);

	static DECLARE_TASKLET(irq_test_tasklet, irq_test_tasklet_function, 0);


static void irq_test_tasklet_function(unsigned long unused)
{
	printk(KERN_INFO "%s: current pid=%d comm=%s\n", THIS_MODULE->name, current->pid, current->comm);
}


static ssize_t irq_test_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	tasklet_schedule(&irq_test_tasklet);
	return length;
}


static struct file_operations irq_test_fops = {
	.owner = THIS_MODULE,
	.write = irq_test_write,
};

static struct miscdevice irq_test_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = THIS_MODULE->name,
	.fops  = &irq_test_fops,
};


static int __init irq_test_init (void)
{
	return misc_register(&irq_test_misc);
}


static void __exit irq_test_exit (void)
{
	misc_deregister(&irq_test_misc);
}


module_init(irq_test_init);
module_exit(irq_test_exit);


MODULE_DESCRIPTION("Simple monolithic interrupt handler");
MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
MODULE_LICENSE("GPL");
