#include<linux/module.h>

#include<linux/timer.h>	// for TIMER APIS
#include<linux/sched.h>
#include<linux/kernel.h>

typedef struct timer_demo
{
	u8  nbCount;
	u8  nbTimeoutInSec;
	u8  nbTimerStarted;
	wait_queue_head_t waitEvent;
	struct timer_list m_timer;
}TimerDemo;


static TimerDemo demo;

static void timer_expire_cb( struct timer_list* pTimer )
{
	pr_notice("\t\t\ttimer_expire_cb: Enter \n");

	//get demo pointer using container_of
	TimerDemo* d = container_of(pTimer,struct timer_demo,m_timer);

	pr_notice("\t\t\tin_intr: %ld in_atomic: %d  pid:%d name:%s  \n",
		in_interrupt() , in_atomic() , current->pid , current->comm);
	
	//Why I moved down this to test mod_timer appi	
	// if(!d->nbTimerStarted)
	// {
	// 	d->nbTimerStarted = true;
	// 	pr_notice("timer_expire_cb: Wakup Start process !!! \n");
	// 	wake_up_interruptible(&d->waitEvent);
	// }

	if( d->nbCount != 0 )
	{
		//Restart timer again!!
		pTimer->expires = jiffies + d->nbTimeoutInSec*HZ;
		d->nbCount--;
		add_timer(pTimer);
	}
	else
	{
		pr_notice("\t\t\ttimer_expire_cb: All Timer Cycle Over !!! \n");
	}

	if(!d->nbTimerStarted)
	{
		d->nbTimerStarted = true;
		pr_notice("timer_expire_cb: Wakup Start process !!! \n");
		wake_up_interruptible(&d->waitEvent);
	}
	
	pr_notice("\t\t\ttimer_expire_cb: Exit \n");
}


//That will start your timer
static void Start_timer_demo(void)
{
	pr_notice("\t\tStart_timer_demo: Enter \n");
	//start timer for evry 5 second expiry
	demo.nbCount = 5;
	demo.nbTimeoutInSec = 5;
	demo.nbTimerStarted = 0;
	init_waitqueue_head(&demo.waitEvent);

	//Setup timer APIS
	timer_setup(&demo.m_timer,timer_expire_cb,0);
	demo.m_timer.expires = jiffies + demo.nbTimeoutInSec*HZ;

	pr_notice("in_intr: %ld in_atomic: %d  pid:%d name:%s  \n",
		in_interrupt() , in_atomic() , current->pid , current->comm);

	//add this timer to run
	add_timer(&demo.m_timer);
	wait_event_interruptible(demo.waitEvent,demo.nbTimerStarted != 0 );	

	pr_notice("\t\t Modified timer exipry \n");
	mod_timer(&demo.m_timer,jiffies + 2*demo.nbTimeoutInSec*HZ);
	pr_notice("\t\tStart_timer_demo: Exit \n");
}


static int __init timer_demo_init(void)
{
	pr_notice("TimerDemo: Start \n");
	Start_timer_demo();
	return 0;
}


static void __exit timer_demo_exit(void)
{
	pr_notice("TimerDemo: exit \n");
}

module_init(timer_demo_init)
module_exit(timer_demo_exit)
MODULE_LICENSE("GPL");
