#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUNAL SINGH");
MODULE_DESCRIPTION("ARGUMENTS TESTING");
MODULE_VERSION("1.0");

int x,arr[2];
char *ptr;
int change=0;

module_param(x, int, S_IWUSR | S_IRUSR);
module_param_array(arr, int,NULL, S_IWUSR | S_IRUSR);
module_param(ptr, charp,S_IRUSR|S_IWUSR);

int notification(const char *val, const struct kernel_param *kp)//the second parameter contains meta data.
{
    int res = param_set_int(val, kp); // Use helper for write variable
    if(res==0) {
            printk(KERN_INFO "Call back function called...\n");
            printk(KERN_INFO "New value of cb_valueETX = %d\n", change);
            return 0;
    }
    return -1;
}

struct kernel_param_ops operations =
{   
    .get = &param_get_int,// default function which returns value in string.
    .set=notification, // &notification is also correct it depends on compiler.
};

module_param_cb(change, &operations , &change,S_IRUGO |S_IWUSR );


/* module init and exit functions here.*/

static int  __init module_entry(void)// __init tells to keep this in speacil memory section and remove /fress it after initialisation
{
    printk("Hello everyone \n");
    int i;
        printk(KERN_INFO "x = %d  \n", x );
        printk(KERN_INFO "change_value = %d  \n", change);
        printk(KERN_INFO "cahr_value %s \n", ptr);
        for (i = 0; i < (sizeof (arr) / sizeof (int)); i++) {
                printk(KERN_INFO "Arr_value[%d] = %d\n", i, arr[i]);
        }
        printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
        return 0;
} 
static void __exit exit_modul(void)
{
    printk("good bye \n");
}

module_exit(exit_modul);
module_init(module_entry);
