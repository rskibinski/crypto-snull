#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x59caa4c3, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xb6b46a7c, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x5d41c87c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0xa1f200db, __VMLINUX_SYMBOL_STR(netpoll_setup) },
	{ 0x90e3ac02, __VMLINUX_SYMBOL_STR(netpoll_print_options) },
	{ 0x5792f848, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0xa8806e07, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0xc25cf0a9, __VMLINUX_SYMBOL_STR(alloc_netdev_mqs) },
	{ 0x6fe1f286, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0x423a11c9, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0xda3e43d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0xd52bf1ce, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x9ecf18e, __VMLINUX_SYMBOL_STR(netif_rx) },
	{ 0x6f957436, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0x5b3a8e4a, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x461d5203, __VMLINUX_SYMBOL_STR(ether_setup) },
	{ 0x68565378, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xdab9e674, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x80d61970, __VMLINUX_SYMBOL_STR(netpoll_send_udp) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x2072ee9b, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "11894B65D253A7190629BE5");
