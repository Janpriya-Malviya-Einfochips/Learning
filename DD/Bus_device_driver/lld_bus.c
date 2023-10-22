#include<linux/module.h>
#include<linux/timer.h>
#include<linux/device/bus.h>
#include<linux/device.h>
#include<linux/of_device.h>


struct lld_device
{
	u32 lld_device_id;
	char* name;
	struct device ldev;	
};

struct lld_driver
{
	struct device_driver ldriver;
};

static const struct of_device_id lld_driver_match_ids[] = {
	{
		.name = "lld,dev_1",
	},
	{
		.name = "lld,dev_2",
	},
	{
		.name = "lld,dev_3",
	},
	{
		.name = "lld_device",
	},
	{ /* sentinel */ }
};

static struct device_node lld_device_node = {
	.name = "lld_device",
};

static struct of_device_id* lld_bus_of_match_device(const struct of_device_id *matches , struct lld_device* device )
{
	const struct of_device_id *match;

	pr_notice("lld_bus:  lld_bus_of_match_device1 \n");
	if (!(device && matches))
		return NULL;

	for (; matches->name[0] ; matches++) 
	{
		u32 len = strlen(&matches->name[0]);
		if( (strlen(device->ldev.of_node->name) == len) && (strncmp(device->ldev.of_node->name, &matches->name[0], len) == 0) )
		{
			pr_notice("matches->name[0] %s   deviceName: %s \n",&matches->name[0],device->ldev.of_node->name);
			return matches; 
		}
	}
	return NULL;
#if 0
	pr_notice("lld_bus:  lld_bus_of_match_device2 \n");
	match = of_match_device(matches, &device->ldev);
	if (match)
	{
		pr_notice("lld_bus:  lld_bus_of_match_device3 \n");
		return match;
	}
	pr_notice("lld_bus:  lld_bus_of_match_device4 \n");
	return NULL;
#endif
}
//=============================================================================
//=============================================================================
//					LLD BUS APIS
//=============================================================================
//=============================================================================
static int lld_bus_match( struct device *dev, struct device_driver *drv )
{
	struct lld_device* lld_dev = container_of(dev,struct lld_device , ldev );
	struct of_device_id* match = NULL;

	pr_notice("lld_bus:  lld_bus_match---> Enter \n");

	match = lld_bus_of_match_device(drv->of_match_table,lld_dev);
	if( match )
	{
		pr_notice("lld_bus:  lld_bus_match---> [%s] found Exit  \n",match->name);
		return 1;
	}
	pr_notice("lld_bus:  lld_bus_match---> Not found Exit  \n");
	return  -1;
}

static int lld_bus_uevent( struct device *dev, struct kobj_uevent_env *env )
{
	int i = 0;
	pr_notice("lld_bus:  lld_bus_uevent---> Enter \n");
	for( ; i < UEVENT_NUM_ENVP && env->envp[i] ; i++ )
	{
		pr_notice("    %d envp=[%s]  buf=[%s] \n",i,env->envp[i],env->buf);
	}
	pr_notice("lld_bus:  lld_bus_uevent---> Exit  \n");
	return  -1;
}

static int lld_bus_probe( struct device *dev )
{
	pr_notice("lld_bus:  lld_bus_probe---> Enter [%s]\n",dev->of_node->name);

	//get driver 
	if( dev->driver && dev->driver->probe )
	{
		return dev->driver->probe(dev);	
	}
	pr_notice("lld_bus:  lld_bus_probe---> Exit  \n");
	return  0;
}

static void lld_bus_remove( struct device *dev )
{
	pr_notice("lld_bus:  lld_bus_remove---> Enter \n");
	pr_notice("lld_bus:  lld_bus_remove---> Exit  \n");
}

static ssize_t lld_bus_version_show(struct bus_type *bus, char *buf)
{	
	char* Version = "lld_bus_V1.0.0";
	return sprintf(buf,"%s\n",Version);
}

static ssize_t lld_bus_poll_timeout_show(struct bus_type *bus, char *buf)
{
	pr_notice("lld_bus:  lld_bus_poll_timeout_show---> Enter \n");
	pr_notice("lld_bus:  lld_bus_poll_timeout_show---> Exit  \n");
	return 0;
}

static ssize_t lld_bus_poll_timeout_store(struct bus_type *bus, const char *buf, size_t count)
{
	pr_notice("lld_bus:  lld_bus_poll_timeout_store---> Enter \n");
	pr_notice("lld_bus:  lld_bus_poll_timeout_store---> Exit  \n");
	return -1;
}

//=============================================================================
//=============================================================================
//					LLD Device APIs
//=============================================================================
//=============================================================================
static void lld_device_release(struct device *dev)
{
	pr_notice("lld_dev:  lld_device_release---> Enter[%s] \n",dev->init_name);
	pr_notice("lld_dev:  lld_device_release---> Exit  \n");
}


