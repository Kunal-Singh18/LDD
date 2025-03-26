#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define mem_size 102

#define WR_VAL _IOW('a',1,int32_t*)
#define RD_VAL _IOR('a',2,int32_t*)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUNAL SINGH");
MODULE_DESCRIPTION("ioctl");
MODULE_VERSION("1.0");

int32_t value=0;
dev_t div=0;
static struct class *class_pointer;
static struct cdev cdev;// dont declare it as a pointer or else it causes null pointere refernce error.
uint8_t *kernel_buffer;
/*IOCTL */
static long  driver_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
    switch(cmd)
    {
        case WR_VAL:
                if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                {
                        pr_err("Data Write : Err!\n");
                }
                pr_info("Value = %d\n", value);
        break;

        case RD_VAL:
                if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                {
                        pr_err("Data Read : Err!\n");
                }
                break;
        default:
                pr_info("deafult \n");
                break; 

    }
    return 0;
}
/*fops.*/
int  dev_release(struct inode *inode, struct file *filp)
{    
    pr_info("Driver Release Function Called...!!!\n");
    return 0;
}
int dev_open(struct inode *inode, struct file *filp)
{   
    pr_info("Driver Open Function Called...!!!\n");
    return 0;
}
ssize_t dev_read(struct file *filp, char __user * user_buffer, size_t len, loff_t * off)
{
    *off += mem_size;
    if(*off > mem_size)
    {
       // return -1; don't return an error code 
       return 0;
    }
    copy_to_user(user_buffer,kernel_buffer,  mem_size);
    pr_info("Driver Read Function Called...!!!\n");
    return mem_size;
}
ssize_t dev_write (struct file *filp, const char __user * buf, size_t len, loff_t * off)
{   
    copy_from_user(kernel_buffer, buf, len);
    pr_info("Driver Write Function Called...!!!\n");
    return len;
}
struct file_operations fops=
{
.owner=THIS_MODULE,
.write=dev_write,
.read=dev_read,
.open=dev_open,
.release=dev_release,
.unlocked_ioctl = driver_ioctl
};
static int __init ioctl_driver_entry(void)
{   
    //allocation of major and minor numbers
    if(alloc_chrdev_region(&div,0,1,"major")<0)//check allocation in the proc devices.
    {
        pr_info("unable to allocate the major number \n");
        return -1;
    }
    pr_info("MAJOR = %d, MINOR = %d \n",MAJOR(div),MINOR(div));

    /* Create a class */ 
    class_pointer = class_create("base_class"); // check in ls -l /sys/class
    if (IS_ERR(class_pointer)) {
        pr_info("Class creation failed \n");
        goto label2;
    }
   
    /*creating a device.*/
    if(IS_ERR(device_create(class_pointer,NULL,div,NULL,"device1")))// check in dev.
    {
        pr_info("deivce creation failed \n");
        goto label1;
    }
   
     /*creating a cdev structure*/
     cdev_init(&cdev, &fops);
     if(cdev_add(&cdev,div,1)<0)
     {
         pr_info("cannot add deice to the system \n");
         goto label1;
     }
     kernel_buffer= (uint8_t*) kmalloc(mem_size,GFP_KERNEL);// type cast.
    if(kernel_buffer == 0)
    {
        printk(KERN_INFO "Cannot allocate memory in kernel\n");
        goto label1;
    }
    pr_info("kernel module inserted.\n");
    
    return 0;

    label1:
    class_destroy(class_pointer);
    label2:
    unregister_chrdev_region(div,1);

    return -1;
}
static void __exit ioctl_driver_exit(void)
{   
    kfree(kernel_buffer);
    device_destroy (class_pointer, div);
    class_destroy (class_pointer);
    unregister_chrdev_region(div,1);
    pr_info("kernel module removed.\n");
    return ;
}
module_init(ioctl_driver_entry);
module_exit(ioctl_driver_exit);