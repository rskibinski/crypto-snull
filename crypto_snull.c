/*
 * crypto_snull.c
 *
 *  Created on: Jan 8, 2015
 *      Author: host1
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/interrupt.h> /* mark_bh */

#include <linux/in.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/tcp.h>         /* struct tcphdr */
#include <linux/skbuff.h>

#include "crypto_snull.h"

#include <linux/in6.h>
//#include <asm-generic/checksum.h>
MODULE_LICENSE("Dual BSD/GPL");
#include <linux/netpoll.h>
int pool_size = 16;
module_param(pool_size, int, 0)
;

int timeout = 5;
module_param(timeout, int, 0)
;
// NETPOOL
char *tun_device = "eth0";
module_param(tun_device, charp, 0000)
;

char *remote_address = "179.0.1.129";
module_param(remote_address, charp, 0000)
;

char *local_address = "179.0.1.128";
module_param(local_address, charp, 0000)
;

int rx_port = 6666;
int tx_port = 6665;

static struct netpoll* np = NULL;
static struct netpoll np_t;
// END OF NETPOOL

unsigned int inet_addr(char *str) {
	int a, b, c, d;
	char arr[4];
	sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
	arr[0] = a;
	arr[1] = b;
	arr[2] = c;
	arr[3] = d;
	return *(unsigned int*) arr;
}

struct net_device *crypto_dev;

struct crypto_packet {
	struct crypto_packet *next;
	struct net_device *dev;
	int datalen;
	u8 data[ETH_DATA_LEN];
};

struct crypto_priv {
	struct net_device *dev;
	struct net_device_stats stats;
	int status;
	struct crypto_packet *ppool;
	struct crypto_packet *rx_queue;
	int rx_int_enabled;//TODO Why?
	int tx_packetlen;
	u8 *tx_packetdata; // data to transmit yet?
	struct sk_buff *skb;//to relase memory?
	spinlock_t lock;
};

int crypto_open(struct net_device *dev) {
	int retval;

	int retval = request_irq(dev->irq, )

	memcpy(dev->dev_addr, "\0CRYPTO", ETH_ALEN); //TODO Check if it matters in vint if its corect
	netif_start_queue(dev);//start passing packets to upper layers
	return 0;//TODO: Check result of above?
}

int crypto_close(struct net_device *dev) {
	netif_stop_queue(dev); //stop passing packets to upper layers
	return 0;
}

static void crypto_rx_interrupts(struct net_device *dev, int enable) {
	struct crypto_priv *priv = netdev_priv(dev);
	priv->rx_int_enabled = enable;
}

void crypto_setup_pool(struct net_device *dev) {
	struct crypto_priv *priv = netdev_priv(dev);
	int i;
	struct crypto_packet *packet;

	priv->ppool = NULL;
	for (i = 0; i < pool_size; i++) {
		packet = kmalloc(sizeof(struct crypto_packet), GFP_KERNEL);
		if (packet == NULL) {
			printk(KERN_NOTICE "Out of memory on kmalloc packet pool\n");
			return;
		}
		packet->dev = dev;
		packet->next = priv->ppool;
		priv->ppool = packet;
	}

}

void crypto_teardown_pool(struct net_device *dev) {
	struct crypto_priv *priv = netdev_priv(dev);
	struct crypto_packet *pkt;

	while ((pkt = priv->ppool)) {
		priv->ppool = pkt->next;
		kfree(pkt);
		/* FIXME - in-flight packets ? */
	}
}

int crypto_tx(struct sk_buff *skb, struct net_device *dev) {
	int len;
	char *data, shortpkt[ETH_ZLEN];//TODO: Whats shortpkt
	struct crypto_priv *priv = netdev_priv(dev);

	data = skb->data;
	len = skb->len;

	printk(KERN_DEBUG "DATA SENDING %d: %s\n", len, data);
	dev->trans_start = jiffies;
	netpoll_send_udp(np, data, len);


	return 0;
}

struct net_device_stats *crypto_stats(struct net_device *dev) {
	struct crypto_priv *priv = netdev_priv(dev);
	return &priv->stats;
}

static const struct net_device_ops crypto_netdev_ops = { .ndo_open =
		crypto_open, .ndo_stop = crypto_close, .ndo_start_xmit = crypto_tx,
		.ndo_get_stats = crypto_stats, };

