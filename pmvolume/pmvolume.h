/*
#include <linux/list.h>
#include <linux/module.h>
#include <linux/mmzone.h>
*/
//pm设备
struct pm_device{
	char *name;
	struct list_head list;
	unsigned long start;
	unsigned int size;
	int device_no;  //pm_device在相应的pm_volume中的编号
	int connection_type;  //连接类型
	int synchronize_type;   //同步类型
};

//pm卷
struct pm_volume{
	
	//struct pmzone PM;//新增的关于内存的zone
	
	struct list_head list;
	
	int supported_modes;
	int file_mode;	
	int interrupted_store_atomicity;
	int fundamental_error_range;
	int fundamental_error_range_offset;
	int discard_if_you_can_capable;
	int discard_immediately_capable;
	int discard_immediately_returns;
	int exists_capable;

	unsigned long start;
	int volume_no;       //pm_volume编号
	int volume_size;
	int num;   //包含的设备数
	struct zone* zone;
	struct pm_device *head; //
	//struct list_head list;  
};

//struct pm_volume *pmv;

//unsigned long (*vol_dev)(unsigned long, struct pm_volume*);