//=============================================================================
//=============================================================================
//					LLD Driver APIS
//=============================================================================
//=============================================================================
static int lld_driver_probe(struct device *dev)
{
	pr_notice("lld_driver:  lld_driver_probe---> Enter[%s] \n",dev->of_node->name);
	pr_notice("lld_driver:  lld_driver_probe---> Exit  \n");
	return 0;
}
static void lld_driver_sync_state(struct device *dev)
{
	pr_notice("lld_driver:  lld_driver_sync_state---> Enter[%s] \n",dev->init_name);
	pr_notice("lld_driver:  lld_driver_sync_state---> Exit  \n");
}
// static int lld_driver_remove(struct device *dev)
// {
// 	pr_notice("lld_driver:  lld_driver_remove---> Enter[%s] \n",dev->init_name);
// 	pr_notice("lld_driver:  lld_driver_remove---> Exit  \n");
// 	return -1;
// }

// static void lld_driver_shutdown(struct device *dev)
// {
// 	pr_notice("lld_driver:  lld_driver_shutdown---> Enter[%s] \n",dev->init_name);
// 	pr_notice("lld_driver:  lld_driver_shutdown---> Exit  \n");
// }
static int lld_driver_suspend(struct device *dev, pm_message_t state)
{
	pr_notice("lld_driver:  lld_driver_suspend---> Enter[%s] \n",dev->init_name);
	pr_notice("lld_driver:  lld_driver_suspend---> Exit  \n");
	return -1;
}

static int lld_driver_resume(struct device *dev)
{
	pr_notice("lld_driver:  lld_driver_resume---> Enter[%s] \n",dev->init_name);
	pr_notice("lld_driver:  lld_driver_resume---> Exit  \n");
	return -1;
}


//create bus 
struct bus_type lld_bus = {
	.name   = "lld",
	.match  = lld_bus_match,
	.remove = lld_bus_remove,
	.uevent = lld_bus_uevent,
	.probe  = lld_bus_probe,
};

BUS_ATTR_RO(lld_bus_version);
BUS_ATTR_RW(lld_bus_poll_timeout);

struct lld_device lld_dev;
struct lld_driver lld_drv;

static int create_and_add_lld_bus(void)
{
	int ret = 0;
	pr_notice("create_and_add_lld_bus -->\n");
	pr_notice("     Reistering bus type -->\n");

	pr_notice("=================== Bus  Register =============================\n");
	ret = bus_register(&lld_bus);
	if (ret)
		return ret;	

	//create version
	ret = bus_create_file(&lld_bus, &bus_attr_lld_bus_version);
	if (ret)
		goto lld_bus_version_fail;

	ret = bus_create_file(&lld_bus, &bus_attr_lld_bus_poll_timeout);
	if (ret)
		goto lld_bus_poll_timeout_fail;

	return 0;

lld_bus_poll_timeout_fail:
	bus_remove_file(&lld_bus, &bus_attr_lld_bus_version);

lld_bus_version_fail:
	bus_unregister(&lld_bus);
	return -1;
}

static void remove_lld_bus(void)
{
	pr_notice("     UnRgister bus type -->\n");
	pr_notice("=================== Bus  Removed =============================\n");
	bus_unregister(&lld_bus);
}

static int create_and_add_lld_device(void)
{	
	struct device* dev = &lld_dev.ldev;
	dev->bus = &lld_bus;
	dev->init_name = "lld_device";
	dev->of_node = &lld_device_node;
	dev->release = &lld_device_release;
	pr_notice("=================== Device  Register =============================\n");
	return device_register(dev);
};

static void remove_lld_device(void)
{
	pr_notice("=================== Device  Removed =============================\n");
	device_unregister(&lld_dev.ldev);
};

static int create_and_add_lld_driver(void)
{	
	struct device_driver* driver = &lld_drv.ldriver;
	driver->bus = &lld_bus;
	driver->name = "lld_driver";
	driver->of_match_table = lld_driver_match_ids;
	driver->probe = &lld_driver_probe;
	driver->sync_state = &lld_driver_sync_state;
	//driver->remove = &lld_driver_remove;
	//driver->shutdown = &lld_driver_shutdown;
	driver->suspend = &lld_driver_suspend;
	driver->resume = &lld_driver_resume;
	pr_notice("=================== Driver  Register =============================\n");
	return driver_register(driver);
};

static void remove_lld_driver(void)
{
	pr_notice("=================== Driver  Removed =============================\n");
	driver_unregister(&lld_drv.ldriver);
};

static int __init lld_bus_init(void)
{
	
	int ret = 0;
	pr_notice("lld_bus_init -->\n");
	ret = create_and_add_lld_bus();
	if(ret)
		return ret;

	ret = create_and_add_lld_driver();
	if(ret)
		goto driver_add_err;

	ret = create_and_add_lld_device();
	if(ret)
		goto device_add_err;

	return 0;

device_add_err:
	remove_lld_driver();

driver_add_err:
	remove_lld_bus();
	return ret;
}


static void __exit lld_bus_exit(void)
{
	pr_notice("lld_bus_exit \n");
	remove_lld_device();
	remove_lld_driver();
	remove_lld_bus();
}

module_init(lld_bus_init)
module_exit(lld_bus_exit)
MODULE_LICENSE("GPL");
