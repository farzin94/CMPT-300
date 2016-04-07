#include "array_stats.h"
#include <errno.h>

/**
 * [sys_array_stats Set the three fields of the stats structure based on the data array.
 * The values in data are signed (positive or negative). Nothing
 * special need be done if the sum of all the data overflows/underflows a long]
 * @param  stats [ A pointer to one array_stats structure allocated
 * by the user-space application. Structure will be written to
 * by the sys-call to store the minimum, maximum, and sum
 * of all values in the array pointed to by data.]
 * @param  data  [ An array of long int values passed in by the userspace
 * application ]
 * @param  size  [ The number of elements in data. Must be > 0 ]
 * @return [ 0 when successful, -EINVAL when size <=0, -EFAULT for mem access]
 * sys-call number = 331
 */
asmlinkage 	long sys_array_stats(
	struct array_stats *stats,
	long data[],
	long size
);
