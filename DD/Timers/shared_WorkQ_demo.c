#include<linux/module.h>




//static void Create


static int __init shared_work_q_init(void)
{
	pr_notice("SharedWorkQDemo: Start \n");
	return 0;
}


static void __exit shared_work_q_demo_exit(void)
{
	pr_notice("SharedWorkQDemo: exit \n");
}

module_init(shared_work_q_init)
module_exit(shared_work_q_exit)
MODULE_LICENSE("GPL");
