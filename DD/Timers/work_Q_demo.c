#include<linux/module.h>
#include<linux/workqueue.h>
#include<linux/timer.h>


typedef struct work_q_demo
{
	u8 nbWorkCount;
	struct timer_list m_work_sechduler;
	struct workqueue_struct* m_workQ;
}WorkQDemo;


static WorkQDemo Q1;
struct work_struct w1 , w2;

static void Work_1(struct work_struct *work)
{
	pr_notice(" \t\t\t\t\t Work_1 Done !! \n");
}

static void Work_2(struct work_struct *work)
{
	pr_notice(" \t\t\t\t\t Work_2 Done !! \n");
}


static void timer_expire_cb(struct timer_list* pTmr )
{
	pr_notice("timer_expire_cb Called \n");
	WorkQDemo* d  = container_of( pTmr , struct work_q_demo , m_work_sechduler );	

	if( d->nbWorkCount != 0 )
	{	
		d->nbWorkCount--;

		//Create work and add to work Q
		INIT_WORK(&w1,&Work_1);
		INIT_WORK(&w2,&Work_2);

		queue_work(d->m_workQ, &w1);
		queue_work(d->m_workQ, &w2);
		//queue_delayed_work(d->m_workQ, &w2 , jiffies + 3*HZ);

		pTmr->expires = jiffies + 5*HZ;
		pr_notice("Timer added again !! \n");
		add_timer(pTmr);
	}
}


static void Create_work_Q(void)
{
	Q1.nbWorkCount = 10;

	//set timer expiry
	Q1.m_work_sechduler.expires = jiffies + 5*HZ;
	timer_setup(&Q1.m_work_sechduler,timer_expire_cb,0);

	//create work Q 
	Q1.m_workQ = create_workqueue("JanpriyaWorkQ");

	//start timer
	add_timer(&Q1.m_work_sechduler);
}


static int __init work_q_demo_init(void)
{
	pr_notice("WorkQDemo: Start \n");
	Create_work_Q();
	return 0;
}


static void __exit work_q_demo_exit(void)
{
	pr_notice("WorkQDemo: exit \n");
	del_timer(&Q1.m_work_sechduler);
	flush_workqueue(Q1.m_workQ);
	destroy_workqueue(Q1.m_workQ);
}

module_init(work_q_demo_init)
module_exit(work_q_demo_exit)
MODULE_LICENSE("GPL");
