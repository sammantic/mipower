/*******************************************************************
 * mipower -- program to print current status of power supply.     *   
 *                                                                 *   
 * Author:  Mohamed Samman                                         *   
 *                                                                 *   
 * Purpose: Tracking laptop's adapter cable,                       *
 * 	    if it connected or not.			           *   
 *                                                                 *   
 * Usage:                                                          *   
 *      insert kernel module and the message appears in kernel log.*   
 *******************************************************************/ 


#include <linux/init.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/power_supply.h>

void ac_message(void) {
	char *name = "AC";
	char *msg = "Power up";
	struct power_supply *myps;
	union  power_supply_propval status; 
	int ret;

	myps = power_supply_get_by_name(name);
	ret = myps->get_property(myps,POWER_SUPPLY_PROP_ONLINE,&status);

	if (status.intval == 0) 
		msg = "Power down";

	pr_info("[ mipower ]Power supplay name : %s\n",myps->name);
	pr_info("[ mipower ]Status : %s \n", msg);

}


static int my_notifier_call(struct notifier_block *b, unsigned long event, void *data)
{
	pr_info("[ mipower ]RECEIVING MSG...\n");
		
	switch (event) {
	case PSY_EVENT_PROP_CHANGED:
		pr_info("[ mipower ] Power supply changed...\n");
		ac_message();				
		break;
	default:
		pr_info("[ mipower ] Unknow change...\n");
		break;
	}
	pr_info("\n");
	return NOTIFY_OK;
}

static struct notifier_block my_block = {
	.notifier_call = my_notifier_call,
	.priority      = 0,
};

static int __init my_init(void)
{
	power_supply_reg_notifier(&my_block);	
	pr_info("[ mipower ] AC plug tracker module loaded...\n");
	return 0;
}

static void __exit my_exit(void)
{
	power_supply_unreg_notifier(&my_block);
	pr_info("[ mipower ] AC plug tracker module unloaded... \n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("MSamman");
MODULE_DESCRIPTION("Tracking Ac power supply");
MODULE_LICENSE("GPL v2");
