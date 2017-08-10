#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x3de77a2a, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x44858c43, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0xaa93148a, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xa4035bf4, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x6a24cdb7, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x6729d3df, __VMLINUX_SYMBOL_STR(__get_user_4) },
	{ 0xbdac0735, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x8091f63, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xff7ceaa5, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x34b5dc5d, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x18515b4a, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x6f5741a3, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0xc811615c, __VMLINUX_SYMBOL_STR(put_disk) },
	{ 0xde0344c0, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0xab87f061, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x38dd0ec8, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x504212b7, __VMLINUX_SYMBOL_STR(add_disk) },
	{ 0x17011ad, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x76b90156, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xf539908d, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0x4719bb72, __VMLINUX_SYMBOL_STR(blk_queue_make_request) },
	{ 0x90b92ca6, __VMLINUX_SYMBOL_STR(blk_alloc_queue) },
	{ 0xa71f79ee, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0x63b03564, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "5E22F8C8ADEFF93B5F4D8C7");
