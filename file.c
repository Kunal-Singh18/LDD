#include <linux/init.h>
#include <linux/module.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include<linux/slab.h>                 
#include<linux/uaccess.h> 

#define mem_size        1024 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUNAL SINGH");
MODULE_DESCRIPTION("MAJOR AND MINOR NUMBERS");
MODULE_VERSION("1.0");
uint8_t *kernel_buffer;

dev_t div =0;
static struct class *class_pointer;
struct device *device_pointer;// no use of this.

static struct cdev cdev_struture;
/*
struct cdev { 
    struct kobject kobj; 
    struct module *owner; 
    const struct file_operations *ops; 
    struct list_head list; 
    dev_t dev; 
    unsigned int count; 
};

struct file_operations fops {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
    ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
    int (*iterate) (struct file *, struct dir_context *);
    int (*iterate_shared) (struct file *, struct dir_context *);
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    int (*mmap) (struct file *, struct vm_area_struct *);
    int (*open) (struct inode *, struct file *);
    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *);
    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    int (*fasync) (int, struct file *, int);
    int (*lock) (struct file *, int, struct file_lock *);
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
    int (*check_flags)(int);
    int (*flock) (struct file *, int, struct file_lock *);
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **, void **);
    long (*fallocate)(struct file *file, int mode, loff_t offset,
              loff_t len);
    void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
    unsigned (*mmap_capabilities)(struct file *);
#endif
    ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
            loff_t, size_t, unsigned int);
    int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
            u64);
    ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,
            u64);
};*/

int  dev_release(struct inode *inode, struct file *filp);
int dev_open(struct inode *inode, struct file *filp);
ssize_t dev_read (struct file *filp, char __user * buf, size_t len, loff_t * off);
ssize_t dev_write (struct file *filp, const char __user * buf, size_t len, loff_t * off);
struct file_operations fops ={
    .owner = THIS_MODULE,
   .read = dev_read,
   .write  = dev_write,
    .open = dev_open,
    .release = dev_release
};
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

static int __init entry(void)
{   
    if((alloc_chrdev_region(&div, 0, 1, "region_x")) <0){
        printk(KERN_INFO "Cannot allocate major number for device 1\n");
        return
         -1;
      }
    printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(div), MINOR(div));
    cdev_init(&cdev_struture, &fops);
    if(cdev_add(&cdev_struture, div, 1)<0)
    {
        printk(KERN_INFO "cannot add the device tot he system \n");
        goto label2;
    }
    class_pointer= class_create("class_name");// on github only one argument is given over there.
    if(IS_ERR(class_pointer))
    {
        printk(KERN_INFO "cannot create the strut class for devices\n");
        goto label2;
    }
    device_pointer=device_create(class_pointer, NULL,  div, NULL, "device_name");
    if(IS_ERR(device_pointer))
    {
        printk(KERN_INFO "cannot create the device\n");
        goto label1;
    }
    printk(KERN_INFO "Kernel Module Inserted Successfully ..\n");
    kernel_buffer= (uint8_t*) kmalloc(mem_size,GFP_KERNEL);// type cast.
    if(kernel_buffer == 0)
    {
        printk(KERN_INFO "Cannot allocate memory in kernel\n");
        goto label1;
    }
    return 0;

   label1:
   class_destroy (class_pointer);
   label2:
   unregister_chrdev_region(div,1);
   return -1;
   


}
static void __exit exit1(void)
{
    kfree(kernel_buffer);
    device_destroy (class_pointer, div);
    class_destroy (class_pointer);
    cdev_del(&cdev_struture);
    unregister_chrdev_region(div,1);// this is correct syntax in case our module minor number is not starting from 0. MKDEV(MAJOR(div), 2),
    printk("dellocated \n");
}

module_init(entry);
module_exit(exit1);