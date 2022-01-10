

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include "ioctl.h"


#define NEW IOCTL
struct cdev *my_cdev;

//FOR DEFAULT DATA
struct stats
{
    int size;
    char kbuff[100];
    int r_w;
};
struct stats *data;

//protocol
int NEW_open(struct inode *inode,struct file *filp);
ssize_t NEW_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t NEW_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int NEW_release(struct inode *inode,struct file *filp);
long NEW_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);

struct file_operations fops=
{
    .owner =THIS_MODULE,
    .open  =NEW_open,
    .read  =NEW_read,
    .write =NEW_write,
    .release=NEW_release,
    .unlocked_ioctl=NEW_ioctl,
};




static int __init prog_init(void)
{
    int result;
    int MAJOR,MINOR;
    dev_t MYDEV;
    MYDEV=MKDEV(6,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d.. THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    result=register_chrdev_region(MYDEV,1,"IOCTL");
    if(result<0)
    {
        printk(KERN_INFO "\n THE DEVICE NUMBER IS NOT REGISTERED..\n");
        return(-1);
    }
    my_cdev=cdev_alloc();
    my_cdev->ops=&fops;

    result=cdev_add(my_cdev,MYDEV,1);
    if(result<0)
    {
        printk(KERN_INFO "\n THE DEVICE NUMBER AND CDEV NOT CONNECTED.\n");
        unregister_chrdev_region(MYDEV,1);
        return(-1);
    }
    return 0;
    
}
static void __exit prog_exit(void)
{
    int MAJOR,MINOR;
    dev_t MYDEV;
    MYDEV=MKDEV(6,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d.. THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    unregister_chrdev_region(MYDEV,1);
    cdev_del(my_cdev);
    printk(KERN_INFO "\n I HAVE REMOVED ALL THE INIT....\n");
}
module_init(prog_init);
module_exit(prog_exit);

//function definition
int NEW_open(struct inode *inode,struct file *filp)
{
    printk(KERN_ALERT "\n THE OPEN SYSTEM CALL...\n");
    return 0;
}

ssize_t NEW_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
    //char kbuff[60]="THIS IS MESSAGE FROM KERNEL....";
    unsigned long result;
    ssize_t retval;
    result=copy_to_user((char*)ubuff,(char*)data->kbuff,sizeof(data->kbuff));
    data->size = count;
    data->r_w = 0;
    if(result==0)
    {
        printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",data->kbuff);
        printk(KERN_INFO  "\n DATA SEND COMPLETED..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {  
       // printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",data->kbuff);
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

ssize_t NEW_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp)
{
    //char kbuff[60];
    unsigned long result;
    ssize_t retval;
    result=copy_from_user((char*)data->kbuff,(char*)ubuff,count);
    data->size = count;
    data->r_w = 1;

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
int NEW_release(struct inode *inode,struct file *filp)
{
    printk(KERN_ALERT "\n THE CLOSE SYSTEM CALL IS CALLED...\n");
    return 0;
}

long NEW_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
    
    printk("\n >>>> IOCTL FUNCTION <<<<");
   switch(cmd)
   {
       case GETSTATS:
                    copy_to_user((struct stat *)arg,data,sizeof(data));
                    break;

       default:
                printk(KERN_ALERT "\nCommand not found!!\n");
                return(-EINVAL);
   }
    return 0;
}
