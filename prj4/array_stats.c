#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/uaccess.h>
//#include <errno.h>

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
	struct array_stats{
	long min;
	long max;
	long sum;
};
 */
asmlinkage long sys_array_stats(struct array_stats *stats, long data[], long size)
{
	struct array_stats copied = {10000000, 0, 0};
	long dataCopied = 0;
	//long len = 0;
	long result = 0;
	int i = 0;
	while (i<size)
	{
		/* copy src, which is in the user’s address space, into buf */
		if (copy_from_user(&dataCopied, &data[i], sizeof(data[i])))
		{
			return -EFAULT;
		}
		/* copy buf into dst, which is in the user’s address space */ 		
		if (copied.min > dataCopied)
		{
			copied.min = dataCopied;
		}
		printk("--min %ld\n", copied.min);
		if (copied.max < dataCopied)
		{
			copied.max = dataCopied;
		}
		printk("--max %ld\n", copied.max);
		
		copied.sum = (copied.sum + dataCopied);
		printk("--sum %ld\n", copied.sum);
		
		printk("--data %ld\n", dataCopied);
		i++;
	}
	/* copy buf into dst, which is in the user’s address space */
	if (copy_to_user(stats, &copied, sizeof(copied)))
	{
		return -EFAULT;
	}
	printk("--STATSmax %ld\n", stats->min);
	printk("--STATSmax %ld\n", stats->max);
	printk("--STATSmax %ld\n", stats->sum);
	return result;
}

