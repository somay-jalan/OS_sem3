#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Counting the number of processes");
MODULE_AUTHOR("SOMAY_JALAN");

struct task_struct *task;
 
int iterate_init(void){
    int count=0;
    printk(KERN_INFO "%s","LOADING MODULE\n");
    for_each_process( task ){
        printk(KERN_INFO "The process is PID:%d STATUS:%i",task->pid,task->__state);
        count++;
    } 
    printk(KERN_INFO "The number of current running processes are: %d",count);
    return 0;
}
     
void cleanup_exit(void){
    printk(KERN_INFO "%s","REMOVING MODULE\n");
}

module_init(iterate_init);
module_exit(cleanup_exit);

