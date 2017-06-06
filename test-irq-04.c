/// \file test-irq-04.c
///
/// \brief Exemples de l'article "[KERNEL] Interruptions et tasklets" (https://www.blaess.fr/christophe/2017/06/06)
///
/// \author Christophe Blaess 2017 (https://www.blaess.fr/christophe)
///
/// \license GPL.

	#include <linux/atomic.h>
	#include <linux/delay.h>
	#include <linux/gpio.h>
	#include <linux/interrupt.h>
	#include <linux/module.h>


	#define IRQ_TEST_GPIO_IN  23

	static void irq_test_tasklet_function(unsigned long);

	static DECLARE_TASKLET(irq_test_tasklet, irq_test_tasklet_function, 0);

	static atomic_t irq_test_counter = ATOMIC_INIT(0);


static irqreturn_t irq_test_handler(int irq, void * ident)
{
	printk(KERN_INFO "%s: %s()\n", THIS_MODULE->name, __FUNCTION__);
	atomic_inc(&irq_test_counter);
	tasklet_schedule(&irq_test_tasklet);
	return IRQ_HANDLED;
}


static void irq_test_tasklet_function(unsigned long unused)
{
	printk(KERN_INFO "%s: %s()\n", THIS_MODULE->name, __FUNCTION__);
	do {
		printk(KERN_INFO "%s:%s() loop\n", THIS_MODULE->name, __FUNCTION__);
		udelay(1000);
	} while (atomic_dec_return(&irq_test_counter) > 0);
}


static int __init irq_test_init (void)
{
	int err;

	if ((err = gpio_request(IRQ_TEST_GPIO_IN,THIS_MODULE->name)) != 0)
		return err;

	if ((err = gpio_direction_input(IRQ_TEST_GPIO_IN)) != 0) {
		gpio_free(IRQ_TEST_GPIO_IN);
		return err;
	}

	if ((err = request_irq(gpio_to_irq(IRQ_TEST_GPIO_IN), irq_test_handler,
	                       IRQF_SHARED | IRQF_TRIGGER_RISING,
	                       THIS_MODULE->name, THIS_MODULE->name)) != 0) {
		gpio_free(IRQ_TEST_GPIO_IN);
		return err;
	}

	return 0;
}


static void __exit irq_test_exit (void)
{
	free_irq(gpio_to_irq(IRQ_TEST_GPIO_IN), THIS_MODULE->name);
	tasklet_kill(&irq_test_tasklet);
	gpio_free(IRQ_TEST_GPIO_IN);
}


module_init(irq_test_init);
module_exit(irq_test_exit);


MODULE_DESCRIPTION("Simple monolithic interrupt handler");
MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
MODULE_LICENSE("GPL");
