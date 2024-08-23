#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define MODULE_NAME "greeter"
MODULE_AUTHOR("fun-var");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple kernel module to display CPU temperature");
MODULE_VERSION("0.1");

static char *name = "Var-temp";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int __init greeter_init(void)
{
    struct file *file;
    char buf[16];
    mm_segment_t oldfs;
    int temp;

    file = filp_open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY, 0);
    if (IS_ERR(file)) {
        pr_err("%s: Failed to open temperature file\n", MODULE_NAME);
        return PTR_ERR(file);
    }

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    vfs_read(file, buf, sizeof(buf) - 1, &file->f_pos);
    buf[sizeof(buf) - 1] = '\0'; 

    set_fs(oldfs);
    filp_close(file, NULL);

    temp = simple_strtol(buf, NULL, 10);

    pr_info("%s: module loaded at 0x%p\n", MODULE_NAME, greeter_init);
    pr_info("%s: greetings %s, CPU temperature is %d.%03d°C\n", MODULE_NAME, name, temp / 1000, temp % 1000);

    return 0;
}

static void __exit greeter_exit(void)
{
    pr_info("%s: goodbye %s\n", MODULE_NAME, name);
    pr_info("%s: module unloaded from 0x%p\n", MODULE_NAME, greeter_exit);
}

module_init(greeter_init);
module_exit(greeter_exit);