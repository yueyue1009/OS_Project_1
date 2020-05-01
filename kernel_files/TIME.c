#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage int TIME(unsigned long __user*s, unsigned long __user *ns)
{
	struct timespec now;

	getnstimeofday(&now);

	copy_to_user(s, &now.tv_sec, sizeof(unsigned long));
	copy_to_user(ns, &now.tv_nsec, sizeof(unsigned long));
	return 0;
}
