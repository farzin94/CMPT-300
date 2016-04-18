#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/compiler.h>
#include <asm/percpu.h>
#include <asm-generic/errno-base.h>
#include <linux/errno.h>
#include <linux/string.h>

#include "process_ancestors.h"


/**
 * [sys_process_ancestors description]
 * @param  info_array [An array of process_info structs that will
 * be written to by the kernel as it fills in information from the
 * current process on up through its ancestors]
 * @param  size       [ The number of structs in info_array[] ]
 * @param  num_filled [ A pointer to a long. To this location the
 * kernel will store the number of structs (in info_array[])
 * which are written by the kernel.]
 * @return            [0 when successful, -EINVAL if size <= 0,
 * -EFAULT if any problems access info_array or num_filled.]
 */
asmlinkage long sys_process_ancestors(
	struct process_info info_array[],
	long size,
	long *num_filled
);

asmlinkage long sys_process_ancestors(
	struct process_info info_array[],
	long size,
	long *num_filled
){
	
	struct process_info process;
	struct task_struct task_struct;
	struct task_struct *task;
	//struct cred cr;	
	struct list_head *list;
	
	long count=0;
	int count_children=0;
	int count_sibling=0;
	int i=0;
	int j=0;	

	/*if (size<=0) return -EINVAL;
	
	if (!info_array || !num_filled) return -EFAULT;*/
// long pid; /* Process ID */
// 	char name[ANCESTOR_NAME_LEN]; /* Program name of process */
// 	long state; /* Current process state */
// 	long uid; /* User ID of process owner */
// 	long nvcsw; /* # voluntary context switches */
// 	long nivcsw; /* # involuntary context switches */
// 	long num_children; /* # children process has */
// 	long num_siblings; /* # sibling process has */

	//process = info_array[0];

	//for (i=0; i<size; i++) {
	for (task = current; task != &init_task; task = task->parent) {
		
		process.pid = (long)task->pid; 
		//printk("Process ID: %ld\n", process.pid);	
	
		process.state = task->state;
		//printk("Process State: %ld\n", process.state);

		//process.uid = (long)cred->uid;
		//printk("Process UID: %ld\n", process.uid);
		//printk("Process Name: "); 
		/*for (j=0; j<16; j++) {
			process.name[j] = task_struct.comm[j];
			printk("%c", process.name[j]);
		}*/
		memset(process.name, '\0', sizeof(process.name));
		strcpy(process.name, task->comm);
		

		list_for_each(list, &current->children) {
			//task = list_entry(list, struct task_struct, sibling);
			// task now points to one of current’s children
			count_children++;
			//if (task==task_struct) break;
		}
		list_for_each(list, &current->sibling) {
			//task = list_entry(list, struct task_struct, sibling);
			// task now points to one of current’s children
			count_sibling++;
			//if (task==task_struct) break;
		}
		process.num_children = count_children;
		process.num_siblings = count_sibling;
		//printk("Process num_children: %ld\n", process.num_children);		
		
		info_array[i] = process;
		i++;
		
		//task_struct = task;

		//if (process.pid == current->pid) break;
		//count++;
	}

	//&num_filled = count;

	return 0;
}
