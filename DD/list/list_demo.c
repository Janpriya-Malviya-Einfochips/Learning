#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>

//########### Test Jiffies things as well ############
#include <linux/sched.h> //for access jiffies 
#include <linux/time_types.h>
#include <linux/time.h>	// time API with Jiffies

#include <linux/timex.h>	// time API for getting clock count 

//shorted list based on priority 
typedef struct listDemo
{
	u32  m_priority;
	char   m_name[10];
	struct list_head m_list;
}sDetails;

static struct list_head m_details = LIST_HEAD_INIT(m_details);



static void jiffies_tutorial(void)
{
	unsigned long  j = jiffies;
	unsigned long onesec  = j + HZ;
	//struct old_timeval32 t;
	struct timespec64 s;

	pr_notice("Jiffes32  %ld onesec: %ld diff:%ld \n",j,onesec,onesec-j);
	pr_notice("Jiffes64  %lld \n",get_jiffies_64());

	j = jiffies;
	jiffies_to_timespec64(j,&s);
	//jiffies_to_timesval(j,&t);

	pr_notice("%ld timespace: %lld.%ld \n",j,s.tv_sec , s.tv_nsec);
	//pr_notice("%ld timeval: %ld.%ld \n",j,t.tv_sec , t.tv_usec);
	//pr_notice("	timeval2jiffies: %ld  \n",timeval_to_jiffies(&t));
	pr_notice("	timespec2jiffies: %ld  \n",timespec64_to_jiffies(&s));
	pr_notice("	get_cycles: %lld  \n",get_cycles());


	j = jiffies;
	//do_gettimeofday64(&s);
	//pr_notice("Jiffes32  %ld do_gettimeofday: %ld.%ld \n",j,t.tv_sec , t.tv_usec);

	j = jiffies;
	ktime_get_raw_ts64(&s);
	pr_notice("Jiffes32  %ld current_kernel_time: %lld.%ld \n",j,s.tv_sec , s.tv_nsec);
}

static void create_details_list(void)
{	
	char tName[10][10] = {
			"JP_0",
			"JP_1",
			"JP_2",
			"JP_3",
			"JP_4",
			"JP_5",
			"JP_6",
			"JP_7",
			"JP_8",
			"JP_9",
			"JP_10",
	} ;

	for( int i = 0 ; i < 10 ; i++ )
	{
		sDetails* pDetails = (sDetails*)( kmalloc( sizeof(sDetails) , GFP_KERNEL) );
		if( pDetails )
		{
			pDetails->m_priority = i;
			INIT_LIST_HEAD(&pDetails->m_list);
			memcpy(pDetails->m_name,tName[i],7);

			//add node in list
			pr_notice("Adding node in list \n");
			list_add(&pDetails->m_list,&m_details);
		}
	}
}

static void print_list(struct list_head* head )
{
	sDetails* tcur;
	pr_notice("print_list-->\n");
	list_for_each_entry(tcur,head,m_list) {
		pr_notice(" %d  -- %s \n",tcur->m_priority,tcur->m_name);
	}
	pr_notice("print_list-->Done\n");
}

static void free_resource(struct list_head* head )
{
	//NOTE ############### if you going to use list_for_each_entry it will create 
	// CORE DUM 
	#if 0
	sDetails* tcur;
	list_for_each_entry(tcur,head,m_list) {
		pr_notice("deleting %d  -- %s \n",tcur->m_priority,tcur->m_name);
		list_del(&tcur->m_list);
		kfree(tcur);
	}
	#else
	sDetails *cursor, *temp;
	list_for_each_entry_safe(cursor, temp, head, m_list) {
		list_del(&cursor->m_list);
		kfree(cursor);
    }
    #endif
    print_list(head);
}


static int __init list_demo_init(void)
{
	pr_notice("list_demo_init Added  \n");
	create_details_list();
	print_list(&m_details);

	pr_notice("Testing Jiffies APIs  \n");
	jiffies_tutorial();
	return 0;
}


static void __exit list_demo_exit(void)
{
	free_resource(&m_details);

	pr_notice("Testing Jiffies APIs  \n");
	jiffies_tutorial();
	pr_notice("list_demo_exit removed  \n");
}

module_init(list_demo_init)
module_exit(list_demo_exit)
MODULE_LICENSE("GPL");
