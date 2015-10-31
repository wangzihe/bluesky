#ifndef lint
static char sfs_xdr_mem_id[] = "@(#)xdr_mem.c     2.1     97/10/23";
#endif
/* @(#)xdr_mem.c	2.1 88/07/29 4.0 RPCSRC */
/*
 *   Copyright (c) 1992-1997,2001 by Standard Performance Evaluation Corporation
 *	All rights reserved.
 *		Standard Performance Evaluation Corporation (SPEC)
 *		6585 Merchant Place, Suite 100
 *		Warrenton, VA 20187
 *
 *	This product contains benchmarks acquired from several sources who
 *	understand and agree with SPEC's goal of creating fair and objective
 *	benchmarks to measure computer performance.
 *
 *	This copyright notice is placed here only to protect SPEC in the
 *	event the source is misused in any manner that is contrary to the
 *	spirit, the goals and the intent of SPEC.
 *
 *	The source code is provided to the user or company under the license
 *	agreement for the SPEC Benchmark Suite for this product.
 */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 * 
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 * 
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 * 
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 * 
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)xdr_mem.c 1.19 87/08/11 Copyr 1984 Sun Micro";
#endif

/*
 * xdr_mem.h, XDR implementation using memory buffers.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * If you have some data to be interpreted as external data representation
 * or to be converted to external data representation in a memory buffer,
 * then this is the package for you.
 *
 */

#include <string.h>
#include "rpc/types.h"
#include "rpc/xdr.h"
#include "rpc/osdep.h"

static bool_t	xdrmem_getlong(XDR *, int32_t *);
static bool_t	xdrmem_putlong(XDR *, int32_t *);
static bool_t	xdrmem_getbytes(XDR *, void *, uint_t);
static bool_t	xdrmem_putbytes(XDR *, void *, uint_t);
static uint_t	xdrmem_getpos(XDR *);
static bool_t	xdrmem_setpos(XDR *, uint_t);
static int32_t *xdrmem_inline(XDR *, uint_t);
static void	xdrmem_destroy(XDR *);

static struct	xdr_ops xdrmem_ops = {
	xdrmem_getlong,
	xdrmem_putlong,
	xdrmem_getbytes,
	xdrmem_putbytes,
	xdrmem_getpos,
	xdrmem_setpos,
	xdrmem_inline,
	xdrmem_destroy
};

/*
 * The procedure xdrmem_create initializes a stream descriptor for a
 * memory buffer.  
 */
void
xdrmem_create(
	XDR *xdrs,
	void *addr,
	uint_t size,
	enum xdr_op op)
{

	xdrs->x_op = op;
	xdrs->x_ops = &xdrmem_ops;
	xdrs->x_private = xdrs->x_base = addr;
	xdrs->x_handy = size;
}

/* ARGSUSED */
static void
xdrmem_destroy(XDR *xdrs)
{
}

static bool_t
xdrmem_getlong(
	XDR *xdrs,
	int32_t *lp)
{

	if ((xdrs->x_handy -= sizeof(int32_t)) < 0)
		return (FALSE);
	*lp = (int32_t)ntohl((uint32_t)(*((int32_t *)(xdrs->x_private))));
	xdrs->x_private += sizeof(int32_t);
	return (TRUE);
}

static bool_t
xdrmem_putlong(
	XDR *xdrs,
	int32_t *lp)
{

	if ((xdrs->x_handy -= sizeof(int32_t)) < 0)
		return (FALSE);
	*(int32_t *)xdrs->x_private = (int32_t)htonl((uint32_t)(*lp));
	xdrs->x_private += sizeof(int32_t);
	return (TRUE);
}

static bool_t
xdrmem_getbytes(
	XDR *xdrs,
	void *addr,
	uint_t len)
{

	if ((xdrs->x_handy -= len) < 0)
		return (FALSE);
	memmove(addr, xdrs->x_private, len);
	xdrs->x_private += len;
	return (TRUE);
}

static bool_t
xdrmem_putbytes(
	XDR *xdrs,
	void *addr,
	uint_t len)
{

	if ((xdrs->x_handy -= len) < 0)
		return (FALSE);
	memmove(xdrs->x_private, addr, len);
	xdrs->x_private += len;
	return (TRUE);
}

static uint_t
xdrmem_getpos(
	XDR *xdrs)
{

	return ((uint_t)xdrs->x_private - (uint_t)xdrs->x_base);
}

static bool_t
xdrmem_setpos(
	XDR *xdrs,
	uint_t pos)
{
	char * newaddr = xdrs->x_base + pos;
	char * lastaddr = xdrs->x_private + xdrs->x_handy;

	if ((int32_t)newaddr > (int32_t)lastaddr)
		return (FALSE);
	xdrs->x_private = newaddr;
	xdrs->x_handy = (int)lastaddr - (int)newaddr;
	return (TRUE);
}

static int32_t *
xdrmem_inline(
	XDR *xdrs,
	uint_t len)
{
	int32_t *buf = 0;

	if (xdrs->x_handy >= len) {
		xdrs->x_handy -= len;
		buf = (int32_t *) xdrs->x_private;
		xdrs->x_private += len;
	}
	return (buf);
}