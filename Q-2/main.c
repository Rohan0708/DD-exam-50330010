#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/wait.h>
#include<linux/kthread.h>

DEFINE_SEMAPHORE(sem);

struct cdev my_cdev;

dev_t dev,dev1;

wait_queue_head_t wq; // dyanmic alloc of wait queue

int flag;

//declaration
int NAME_open(struct inode *inode,struct file *filp);
ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int NAME_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner  =   THIS_MODULE,
    .open   =   NAME_open,
    .read   =   NAME_read,
    .write  =   NAME_write,
    .release=   NAME_release,
};




static int __init NAME_init(void)
{
    
    if((alloc_chrdev_region(&dev,25,1,"NAME") & alloc_chrdev_region(&dev1,26,1,"dev2"))<0)
    {
        printk("\n cannot create major number..");
        return -1;
    }
    printk(KERN_ALERT "\n major number =%d,minor number =%d",MAJOR(dev),MINOR(dev));
     printk(KERN_ALERT "\n major number =%d,minor number =%d",MAJOR(dev1),MINOR(dev1));

    // creating cdev structure
    cdev_init(&my_cdev,&fops);


    // adding major number with cdev

    if((cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add major number and cdev.\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    init_waitqueue_head(&wq);

   
    printk("\n device driver loaded..\n");
    return 0;  
    
}

static void __exit NAME_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev,1);
    printk("device driver unloaded..\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rohan");

module_init(NAME_init);
module_exit(NAME_exit);

char kbuff[50];

int NAME_open(struct inode *inode,struct file *filp)
{
    printk("\n....opened call called..\n");
    return 0;
}

ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
    unsigned long result;
    ssize_t retval;
    result=copy_to_user((char*)ubuff,(char*)kbuff,count);
    flag =1;
    wake_up(&wq);
    if(result==0)
    {
        printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",kbuff);
        printk(KERN_INFO  "\n DATA SEND COMPLETED..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {  
        printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",kbuff);
        printk(KERN_ALERT "\n THE PART OF DATA IS SENDED..\n ");
        retval=(count-result);
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\n ERROR IN READING");
        retval=-EFAULT;
        return retval;
    }   
}

ssize_t NAME_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp)
{

    unsigned long result;
    ssize_t retval;
    down(&sem);
    result=copy_from_user((char*)kbuff,(char*)ubuff,count);
    flag =0;
    up(&sem);    
     wait_event(wq,flag==1);

    if(result==0)
    {
        printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",ubuff);
        printk(KERN_INFO  "\n DATA RECEIVED COMPLETED..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {  
        printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",ubuff);
        printk(KERN_ALERT "\n THE PART OF DATA IS RECEIVED..\n ");
        retval=(count-result);
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\n ERROR IN WRITING");
        retval=-EFAULT;
        return retval;
    }
    
}
int NAME_release(struct inode *inode,struct file *filp)
{
    printk("\n closed call closed..\n");
    return 0;
}