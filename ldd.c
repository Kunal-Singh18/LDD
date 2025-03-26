#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUNAL SINGH");
MODULE_DESCRIPTION("PHLI MODULE HAI");


ssize_t driver_read(struct file *file_pointer,
     char __user *user_space_buffer, // user spcae can use this buffer.
     size_t count, 
     loff_t * offset)// this is the offset due to which subsequent reads happen.
{  
    
    char mssg[] = "asjfsndjfsjdfsjdfn \n";
    size_t  len = strlen(mssg);
    *offset +=len;
    if(*offset > len)
    {
       // return -1; don't return an error code 
       return 0;
    }
    int result = -1 ;
    result=copy_to_user(user_space_buffer,mssg,len);

    printk("driver read %d \n",result);
    return len;// we need to return this.
}
static struct proc_dir_entry *proc_node;
struct proc_ops driver_proc_ops ={
    .proc_read = driver_read 
};

static int kunal(void)// module_init is standard way of writing the code
{
    printk("create entry \n");
    proc_node = proc_create("basic_driver",
           0,
          NULL, 
          &driver_proc_ops);
    
    /*if(proc_node ==NULL)// but this does not work as it does not return anything.
    {
        printk("failed \n");
        return -2;// error code.
    }*/
    
    printk("create exit \n");
    return 0;
}
static void kunal1(void)// module_exit is the standard.
{
    printk("delete entry \n");
    proc_remove(proc_node);
    printk("delete exit \n");
}
module_exit(kunal1);
module_init(kunal);
 