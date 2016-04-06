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
long sys_process_ancestors(
	struct process_info info_array[],
	long size,
	long *num_filled
){
	
}
