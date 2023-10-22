#include<linux/module.h>
#include<linux/interrupt.h>


typedef struct tasklet_demo
{
	u8  nbCount;
	u8  nbTimerStarted;
	wait_queue_head_t waitEvent;
	struct tasklet_struct m_tasklet;
}TaskletDemo;

static TaskletDemo demo_1; // use old methods 
static TaskletDemo demo_2; // use new methods 

static void tasklet_cb_1(unsigned long data)
{
	TaskletDemo* d = (TaskletDemo*)(data);
	pr_notice("\t\t\ttasklet_cb_1-->Enter \n");
	
	pr_notice("\t\t\t tasklet_cb_1-->in_intr: %ld in_atomic: %d  pid:%d name:%s  \n",
		in_interrupt() , in_atomic() , current->pid , current->comm);

	if( d->nbCount != 0 )
	{
		//Restart timer again!!
		d->nbCount--;
		tasklet_schedule(&d->m_tasklet);
	}
	else
	{
		pr_notice("\t\t\tasklet_cb_1: All Tasklet Cycle Over !!! \n");
	}

	if(!d->nbTimerStarted)
	{
		d->nbTimerStarted = true;
		pr_notice("\t\t\tasklet_cb_1: Wakup Start process !!! \n");
		wake_up_interruptible(&d->waitEvent);
	}
	pr_notice("\t\t\t tasklet_cb_1-->Exit \n");
}

static void tasklet_cb_2(struct tasklet_struct* tasklet )
{
	pr_notice("\t\t\t tasklet_cb_2-->Enter \n");
	TaskletDemo* d = container_of(tasklet,struct tasklet_demo,m_tasklet);
	

	pr_notice("\t\t\t tasklet_cb_2-->in_intr: %ld in_atomic: %d  pid:%d name:%s  \n",
		in_interrupt() , in_atomic() , current->pid , current->comm);

	if( d->nbCount != 0 )
	{
		//Restart timer again!!
		d->nbCount--;
		tasklet_hi_schedule(tasklet);
	}
	else
	{
		pr_notice("\t\t\tasklet_cb_2: All Tasklet Cycle Over !!! \n");
	}

	if(!d->nbTimerStarted)
	{
		d->nbTimerStarted = true;
		pr_notice("\t\t\tasklet_cb_2: Wakup Start process !!! \n");
		wake_up_interruptible(&d->waitEvent);
	}
	pr_notice("\t\t\t tasklet_cb_2-->Exit \n");
}


static void Start_Tasklet(void)
{
	pr_notice("\t\t Start_Tasklet-->ENTER \n");
	//Setup Tasklet for Demo1/demo_2
	demo_1.nbCount = 5;
	demo_2.nbCount = 5;
	demo_1.nbTimerStarted = 0;
	demo_2.nbTimerStarted = 0;
	init_waitqueue_head(&demo_1.waitEvent);
	init_waitqueue_head(&demo_2.waitEvent);

	tasklet_init(&demo_1.m_tasklet,tasklet_cb_1,(unsigned long)&demo_1);
	tasklet_setup(&demo_2.m_tasklet,tasklet_cb_2);

	tasklet_schedule(&demo_1.m_tasklet);
	tasklet_hi_schedule(&demo_2.m_tasklet);

	wait_event_interruptible(demo_1.waitEvent,demo_1.nbTimerStarted != 0 );	
	wait_event_interruptible(demo_2.waitEvent,demo_2.nbTimerStarted != 0 );	
	pr_notice("\t\t Start_Tasklet-->EXIT \n");
}

static int __init tasklet_demo_init(void)
{
	pr_notice("taskletDemo: Start \n");
	Start_Tasklet();
	return 0;
}


static void __exit tasklet_demo_exit(void)
{
	pr_notice("taskletDemo: exit \n");
}

module_init(tasklet_demo_init)
module_exit(tasklet_demo_exit)
MODULE_LICENSE("GPL");
