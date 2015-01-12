/*
 * crypto_snull.h
 *
 *  Created on: Jan 8, 2015
 *      Author: host1
 */

#ifndef CRYPTO_SNULL_H_
#define CRYPTO_SNULL_H_

/*
 * Macros to help debugging
 */

#undef PDEBUG             /* undef it, just in case */
#ifdef SNULL_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "snull: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) /* nothing: it's a placeholder */


/* These are the flags in the statusword */
#define SNULL_RX_INTR 0x0001
#define SNULL_TX_INTR 0x0002

/* Default timeout period */
#define CRYPTO_TIMEOUT 5   /* In jiffies */

extern struct net_device *snull_devs[];

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
	struct crypto_cipher *tfm;
	spinlock_t lock;
};

int crypto_open(struct net_device*);
int crypto_close(struct net_device*);
static void crypto_rx_interrupts(struct net_device*, int);
void crypto_setup_pool(struct net_device*);
void crypto_teardown_pool(struct net_device*);
int crypto_tx(struct sk_buff*, struct net_device*);
struct net_device_stats* crypto_stats(struct net_device*);
void crypto_rx(struct net_device*, struct crypto_packet*);
static irqreturn_t crypto_interrupt(int, void*);
void crypto_init(struct net_device*);
void crypto_cleanup(void);
int crypto_init_module(void);

#endif /* CRYPTO_SNULL_H_ */
