#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <linux/gpio_dev.h>
//#include <linux/ioctl.h>

void
print_usage()
{
	printf("stx10-server [PORT]\n");
	exit(0);
}

int
main(int argc, char **argv)
{
	int gpio_pin;
	int fd;
	int result = 0;

	if (argc != 3)
	{
		print_usage();
	}

	if ((fd = open("/dev/gpio", O_RDWR)) < 0)
	{
        printf("Error whilst opening /dev/gpio\n");
        return -1;
	}

	gpio_pin = atoi(argv[2]);

	printf("using gpio pin %d\n", gpio_pin);

	//if (!strcmp(argv[1], "dirin"))
	//{
		//ioctl(fd, GPIO_DIR_IN, gpio_pin);
	//} else if (!strcmp(argv[1], "dirout"))
	//{
		//ioctl(fd, GPIO_DIR_OUT, gpio_pin);
	//} else print_usage();

	return result;
}
