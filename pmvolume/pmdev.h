#include <linux/ioctl.h>

/*struct pm_device{
	char *name;
	struct list_head list;
	unsigned long start;
	int size;
	int device_no;  //pm_device在相应的pm_volume中的编号
	int connection_type;  //连接类型
	int synchronize_type;   //同步类型
};*/

//卷参数结构体
struct ioc_val{
	int volume_no;  //卷编号
	int value;    //值
};

//设备参数结构体
struct device_val{
	unsigned long start;
	int size;
	int device_no;  //pm_device在相应的pm_volume中的编号
	int connection_type;  //连接类型
	int synchronize_type;   //同步类型
};

struct vol_val{
	int *device_no;
	unsigned int *device_size;
	int *device_start;
	unsigned int device_num;
	int volume_no;
	unsigned long start;
};
//suoyoushebei
struct all_device_val{
	int name;
	int size;
};

/*定义幻数*/
#define PMDEV_IOC_MAGIC 'c'
/*定义序数*/
#define PMDEV_MAX_NR 25
/*定义命令(_IO：设置参数)*/
#define SUPPORTED_MODES_SET _IOW(PMDEV_IOC_MAGIC,1,struct ioc_val)
#define VOLUME_SIZE_SET _IOW(PMDEV_IOC_MAGIC,2,struct ioc_val)
#define INTERRUPTED_STORE_ATOMICITY_SET _IOW(PMDEV_IOC_MAGIC,3,struct ioc_val)
#define FUNDAMENTAL_ERROR_RANGE_SET _IOW(PMDEV_IOC_MAGIC,4,struct ioc_val)
#define FUNDAMENTAL_ERROR_RANGE_OFFSET_SET _IOW(PMDEV_IOC_MAGIC,5,struct ioc_val)
#define DISCARD_IF_YOU_CAN_CAPABLE_SET _IOW(PMDEV_IOC_MAGIC,6,struct ioc_val)
#define DISCARD_IMMEDIATELY_CAPABLE_SET _IOW(PMDEV_IOC_MAGIC,7,struct ioc_val)
#define DISCARD_IMMEDIATELY_RETURNS_SET _IOW(PMDEV_IOC_MAGIC,8,struct ioc_val)
#define EXISTS_CAPABLE_SET _IOW(PMDEV_IOC_MAGIC,9,struct ioc_val)
/*定义命令(_IO：获取命令)*/
#define SUPPORTED_MODES_GET _IOR(PMDEV_IOC_MAGIC,10,struct ioc_val)
#define FILE_MODE_GET _IOR(PMDEV_IOC_MAGIC,11,struct ioc_val)
#define VOLUME_SIZE_GET _IOR(PMDEV_IOC_MAGIC,12,struct ioc_val)
#define INTERRUPTED_STORE_ATOMICITY_GET _IOR(PMDEV_IOC_MAGIC,13,struct ioc_val)
#define FUNDAMENTAL_ERROR_RANGE_GET _IOR(PMDEV_IOC_MAGIC,14,struct ioc_val)
#define FUNDAMENTAL_ERROR_RANGE_OFFSET_GET _IOR(PMDEV_IOC_MAGIC,15,struct ioc_val)
#define DISCARD_IF_YOU_CAN_CAPABLE_GET _IOR(PMDEV_IOC_MAGIC,16,struct ioc_val)
#define DISCARD_IMMEDIATELY_CAPABLE_GET _IOR(PMDEV_IOC_MAGIC,17,struct ioc_val)
#define DISCARD_IMMEDIATELY_RETURNS_GET _IOR(PMDEV_IOC_MAGIC,18,struct ioc_val)
#define EXISTS_CAPABLE_GET _IOR(PMDEV_IOC_MAGIC,19,struct ioc_val)

/*获取构成卷的设备的信息*/
#define GET_RANGESET _IOR(PMDEV_IOC_MAGIC,20,struct device_val)
#define CREATEVOLUME _IOW(PMDEV_IOC_MAGIC,21,struct vol_val)
#define CREATEDEVICE _IOW(PMDEV_IOC_MAGIC,22,int)
#define DELETEVOLUME _IOW(PMDEV_IOC_MAGIC,23,int)

#define GET_DEVICE_DETAIL _IOR(PMDEV_IOC_MAGIC,24,struct all_device_val)
#define RAMDISK _IOW(PMDEV_IOC_MAGIC,25,int)   //ramdisk
