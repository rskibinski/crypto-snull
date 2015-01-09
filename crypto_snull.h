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


#endif /* CRYPTO_SNULL_H_ */
