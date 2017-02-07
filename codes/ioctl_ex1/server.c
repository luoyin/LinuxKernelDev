#include <linux/module.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <asm/uaccess.h>	// copy_to_user copy_from_user
#define MODULE_NAME "luo_ioctl_ex1"
#define IOCTL_EX1_TEST 0x0	// request code

#define IOCTL_EX1_TEST_OK 0
#define IOCTL_EX1_TEST_UNKNOWN_OPTION -1
#define IOCTL_EX1_TEST_UNKNOWN_DEV -2

#define IOCTL_EX1_MAGIC 'k'

struct COMMAND_STRU_T
{
	short op;
	short error;
};

int ioctl_ex1_cmd(struct COMMAND_STRU_T *cmd)
{
	switch(cmd->op)
	{
		case IOCTL_EX1_TEST:
			printk("luo_ioctl_ex1_cmd: this is a test\n");
			cmd->error=IOCTL_EX1_TEST_OK;
			break;
		default:
			printk("luo_ioctl_ex1_cmd: error option\n");
			cmd->error=IOCTL_EX1_TEST_UNKNOWN_OPTION;
			return -1;
	}
	return 0;
}

int luo_ioctl(struct inode *inode, struct file *file, unsigned int opt, unsigned long arg)
{
	int ret_val;
	struct COMMAND_STRU_T *cmd=(struct COMMAND_STRU_T *)kmalloc(sizeof(struct COMMAND_STRU_T), GFP_KERNEL);
	if(!cmd)
	{
		printk("luo_ioctl_ex1_ioctl: failed to allocate spcmd\n");
		goto ioctl_error1;
	}
	if(copy_from_user(cmd, (long *)arg, sizeof(struct COMMAND_STRU_T)))
	{
		printk("luo_ioctl_ex1_ioctl: failed to copy from user\n");
		goto ioctl_error2;
	}
	if(_IOC_TYPE(opt)!=IOCTL_EX1_MAGIC)
	{
		cmd->error=IOCTL_EX1_TEST_UNKNOWN_DEV;
		printk("luo_ioctl_ex1_ioctl: unknown device type\n");
		if(copy_to_user((long *)arg, cmd, sizeof(struct COMMAND_STRU_T)))
			printk("luo_ioctl_ex1_ioctl: failed to copy to user\n");
		goto ioctl_error2;
	}
	ret_val=ioctl_ex1_cmd(cmd);
	if(copy_to_user((long *)arg, cmd, sizeof(struct COMMAND_STRU_T)))
	{
		printk("luo_ioctl_ex1_ioctl: failed to copy to user\n");
		goto ioctl_error2;
	}
	kfree(cmd);
	return 0;
	
	ioctl_error2:
		kfree(cmd);
	ioctl_error1:
		return -1;
}
int sp_major;

int test_init(void)
{
	struct file_operations module_params=
	{
		.owner = THIS_MODULE,
		.compat_ioctl = luo_ioctl,
	};
	sp_major=register_chrdev(0, MODULE_NAME, &module_params);
	if(sp_major<0)
	{
		printk(MODULE_NAME ": failed to initialize\n");
		return -1;
	}
	printk(MODULE_NAME ": initialize ok\n");
	return 0;
}

void test_clean(void)
{
	printk(MODULE_NAME ": quit module\n");
	unregister_chrdev(sp_major, MODULE_NAME);
}

module_init(test_init);
module_exit(test_clean);

MODULE_DESCRIPTION("Only A Test");
MODULE_AUTHOR("Luo Yin");
MODULE_LICENSE("GPL v2");
