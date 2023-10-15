#include<linux/module.h>

#include <linux/sched.h> //for access jiffies , shduled_timeout() shecudal()
#include <linux/time_types.h>
#include <linux/time.h>	// time API with Jiffies
#include <linux/jiffies.h>	// For HZ


#include <linux/wait.h>	// Wait Q
#include <linux/delay.h>	// Wait Q

static void busy_waiting(void)
{
	u64 targeJiffies = jiffies + 5*HZ;

	pr_notice("Started : busy_waiting , waiting for 5 seconds \n");

	while(time_before64(get_jiffies_64(),targeJiffies))
		cpu_relax();

	pr_notice("End : busy_waiting \n");

}

static void schedule_waiting(void)
{
	u64 targeJiffies = jiffies + 5*HZ;

	pr_notice("Started : schedule_waiting , waiting for 5 seconds \n");

	while(time_before64(get_jiffies_64(),targeJiffies))
		schedule();

	pr_notice("End : schedule_waiting \n");

}


static void waitQ_waiting(void)
{
	wait_queue_head_t waitQ;
	init_waitqueue_head(&waitQ);

	pr_notice("Started : waitQ_waiting , waiting for 5 seconds \n");
	wait_event_timeout(waitQ,0,5*HZ);	
	pr_notice("End : waitQ_waiting \n");

}

static void schedule_timeout_waiting(void)
{
	pr_notice("Started : schedule_timeout_waiting , waiting for 5 seconds \n");
	set_current_state(TASK_INTERRUPTIBLE);	//NOTE: if you not set this then schedule_timeout work as shecudle() no wait will happens
	schedule_timeout(5*HZ);	
	pr_notice("End : schedule_timeout_waiting \n");
}


static void small_delays(void)
{
	pr_notice("Started : small_delays \n");
	pr_notice("sleeping for mdelay 1000ms");
	mdelay(1000);
	pr_notice("sleeping for 1000000us __bad_udelay error for large value during compliaton");
	//udelay(10000000);	//modpost: "__bad_udelay" [/home/developer/Work/Learning/Learning/DD/delays/delays_demo.ko] undefined! for lagre value
	pr_notice("sleeping for 1000us ");
	udelay(1000);	
	pr_notice("sleeping for msleep 1000ms");
	msleep(1000);
	pr_notice("sleeping for msleep 1");
	ssleep(1);
	pr_notice("End : small_delays \n");
}


static int __init delays_demo_init(void)
{
	pr_notice("Started : delays_demo_init \n");
	busy_waiting();
	schedule_waiting();
	waitQ_waiting();		//NOTE: insmode not able to come out till 5 seconds !
	schedule_timeout_waiting();
	small_delays();
	return 0;
}
static void __exit delays_demo_exit(void)
{
	pr_notice("Exited : delays_demo_exit \n");
	
}


module_init(delays_demo_init)
module_exit(delays_demo_exit)
MODULE_LICENSE("GPL");
