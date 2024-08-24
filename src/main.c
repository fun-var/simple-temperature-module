#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/thermal.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define MODULE_NAME "temperature"
MODULE_AUTHOR("fun-var");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple kernel module to display CPU temperature");
MODULE_VERSION("0.1");

char zone_name[] = "x86_pkg_temp";

static int __init greeter_init(void)
{
    struct thermal_zone_device *thermal_zone;
    thermal_zone = thermal_zone_get_zone_by_name(zone_name);
    if (IS_ERR(thermal_zone)) {
        pr_err("Failed to get thermal zone 0\n");
        return -ENODEV;
    }

    if (!thermal_zone->ops) {
        pr_err("Thermal zone NONE\n");
        return -1;
    }

    if(thermal_zone->ops->get_temp) 
        pr_info("Get temp is defined");

    int temp, ret;

    ret = thermal_zone_get_temp(thermal_zone, &temp);
    if (ret) {
        pr_err("Failed to read temperature for zone: %s\n", zone_name);
        return ret;
    }

    // Температура возвращается в тысячных долях градуса Цельсия
    pr_info("Temperature of zone %s: %d.%03d°C\n",
            zone_name, temp / 1000, temp % 1000);

    pr_info("%s: module loaded at 0x%p\n", MODULE_NAME, greeter_init);
    

    return 0;
}

static void __exit greeter_exit(void)
{
    pr_info("%s: module unloaded from 0x%p\n", MODULE_NAME, greeter_exit);
}

module_init(greeter_init);
module_exit(greeter_exit);