static const struct header_ops crypto_header_ops = {

};

void crypto_init(struct net_device *dev) {
	//initialize private fields
	struct crypto_priv *priv;
	priv = netdev_priv(dev);//must do via a function call;
	memset(priv, 0, sizeof(struct crypto_priv));
	spin_lock_init(&priv->lock);
	priv->dev = dev;
#if 0 //TODO
	/*
	 * Make the usual checks: check_region(), probe irq, ...  -ENODEV
	 * should be returned if no device found.  No resource should be
	 * grabbed: this is done on open().
	 */
#endif

	//call ethernet dev setup
	ether_setup(dev);

	dev->watchdog_timeo = timeout;
	dev->features |= NETIF_F_HW_CSUM;/* Can checksum all the packets. */
	dev->netdev_ops = &crypto_netdev_ops;
	dev->header_ops = &crypto_header_ops;
	//TODO ppp flag?

	crypto_rx_interrupts(dev, 1);//enable rx interrupts TODO check if needed on regular ints handling
	crypto_setup_pool(dev);
}


void crypto_cleanup(void) {
	if (crypto_dev) {
		unregister_netdev(crypto_dev);
		crypto_teardown_pool(crypto_dev);
		free_netdev(crypto_dev);
	}
	return;
}

void crypto_rx(struct net_device *dev, struct crypto_packet *pkt){
	struct sk_buff *skb;
	struct crypto_priv *priv = netdev_priv(dev);

	skb = dev_alloc_skb(pkt->datalen +2);
	skb_reserve(skb,2);
	skb->dev = dev;
	skb->protocol = eth_type_trans(skb, dev);
	skb->ip_summed = CHECKSUM_UNNECESSARY; //TODO Check cheksum
	priv->stats.rx_packets++;
	priv->stats.rx_bytes += pkt->datalen;
	netif_rx(skb);
	out: return;
}

static void crypto_rx_interrupt(int rq, void *dev_id, struct pt_regs *regs){
	int statusword;
	struct crypto_priv *priv;
	struct crypto_packet *pkt = NULL;

	struct net_device *dev= (struct net_device *)dev_id;
	if(!dev){
		printk(KERN_DEBUG "crypto_snull: dev null\n");
	}
	priv = netdev_priv(dev);
	spin_lock(&priv->lock);

	statusword = priv->status;
	priv->status = 0;
	if (statusword & SNULL_RX_INTR){
		printk(KERN_DEBUG "crypto_snull: rx interruption\n");
		pkt = priv->rx_queue;
		if(pkt){
			priv->rx_queue = pkt->next;
			crypto_rx(dev, pkt);
		}
	}
	if (statusword & SNULL_TX_INTR){
		printk(KERN_DEBUG "crypto_snull: tx interruption\n");
	}
	spin_unlock(&priv->lock);
	//TODO: relase buffer
	return;

}

//static irqreturn_t net

int crypto_init_module(void) {
	int result, ret = -ENOMEM; // TODO Why?

	crypto_dev
			= alloc_netdev(sizeof(struct crypto_priv), "cryp%d", crypto_init);
	if (crypto_dev == NULL)
		goto out;

	ret = -ENODEV;
	if ((result = register_netdev(crypto_dev))) {
		printk("crypto_snull: Error %i registering net device %s \n", result,
				crypto_dev->name);
	} else {
		ret = 0;
	}

	np_t.name = "LRNG";
	strlcpy(np_t.dev_name, tun_device, IFNAMSIZ);
	np_t.local_ip.ip = inet_addr(local_address);
	np_t.remote_ip.ip = inet_addr(remote_address);
	np_t.local_port = rx_port;
	np_t.remote_port = tx_port;
	memset(np_t.remote_mac, 0xff, ETH_ALEN);
	netpoll_print_options(&np_t);
	netpoll_setup(&np_t);
	np = &np_t;
	if (np == NULL) {
		ret = -1;
		printk("crypto_snull: Error while setting netpool\n");
		goto out;
	}

	out: if (ret)
		crypto_cleanup();
	return ret;
}

module_init( crypto_init_module);
module_exit( crypto_cleanup);

