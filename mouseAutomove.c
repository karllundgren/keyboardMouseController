#include <linux/module.h>

#include <linux/init.h>
#include <linux/tty.h>		/* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>		/* For KDSETLED */
#include <linux/vt.h>
#include <linux/vt_kern.h>
#include <linux/console_struct.h>	/* For vc_cons */

#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include<linux/sched.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/slab.h>

#include <linux/keyboard.h>

//keyboard & mouse
struct notifier_block tom;
struct input_dev *jerry;
//the amount mouse moves when a key is pressed
int n = 4;

//get keystroke
int kb_notifier_fn(struct notifier_block *tom, unsigned long action, void* data){
	
	struct keyboard_notifier_param *kp = (struct keyboard_notifier_param*)data;	
	int value = kp->value;
	

	//up arrow key	
	if (value == 103)
	{	
		printk("value: %d\n", value);
		input_report_rel(jerry, REL_Y, -n);
	}
	//left arrow key
	else if (value == 105)
	{	
		printk("value: %d\n", value);
		input_report_rel(jerry, REL_X, -n);
	}
	//down arrow key
	else if (value == 108)
	{	
		
		printk("value: %d\n", value);
		input_report_rel(jerry, REL_Y, n);
	}
	//right arrow key
	else if (value == 106)
	{	
		printk("value: %d\n", value);
		input_report_rel(jerry, REL_X, n);
	}
	//Pause button(causes left click)
	else if (value == 119)
	{	
		
		printk("value: %d\n", value);
		printk("value: %d\n", kp->down);
		input_report_key(jerry, BTN_LEFT, kp->down);
	}
	input_sync(jerry);
	return 0;
}


//init
static int __init mm_init(void)
{
	//keyboard
	tom.notifier_call = kb_notifier_fn;
	register_keyboard_notifier(&tom);

	//mouse
	jerry = input_allocate_device();
	
	jerry->name = "jerry";
	set_bit(EV_REL, jerry->evbit);
	set_bit(REL_X, jerry->relbit);
	set_bit(REL_Y, jerry->relbit);

	set_bit(EV_KEY, jerry->evbit);
	set_bit(BTN_LEFT, jerry->keybit);
	
	input_register_device(jerry);
	return 0;
}


//exit
static void __exit mm_remove(void)
{
	//get rid of keyboard
	unregister_keyboard_notifier(&tom);
	input_unregister_device(jerry);
}


MODULE_LICENSE("GPL"); 
module_init(mm_init);
module_exit(mm_remove);

