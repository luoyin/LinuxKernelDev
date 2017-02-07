#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>

using std::cout;
using std::endl;

#define IOCTL_EX1_TEST 0x0
#define IOCTL_EX1_MAGIC 'k'
#define IOCTL_EX1_CMD _IOWR(IOCTL_EX1_MAGIC, 2, struct COMMAND_STRU_T)

struct COMMAND_STRU_T
{
	short op;
	short error;
};

int main()
{
	int fd=open("/dev/luo_ioctl_ex1", O_RDWR);
	if(fd<0)
	{
		cout<<"Error open"<<endl;
		return -1;
	}
	
	COMMAND_STRU_T cmd;
	cmd.op=IOCTL_EX1_TEST;
	if(ioctl(fd, IOCTL_EX1_CMD, (void *)&cmd)<0)
	{
		cout<<"Error ioctl"<<endl;
		cout<<"cmd.error: "<<cmd.error<<endl;
		return -1;
	}
	cout<<"cmd.error: "<<cmd.error<<endl;
	close(fd);
	return 0;
}
