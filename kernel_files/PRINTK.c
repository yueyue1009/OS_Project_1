#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage int PRINTK(unsigned long a, unsigned long b, unsigned long c, unsigned long d, int pid)
{
	printk("[project1] %d %lu.%09lu %lu.%09lu\n", pid, a, b, c, d);
}
