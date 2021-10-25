#include <linux/init.h>
// /usr/src/linux-headers-5.7.0-kali1-common/include/linux
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/slab.h> //kalloc() kfree()
#include "mtx.h"

#define DEVICE_NAME "mtx"

typedef struct{
    int locked;
}kmtx;

//floating point is allowed in the kernel
//pointers from user space can be derefrence here
//but pointers to objects allocated here cannot be derefenced
//by a user program
//this applies to global variables and variables on the stack

//these functions are atomic (they cannot be interrupted)
//this is because the kernel cannot interupt itself
int try_lock(kmtx *m){
    if(m->locked) return 0;
    m->locked=1;
    return 1;
}

int try_unlock(kmtx *m){
    if(m->locked) m->locked=0;
    return 1;
}

//persistant global mutex that exits in the kernel
kmtx mtx;
long a=420;

void print_caller(void){
    printk(KERN_INFO "The process is \"%s\" (pid %i)\n",current->comm, current->pid);
}

//our implementation of the read system call
ssize_t dev_read(struct file *filep,char *buffer,size_t len,loff_t *offset){
    //len=2 means reset
    //len=1 means you want to lock
    //len=0 is an unlock attempt
    // printk(KERN_ALERT "read");
    // if(buffer!=NULL){
    //     long d=420;
    //     long *b=&d;
    //     long **c=&b;
    //     printk(KERN_ALERT "ptr: %lx",&d);
    //     memcpy(buffer,c,8);
    // }
    printk(KERN_ALERT "ptr: %lx",buffer);
    if(len==1) return try_lock(&mtx);
    if(len==0) return try_unlock(&mtx);
    if(len==2){
        mtx.locked=0;
        return 2;
    }
    return -1;
}

//structure containning pointers to functions which implement syscalls
struct file_operations fops={
   .read = dev_read,
};

int major;

int __init mtx_init(void){
    mtx.locked=0;
    major=register_chrdev(0,DEVICE_NAME,&fops);
    printk("mtx module has been loaded: %d\n", major);
    return 0;
}
void __exit mtx_exit(void){
    unregister_chrdev(major, DEVICE_NAME);
    printk("mtx module has been unloaded\n");
}

module_init(mtx_init);
module_exit(mtx_exit);
