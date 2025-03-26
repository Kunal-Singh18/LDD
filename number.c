#include <linux/init.h>
#include <linux/module.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/device.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUNAL SINGH");
MODULE_DESCRIPTION("MAJOR AND MINOR NUMBERS");
MODULE_VERSION("1.0");
// int alloc_chrdev_region(dev_t *, unsigned, unsigned, const char *);

// dev_t div = MKDEV(32,0); bc isko daal ke synamicalay allocate kr rhe toh dikakt hogi hi

dev_t div =0;

static int __init entry(void)
{   
    // gandu is device.
    // int status = register_chrdev_region(div, 400,"region_x");// arguenets dev_t , then range and then name.
    if((alloc_chrdev_region(&div, 2, 4, "region_x")) <0){
        printk(KERN_INFO "Cannot allocate major number for device 1\n");
        return -1;
      }
printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(div), MINOR(div));
printk(KERN_INFO "Kernel Module Inserted Successfully...\n");

return 0;
}
static void __exit exit1(void)
{
    unregister_chrdev_region(div,4);// this is correct syntax in case our module minor number is not starting from 0. MKDEV(MAJOR(div), 2),
    printk("dellocated \n");
}

module_init(entry);
module_exit(exit1);