#ifndef DRIVER_IOCTL_H
#define DRIVER_IOCTL_H

#include <linux/ioctl.h>

#define IOC_MAGIC_READ 0
#define IOC_MAGIC_LIMIT 1
#define IOC_MAGIC_CLEAR_MESSAGE 2

#define SET_READ_MODE _IOW(IOC_MAGIC_READ, 0, int)
#define SET_UNREAD_LIMIT _IOW(IOC_MAGIC_LIMIT, 0, int)
#define CLEAR_MESSAGE_OF _IOW(IOC_MAGIC_CLEAR_MESSAGE, 0, int)

#endif
