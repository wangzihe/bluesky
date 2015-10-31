#ifndef lint
static char sfs_3_opsSid[] = "@(#)sfs_3_ops.c	2.1	97/10/23";
#endif

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

/*****************************************************************
 *                                                               *
 *	Copyright 1991,1992  Legato Systems, Inc.                *
 *	Copyright 1991,1992  Auspex Systems, Inc.                *
 *	Copyright 1991,1992  Data General Corporation            *
 *	Copyright 1991,1992  Digital Equipment Corporation       *
 *	Copyright 1991,1992  Interphase Corporation              *
 *	Copyright 1991,1992  Sun Microsystems, Inc.              *
 *                                                               *
 *****************************************************************/

/*
 * ---------------------- sfs_3_ops.c ---------------------
 *
 *      RPC routines to implement the NFS protocol.
 *
 *.Local Routines
 *	int op_null(void)
 *	int op_getattr(void)
 *	int op_setattr(int)
 *	int op_lookup(void)
 *	int op_access(void)
 *	int op_readlink(void)
 *	int op_read(int)
 *	int op_write(int, int, stable_how)
 *	int op_create(void)
 *	int op_mkdir(void);
 *	int op_symlink(void);
 *	int op_mknod(void);
 *	int op_remove(void);
 *	int op_rmdir(void);
 *	int op_rename(void);
 *	int op_link(void);
 *	int op_readdir(void);
 *	int op_readdirplus(void);
 *	int op_fsstat(void);
 *	int op_fsinfo(void);
 *	int op_pathconf(void);
 *	int op_commit(void);
 *
 *.Revision_History
 *	30-Jun-94	ChakChung Ng	Created.
 */


/*
 * -------------------------  Include Files  -------------------------
 */

/*
 * ANSI C headers
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#include "sfs_c_def.h"

/*
 * --------------------  Local NFS ops function --------------------
 */
static int op_null(void);
static int op_getattr(void);
static int op_setattr(int);
static int op_lookup(void);
static int op_access(void);
static int op_readlink(void);
static int op_read(int);
static int op_write(int, int, stable_how);
static int op_create(void);
static int op_mkdir(void);
static int op_symlink(void);
static int op_mknod(void);
static int op_remove(void);
static int op_rmdir(void);
static int op_rename(void);
static int op_link(void);
static int op_readdir(void);
static int op_readdirplus(void);
static int op_fsstat(void);
static int op_fsinfo(void);
static int op_pathconf(void);
static int op_commit(void);
static int op_nosys(void);
static char *nfs3_strerror(int);


/*
 * --------------------  NFS ops vector --------------------
 */
/*
 * per operation information
 */
static sfs_op_type nfsv3_Ops[] = {

/* name        mix   function         op    call  no  req  req  req  results */
/*             pcnt                  class  targ call pcnt cnt  targ         */

 { "null",        0, op_null,        Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "getattr",    11, op_getattr,     Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "setattr",     1, op_setattr,     Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "root",        0, op_nosys,       Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "lookup",     27, op_lookup,      Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "readlink",    7, op_readlink,    Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "read",       18, op_read,        Read,    0,  0,  0.0,  0,   0,  { 0, }},
 { "wrcache",     0, op_nosys,       Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "write",       9, op_write,       Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "create",      1, op_create,      Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "remove",      1, op_remove,      Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "rename",      0, op_rename,      Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "link",        0, op_link,        Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "symlink",     0, op_symlink,     Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "mkdir",       0, op_mkdir,       Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "rmdir",       0, op_rmdir,       Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "readdir",     2, op_readdir,     Read,    0,  0,  0.0,  0,   0,  { 0, }},
 { "fsstat",      1, op_fsstat,	     Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "access",      7, op_access,      Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "commit",      5, op_commit,      Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "fsinfo",      1, op_fsinfo,      Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "mknod",       0, op_mknod,       Write,   0,  0,  0.0,  0,   0,  { 0, }},
 { "pathconf",    0, op_pathconf,    Lookup,  0,  0,  0.0,  0,   0,  { 0, }},
 { "readdirplus", 9, op_readdirplus, Read,   0,  0,  0.0,  0,   0,  { 0, }},
 { "TOTAL",     100, 0,              Lookup,  0,  0,  0.0,  0,   0,  { 0, }}
};

sfs_op_type *Ops;

/*
 * --------------------  RPC routines for NFS protocol --------------------
 */

void
init_ops(void)
{
    Ops = nfsv3_Ops;
    nfs_version = NFS_V3;
}

/*
 * The routines below attempt to do over-the-wire operations.
 * Each op tries to cause one or more of a particular
 * NFS operation to go over the wire.  OPs return the success
 * of their NFS call(s).  Each OP records how many calls it
 * actually made in global data.
 *
 * An array of file information is kept for files existing in
 * the test directory.  File handles, attributes, names, etc
 * are stored in this array.
 *
 */

/*
 * Generic catch all for operations not covered by this protocol.
 */
static int
op_nosys(int i)
{
	RFS_ASSERT (dep_tab[i].flag == TO_BE_SENT);
	dep_tab[i].flag = DONE;
    Ops[TOTAL].results.bad_calls++;
	if (i==min_index)
		adjust_min_index ();
    return(0);
}

static struct biod_req *
get_biod_reqp (int dep_tab_index)
{
	static int index = 0;
	int i;

	for (i=0; i<max_biod_reqs; i++, index = (index+1)%max_biod_reqs) {
		if (biod_reqp[index].in_use == FALSE) {
			biod_reqp[index].in_use = TRUE;
			dep_tab[dep_tab_index].biod_index = index;
			biod_req[index].dep_tab_index = dep_tab_index;
			return (biod_reqp+index);
		}
	}
}

static int
op_null(int i)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int 		ret;		/* ret val == call success */

	struct biod_req * reqp;
	struct ladtime call_timeout;

	if (dep_tab[i].flag == SENT)
		goto RECEIVE_REPLY;

    op_ptr = &Ops[NULLCALL];
	reqp = get_biod_reqp(i);

	call_timeout.sec = Nfs_timers[op_ptr->call_class].tv_sec;
	call_timeout.usec = Nfs_timers[op_ptr->call_class].tv_usec;

    ret = 0;

    /* make the call */
    sfs_gettime(&reqp->start);
	reqp->xid = boid_clnt_call(NFS_client, NFS3PROC_NULL, xdr_void, (char *)0);
	if (reqp->xid != 0) {
		reqp->timeout = reqp->start;
		ADDTIME (reqp->timeout, call_timeout);
		num_out_reqs++;
		dep_tab[i].flag = SENT;
	} else 
		RFS_ASSERT (0);

	return 0;

RECEIVE_REPLY:
    rpc_stat = clnt_call(NFS_client, NFSPROC3_NULL,
			xdr_void, (char *)0, xdr_void, (char *)0,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: null_op call RPC error %d\n",
						sfs_Myname, rpc_stat);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_null */


static int
op_getattr(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    GETATTR3args	args;		/* fh to do op on */
    GETATTR3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[GETATTR];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.object, (char *) &Cur_file_ptr->fh3,
							sizeof (nfs_fh3));

    /* make the call */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_GETATTR,
			xdr_GETATTR3args, (char *) &args,
			xdr_GETATTR3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
				(char *) &reply.resok.obj_attributes,
				sizeof (Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: getattr call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr,
			"%s: getattr call RPC error %d on file %d\n",
		    	sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_getattr */


/*
 * perform an RPC setattr operation.  If 'truncate_size' is non-negative,
 * truncate the file to that size.
 */
static int
op_setattr(
    int		truncate_size)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    SETATTR3args	args;
    SETATTR3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[SETATTR];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.object, (char *) &Cur_file_ptr->fh3,
							sizeof (nfs_fh3));
    args.new_attributes.mode.set_it = TRUE;
    args.new_attributes.mode.mode = (uint32_t) 0666;
    args.new_attributes.uid.set_it = FALSE;
    args.new_attributes.uid.uid = (uint32_t) -1;
    args.new_attributes.gid.set_it = FALSE;
    args.new_attributes.gid.gid = (uint32_t) -1;
    args.new_attributes.size.set_it = FALSE;
    args.new_attributes.size.size._p._u = (uint32_t) ~0;
    args.new_attributes.size.size._p._l = (uint32_t) -1;
    args.new_attributes.atime.set_it = TRUE;
    args.new_attributes.atime.atime.seconds = Cur_time.esec;
    args.new_attributes.atime.atime.nseconds = Cur_time.usec * 1000;
    args.new_attributes.mtime.set_it = TRUE;
    args.new_attributes.mtime.mtime.seconds = Cur_time.esec;
    args.new_attributes.mtime.mtime.nseconds = Cur_time.usec * 1000;
    args.guard.check = FALSE;

    /* handle file truncations */
    if (truncate_size >= 0) {
	args.new_attributes.size.set_it = TRUE;
	args.new_attributes.size.size._p._u = (uint32_t) 0;
	if (truncate_size > Cur_file_ptr->attributes3.size._p._l)
	    args.new_attributes.size.size._p._l = (uint32_t) 0;
	else
	    args.new_attributes.size.size._p._l =
		(uint32_t) Cur_file_ptr->attributes3.size._p._l -
								truncate_size;
    }

    /* make the call */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_SETATTR,
			xdr_SETATTR3args, (char *) &args,
			xdr_SETATTR3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);

    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
				(char *) &reply.resok.obj_wcc.after.attr,
				sizeof (Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: setattr call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr,
			"%s: setattr call RPC error %d on file %d\n",
			sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_setattr */


static int
op_lookup(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    LOOKUP3args		args;
    LOOKUP3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[LOOKUP];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.what.dir, (char *) &Cur_file_ptr->dir->fh3,
							sizeof (nfs_fh3));
    args.what.name = Cur_filename;
    (void) memset((char *) &reply.resok.object, '\0', sizeof (nfs_fh3));

    /* make the call */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_LOOKUP,
			xdr_LOOKUP3args, (char *) &args,
			xdr_LOOKUP3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Exists;
	    (void) memmove((char *) &Cur_file_ptr->fh3,
			(char *) &reply.resok.object,
			sizeof (nfs_fh3));
	    (void) strcpy(Cur_file_ptr->file_name, Cur_filename);
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
				(char *) &reply.resok.obj_attributes.attr,
				sizeof (Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    /* We do lookup Nonexistent and this is not an error */
	    if (reply.status != NFS3ERR_NOENT ||
			Cur_file_ptr->state != Nonexistent) {
	        if (DEBUG_CHILD_ERROR) {
		     (void) fprintf(stderr,
			"%s: lookup call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	        }
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: lookup call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_lookup */


static int
op_access(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    ACCESS3args		args;
    ACCESS3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[ACCESS];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.object, (char *) &Cur_file_ptr->dir->fh3,
				sizeof (nfs_fh3));
    args.access = ACCESS3_MODIFY;

    /* make the call */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_ACCESS,
			xdr_ACCESS3args, (char *) &args,
			xdr_ACCESS3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Exists;
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
				(char *) &reply.resok.obj_attributes.attr,
				sizeof (Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: access call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: access call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_access */


static int
op_readlink(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    READLINK3args	args;			/* the args */
    READLINK3res	reply;			/* the reply */
    char		sym_data[NFS_MAXPATHLEN];
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */

    op_ptr = &Ops[READLINK];
    ret = 0;

    /* set up the arguments */
    /*
     * Note: this symlink may be bogus because SYMLINK does
     * not return a symlink ... only a status.  So unless we have
     * done a LOOKUP on this guy, the symlink will probably be bad.
     * If it is bad it shows up as a symlink error in the results.
     */
    (void) memmove((char *) &args.symlink,
			(char *) &Cur_file_ptr->fh3,
			sizeof (nfs_fh3));

    /* Have lower layers fill in the data directly. */
    reply.resok.data = sym_data;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_READLINK,
			xdr_READLINK3args, (char *) &args,
			xdr_READLINK3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    if (DEBUG_CHILD_RPC) {
		(void) fprintf(stderr, "%s: READLINK on %s returned %s\n",
				    sfs_Myname, Cur_filename, sym_data);
		(void) fflush(stderr);
	    }
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: readlink call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr,
			"%s: readlink call RPC error %d on file %d\n",
			sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_readlink */


/*
 * perform an RPC read operation of length 'xfer_size'
 */
static int
op_read(
    int 			xfer_size)
{
    sfs_op_type			*op_ptr;	/* per operation info */
    int				cur_cnt;
    int				max_cnt;	/* packet ctrs */
    char			buf[DEFAULT_MAX_BUFSIZE];/* data buffer */
    READ3args			args;
    READ3res			reply;		/* the reply */
    enum clnt_stat		rpc_stat;	/* result from RPC call */
    struct ladtime		start;
    struct ladtime		stop;
    int				size;
    int				j;
    int				ret;		/* ret val == call success */

    op_ptr = &Ops[READ];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.file,
			(char *) &Cur_file_ptr->fh3,
			sizeof (nfs_fh3));

    /*
     * Don't allow a read of less than one block size
     */
    if (xfer_size < Bytes_per_block)
	xfer_size = Bytes_per_block;

    /*
     * randomly choose an offset that is a multiple of the block size
     * and constrained by making the transfer fit within the file
     */
    args.offset._p._u = 0;
    if (Cur_file_ptr->attributes3.size._p._l > xfer_size)
	args.offset._p._l = Bytes_per_block * (sfs_random() %
			(((Cur_file_ptr->attributes3.size._p._l - xfer_size)
			/ Bytes_per_block) + 1));
    else
	args.offset._p._l = 0;

    /* Have lower layers fill in the data directly.  */
    reply.resok.data.data_len = 0;
    reply.resok.data.data_val = buf;

    /* first read the whole buffers, then the fragment */
    for (j = 0; j < 2; j++) {

	if (j == 0) {
	    size = Bytes_per_block;
	    max_cnt = xfer_size / Bytes_per_block;
	} else {
	    /* 1KB - (Kb_per_block -1) KB fragment */
	    size = xfer_size % Bytes_per_block;
	    max_cnt = 1;
	}
	if (size == 0)
	    continue;

	/* check our stats to see if this would overflow */
	if (!Timed_run) {
	    if (op_ptr->target_calls > 0) {
		if ((op_ptr->results.good_calls + max_cnt)
		     > op_ptr->target_calls) {
		    max_cnt = op_ptr->target_calls - op_ptr->results.good_calls;
		}
	    }
	}

	args.count = size;

	if (DEBUG_CHILD_RPC) {
	    (void) fprintf(stderr, "read: %d buffers\n", max_cnt);
	    (void) fflush(stderr);
	}

	/* make the call(s) now */
	for (cur_cnt = 0; cur_cnt < max_cnt; cur_cnt++) {

	    /* capture length for possible dump */
	    Dump_length = fh_size(Cur_file_ptr);
 
	    sfs_gettime(&start);
	    rpc_stat = clnt_call(NFS_client, NFSPROC3_READ,
				xdr_READ3args, (char *) &args,
				xdr_READ3res, (char *) &reply,
				(Current_test_phase < Warmup_phase)
				     ? Nfs_timers[Init]
				     : Nfs_timers[op_ptr->call_class]);
	    sfs_gettime(&stop);
	    Cur_time = stop;

	    /* capture count and offset for possible dump */
	    Dump_count = (rpc_stat == RPC_SUCCESS && reply.status == NFS3_OK)
			    ? reply.resok.data.data_len : 0;
	    Dump_offset = args.offset._p._l;

	    if (rpc_stat == RPC_SUCCESS) {
		if (reply.status == NFS3_OK) {
		    Cur_file_ptr->state = Exists;
		    (void) memmove((char *) &Cur_file_ptr->attributes3,
				(char *) &reply.resok.file_attributes.attr,
				sizeof (Cur_file_ptr->attributes3));
		    Cur_file_ptr->size = fh_size(Cur_file_ptr);
		    size = reply.resok.data.data_len;

		    if (DEBUG_CHILD_RPC) {
			(void) fprintf(stderr, "%s: READ %s %d bytes\n",
					   sfs_Myname, Cur_filename, size);
			(void) fflush(stderr);
		    }
		    args.offset._p._l += size;
		} else {
		    if (DEBUG_CHILD_ERROR) {
			 (void) fprintf(stderr,
				"%s: read call NFS error %s on file %d\n",
					sfs_Myname,
					nfs3_strerror(reply.status),
					Cur_file_ptr->unique_num);
		    }
		}
		sfs_elapsedtime(op_ptr, &start, &stop);
		op_ptr->results.good_calls++;
		Ops[TOTAL].results.good_calls++;
		ret++;
	    } else {
		if (DEBUG_CHILD_ERROR) {
		     (void) fprintf(stderr,
				"%s: read call RPC error %d on file %d\n",
			    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
		}
		op_ptr->results.bad_calls++;
		Ops[TOTAL].results.bad_calls++;
	    }
	} /* for reading max_cnt packets */
    } /* for buffers and fragments */
    return(ret);

} /* op_read */

char *
init_write_buffer(
    void)
{
    uint32_t *bp;
    static uint32_t write_buf[DEFAULT_MAX_BUFSIZE / sizeof(uint32_t)];
    uint32_t *be  = write_buf + (sizeof(write_buf) /
							sizeof(uint32_t));

    if (write_buf[0] != (uint32_t)0xdeadbeef) {
        for (bp = write_buf; bp < be; bp++)
            *bp = (uint32_t)0xdeadbeef;
    }
    return (char *)write_buf;
}


/*
 * Perform and RPC write operation of length 'xfer_size'.  If 'append_flag'
 * is true, then write the data to the end of the file.
 *
 * If the stab_flag is set to UNSTABLE we issue the requests and then
 * issue a op_commit to sync the data.
 */
static int
op_write(
    int			xfer_size,
    int			append_flag,
    stable_how		stab_flag)
{
    sfs_op_type			*op_ptr;	/* per operation info */
    static char			*buf = NULL;	/* the data buffer */
    unsigned int		size;		/* size of data write */
    int				cur_cnt;	/* controls # NFS calls */
    int				max_cnt;
    WRITE3args			args;
    WRITE3res			reply;		/* the reply */
    enum clnt_stat		rpc_stat;	/* result from RPC call */
    struct ladtime		start;
    struct ladtime		stop;
    int				j;
    int				ret;		/* ret val == call success */

    /*
     * For now we treat DATA_SYNC to be the same as FILE_SYNC.
     */
    if (stab_flag == DATA_SYNC)
	stab_flag = FILE_SYNC;

    /*
     * Initialize write buffer to known value
     */
    if (buf == NULL) {
	buf = init_write_buffer();
    }

    op_ptr = &Ops[WRITE];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.file, (char *) &Cur_file_ptr->fh3,
		sizeof (nfs_fh3));
    args.offset._p._u = 0;
    if (append_flag == 1) {
	args.offset._p._l = Cur_file_ptr->attributes3.size._p._l;
    } else {
	/*
	 * randomly choose an offset that is a multiple of the block size
	 * and constrained by making the transfer fit within the file
	 */
	if (Cur_file_ptr->attributes3.size._p._l > xfer_size) {
	    args.offset._p._l = Bytes_per_block * (sfs_random() %
			    (((Cur_file_ptr->attributes3.size._p._l - xfer_size)
			    / Bytes_per_block) + 1));
	} else
	    args.offset._p._l = 0;
    }

    /* stab_flag has to be set in op() in sfs_3_chd.c */
    args.stable = stab_flag;

    /* first write the whole buffers, then the fragment */
    for (j = 0; j < 2; j++) {

	if (j == 0) {
	    size = Bytes_per_block;
	    max_cnt = xfer_size / Bytes_per_block;
	} else {
	    /* 1KB - (Kb_per_block - 1) KB fragment */
	    size = xfer_size % Bytes_per_block;
	    max_cnt = 1;
	}
	if (size == 0)
	    continue;

	args.count = size;
	args.data.data_len = size;
	args.data.data_val = buf;

	/* check our stats to see if this would overflow */
	if (!Timed_run) {
	    if (op_ptr->target_calls > 0) {
		if ((op_ptr->results.good_calls + max_cnt)
		     > op_ptr->target_calls) {
		    max_cnt = op_ptr->target_calls - op_ptr->results.good_calls;
		}
	    }
	}

	if (DEBUG_CHILD_RPC) {
	    (void) fprintf(stderr, "write: %d buffers\n", max_cnt);
	    (void) fflush(stderr);
	}

	/* make the call(s) now */
	for (cur_cnt = 0; cur_cnt < max_cnt; cur_cnt++) {

	    if (DEBUG_CHILD_RPC) {
(void) fprintf(stderr, "%s: WRITE %s offset %u count %lu stable %d\n",
sfs_Myname, Cur_filename, args.offset._p._l, args.count, args.stable);
		(void) fflush(stderr);
	    }

	    /* capture length for possible dump */
	    Dump_length = fh_size(Cur_file_ptr);
 
	    sfs_gettime(&start);
	    rpc_stat = clnt_call(NFS_client, NFSPROC3_WRITE,
				xdr_WRITE3args, (char *) &args,
				xdr_WRITE3res, (char *) &reply,
				(Current_test_phase < Warmup_phase)
				     ? Nfs_timers[Init]
				     : Nfs_timers[op_ptr->call_class]);
	    sfs_gettime(&stop);
	    Cur_time = stop;

	    /* capture count and offset for possible dump */
	    Dump_count = args.data.data_len;
	    Dump_offset = args.offset._p._l;

	    if (rpc_stat == RPC_SUCCESS) {
		if (reply.status == NFS3_OK) {
		    Cur_file_ptr->state = Exists;
		    (void) memmove((char *) &Cur_file_ptr->attributes3,
				(char *) &reply.resok.file_wcc.after.attr,
				sizeof (Cur_file_ptr->attributes3));
		    Cur_file_ptr->size = fh_size(Cur_file_ptr);
		    args.offset._p._l += size;

		    if (DEBUG_CHILD_RPC) {
			(void) fprintf(stderr, "%s: WRITE %s %d bytes\n",
					   sfs_Myname, Cur_filename, size);
			(void) fflush(stderr);
		    }
		} else {
		    if (DEBUG_CHILD_ERROR) {
			 (void) fprintf(stderr,
				"%s: write call NFS error %s on file %d\n",
				sfs_Myname, nfs3_strerror(reply.status),
				Cur_file_ptr->unique_num);
		    }
		}
		sfs_elapsedtime(op_ptr, &start, &stop);
		op_ptr->results.good_calls++;
		Ops[TOTAL].results.good_calls++;
		ret++;
	    } else {
		if (DEBUG_CHILD_ERROR) {
		     (void) fprintf(stderr,
				"%s: write call RPC error %d on file %d\n",
			    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
		}
		op_ptr->results.bad_calls++;
		Ops[TOTAL].results.bad_calls++;
	    }
	} /* for writing max_cnt packets */
    } /* for buffers and fragments */

    /*
     * If we have not gotten an error and we were asked for an async write
     * send a commit operation.
     */
    if (ret && stab_flag != FILE_SYNC)
	ret += op_commit();

    return(ret);

} /* op_write */


static int
op_create(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    CREATE3args		args;
    CREATE3res		reply;			/* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */

    op_ptr = &Ops[CREATE];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.where.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.where.name = Cur_filename;
    args.how.mode = UNCHECKED;
    args.how.createhow3_u.obj_attributes.mode.set_it = TRUE;
    args.how.createhow3_u.obj_attributes.mode.mode = (NFSMODE_REG | 0666);
    args.how.createhow3_u.obj_attributes.uid.set_it = TRUE;
    args.how.createhow3_u.obj_attributes.uid.uid = Cur_uid;
    args.how.createhow3_u.obj_attributes.gid.set_it = TRUE;
    args.how.createhow3_u.obj_attributes.gid.gid = Cur_gid;
    args.how.createhow3_u.obj_attributes.atime.set_it = TRUE;
    args.how.createhow3_u.obj_attributes.atime.atime.seconds = Cur_time.esec;
    args.how.createhow3_u.obj_attributes.atime.atime.nseconds =
						Cur_time.usec * 1000;
    args.how.createhow3_u.obj_attributes.mtime.set_it = TRUE;
    args.how.createhow3_u.obj_attributes.mtime.mtime.seconds = Cur_time.esec;
    args.how.createhow3_u.obj_attributes.mtime.mtime.nseconds =
						Cur_time.usec * 1000;
    args.how.createhow3_u.obj_attributes.size.set_it = TRUE;
    args.how.createhow3_u.obj_attributes.size.size._p._u = (uint32_t) 0;
    args.how.createhow3_u.obj_attributes.size.size._p._l = (uint32_t) 0;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_CREATE,
			xdr_CREATE3args, (char *) &args,
			xdr_CREATE3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Exists;
	    (void) memmove((char *) &Cur_file_ptr->fh3,
			(char *) &reply.resok.obj.handle,
			sizeof (nfs_fh3));
	    (void) strcpy(Cur_file_ptr->file_name, Cur_filename);
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
			(char *) &reply.resok.obj_attributes.attr,
			sizeof(Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: create call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: create call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_create */


static int
op_mkdir(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    MKDIR3args		args;
    MKDIR3res		reply;			/* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */

    op_ptr = &Ops[MKDIR];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.where.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.where.name = Cur_filename;
    args.attributes.mode.set_it = TRUE;
    args.attributes.mode.mode = (NFSMODE_DIR | 0777);
    args.attributes.uid.set_it = TRUE;
    args.attributes.uid.uid = Cur_uid;
    args.attributes.gid.set_it = TRUE;
    args.attributes.gid.gid = Cur_gid;
    args.attributes.size.set_it = TRUE;
    args.attributes.size.size._p._u = 0;
    args.attributes.size.size._p._l = 512;
    args.attributes.atime.set_it = TRUE;
    args.attributes.atime.atime.seconds = Cur_time.esec;
    args.attributes.atime.atime.nseconds = Cur_time.usec * 1000;
    args.attributes.mtime.set_it = TRUE;
    args.attributes.mtime.mtime.seconds = Cur_time.esec;
    args.attributes.mtime.mtime.nseconds = Cur_time.usec * 1000;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_MKDIR,
			xdr_MKDIR3args, (char *) &args,
			xdr_MKDIR3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
            Cur_file_ptr->state = Empty_dir;
	    (void) memmove((char *) &Cur_file_ptr->fh3,
			(char *) &reply.resok.obj.handle,
			sizeof (nfs_fh3));
	    (void) strcpy(Cur_file_ptr->file_name, Cur_filename);
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
			(char *) &reply.resok.obj_attributes.attr,
			sizeof(Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: mkdir call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: mkdir call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_mkdir */


static int
op_symlink(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    sfs_fh_type		*target_fileinfo_ptr;	/* target file */
    SYMLINK3args	args;
    SYMLINK3res		reply;			/* the reply */
    char		sym_data[NFS_MAXPATHLEN];	/* symlink data */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */

    op_ptr = &Ops[SYMLINK];
    ret = 0;

    /* set up the arguments */
    target_fileinfo_ptr = randfh(SYMLINK, 0, 0, Exists, Sfs_non_io_file);
    (void) memmove((char *) &args.where.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.where.name = Cur_filename;

    (void) strcpy(sym_data, "./");
    (void) strcat(sym_data, target_fileinfo_ptr->file_name);
    args.symlink.symlink_attributes.size.set_it = TRUE;
    args.symlink.symlink_attributes.size.size._p._u = (uint32_t) 0;
    args.symlink.symlink_attributes.size.size._p._l = strlen(sym_data);
    args.symlink.symlink_data = sym_data;

    args.symlink.symlink_attributes.mode.set_it = TRUE;
    args.symlink.symlink_attributes.mode.mode = (NFSMODE_LNK | 0777);
    args.symlink.symlink_attributes.uid.set_it = TRUE;
    args.symlink.symlink_attributes.uid.uid = Cur_uid;
    args.symlink.symlink_attributes.gid.set_it = TRUE;
    args.symlink.symlink_attributes.gid.gid = Cur_gid;
    args.symlink.symlink_attributes.atime.set_it = TRUE;
    args.symlink.symlink_attributes.atime.atime.seconds = Cur_time.esec;
    args.symlink.symlink_attributes.atime.atime.nseconds =
						Cur_time.usec * 1000;
    args.symlink.symlink_attributes.mtime.set_it = TRUE;
    args.symlink.symlink_attributes.mtime.mtime.seconds = Cur_time.esec;
    args.symlink.symlink_attributes.mtime.mtime.nseconds =
						Cur_time.usec * 1000;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_SYMLINK,
			xdr_SYMLINK3args, (char *) &args,
			xdr_SYMLINK3res, (char *) &reply,
			((int)Current_test_phase < (int)Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    /*
	     * SYMLINK doesn't return a fh. If we try to access this symlink
	     * (eg, remove(), readlink()) before we do a lookup, we won't have
	     * a fh to use. So, we do a lookup call here. If it fails, we fill
	     * in what we can.
	     */
	    Cur_file_ptr->state = Exists;
	    if (op_lookup() == 0) {
		(void) strcpy(Cur_file_ptr->file_name, Cur_filename);
		Cur_file_ptr->attributes3.type = NF3LNK;
		Cur_file_ptr->attributes3.mode = (NFSMODE_LNK|0777);
		Cur_file_ptr->attributes3.uid = Cur_uid;
		Cur_file_ptr->attributes3.gid = Cur_gid;
		Cur_file_ptr->attributes3.atime.seconds = Cur_time.esec;
		Cur_file_ptr->attributes3.atime.nseconds =
							Cur_time.usec * 1000;
		Cur_file_ptr->attributes3.mtime =
			Cur_file_ptr->attributes3.atime;
	    } else
		ret++;
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: symlink call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr,
			"%s: symlink call RPC error %d on file %d\n",
			sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_symlink */


static int
op_mknod(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    MKNOD3args		args;
    MKNOD3res		reply;
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */

    op_ptr = &Ops[MKNOD];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.where.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.where.name = Cur_filename;
    args.what.type = NF3FIFO;
    args.what.mknoddata3_u.pipe_attributes.mode.set_it = TRUE;
    args.what.mknoddata3_u.pipe_attributes.mode.mode = (NFSMODE_FIFO | 0777);
    args.what.mknoddata3_u.pipe_attributes.uid.set_it = TRUE;
    args.what.mknoddata3_u.pipe_attributes.uid.uid = Cur_uid;
    args.what.mknoddata3_u.pipe_attributes.gid.set_it = TRUE;
    args.what.mknoddata3_u.pipe_attributes.gid.gid = Cur_gid;
    args.what.mknoddata3_u.pipe_attributes.size.set_it = TRUE;
    args.what.mknoddata3_u.pipe_attributes.size.size._p._u = (uint32_t) 0;
    args.what.mknoddata3_u.pipe_attributes.size.size._p._l =
							(uint32_t) 512;
    args.what.mknoddata3_u.pipe_attributes.atime.set_it = TRUE;
    args.what.mknoddata3_u.pipe_attributes.atime.atime.seconds =
							Cur_time.esec;
    args.what.mknoddata3_u.pipe_attributes.atime.atime.nseconds =
							Cur_time.usec * 1000;
    args.what.mknoddata3_u.pipe_attributes.atime.set_it = TRUE;
    args.what.mknoddata3_u.pipe_attributes.mtime.mtime.seconds =
								Cur_time.esec;
    args.what.mknoddata3_u.pipe_attributes.mtime.mtime.nseconds =
							Cur_time.usec * 1000;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_MKNOD,
			xdr_MKNOD3args, (char *) &args,
			xdr_MKNOD3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Exists;
	    (void) memmove((char *) &Cur_file_ptr->fh3,
			(char *) &reply.resok.obj.handle,
			sizeof (nfs_fh3));
	    (void) strcpy(Cur_file_ptr->file_name, Cur_filename);
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
			(char *) &reply.resok.obj_attributes.attr,
			sizeof(Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: mknod call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: mknod call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_mknod */


static int
op_remove(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    REMOVE3args		args;
    REMOVE3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[REMOVE];
    ret = 0;

    /* set up the arguments */
    args.object.name = Cur_filename;
    (void) memmove((char *) &args.object.dir,(char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_REMOVE,
			xdr_REMOVE3args, (char *) &args,
			xdr_REMOVE3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Nonexistent;
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
				"%s: remove call NFS error %s on file %d\n",
				sfs_Myname, nfs3_strerror(reply.status),
				Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: remove call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_remove */


static int
op_rmdir(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    RMDIR3args		args;
    RMDIR3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[RMDIR];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.object.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.object.name = Cur_file_ptr->file_name;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_RMDIR,
			xdr_RMDIR3args, (char *) &args,
			xdr_RMDIR3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Nonexistent;
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
				"%s: rmdir call NFS error %s on file %d\n",
				sfs_Myname, nfs3_strerror(reply.status),
				Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: rmdir call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_rmdir */


static int
op_rename(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    sfs_fh_type		*target_fileinfo_ptr;	/* target name */
    RENAME3args		args;
    RENAME3res		reply;			/* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */

    op_ptr = &Ops[RENAME];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.from.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    (void) memmove((char *) &args.to.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));

    target_fileinfo_ptr = randfh(RENAME, 0, 0, Nonexistent,
				 Sfs_non_io_file);

    args.from.name = Cur_file_ptr->file_name;
    (void) sprintf(target_fileinfo_ptr->file_name, Filespec,
		   target_fileinfo_ptr->unique_num);
    args.to.name = target_fileinfo_ptr->file_name;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_RENAME,
			xdr_RENAME3args, (char *) &args,
			xdr_RENAME3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    target_fileinfo_ptr->state = Exists;
	    (void) memmove((char *) &target_fileinfo_ptr->fh3,
			(char *) &Cur_file_ptr->fh3,
			sizeof (nfs_fh3));
	    target_fileinfo_ptr->attributes3 = Cur_file_ptr->attributes3;
	    target_fileinfo_ptr->size = fh_size(Cur_file_ptr);
	    Cur_file_ptr->state = Nonexistent;
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: rename call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: rename call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_rename */


static int
op_link(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    sfs_fh_type		*target_fileinfo_ptr;	/* target */
    LINK3args		args;
    LINK3res		reply;			/* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */

    op_ptr = &Ops[LINK];
    ret = 0;

    /* set up the arguments */
    target_fileinfo_ptr = randfh(LINK, 0, 0, Exists, Sfs_non_io_file);
    (void) memmove((char *) &args.file, (char *) &target_fileinfo_ptr->fh3,
		sizeof (nfs_fh3));
    (void) memmove((char *) &args.link.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.link.name = Cur_filename;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_LINK,
			xdr_LINK3args, (char *) &args,
			xdr_LINK3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Exists;
	    (void) memmove((char *) &Cur_file_ptr->fh3,
			(char *) &target_fileinfo_ptr->fh3,
			sizeof (nfs_fh3));
	    (void) strcpy(Cur_file_ptr->file_name, Cur_filename);
	    target_fileinfo_ptr->attributes3.nlink++;
	    Cur_file_ptr->attributes3 = target_fileinfo_ptr->attributes3;
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: link call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: link call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_link */


static int
op_readdir(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    READDIR3args	args;
    READDIR3res		reply;			/* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    int			i;
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */
    bool_t		hit_eof;
	/* array of entries */
    entry3		entry_stream[SFS_MAXDIRENTS];
    entry3		*entries;		/* ptr to the dir entry */

    op_ptr = &Ops[READDIR];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.cookie._p._l = args.cookie._p._u = (uint32_t) 0;
    (void) memset((char *) args.cookieverf, '\0', NFS3_COOKIEVERFSIZE);
    args.count = DEFAULT_MAX_BUFSIZE;

    /* Have lower layers fill in the data directly.  */
    (void) memset((char *) &reply, '\0', sizeof (reply));
    (void) memset((char *) entry_stream, '\0',
					sizeof (entry3) * SFS_MAXDIRENTS);
    reply.resok.count = SFS_MAXDIRENTS;
    reply.resok.reply.entries = entry_stream;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_READDIR,
			xdr_READDIR3args, (char *) &args,
			xdr_READDIR3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {

	    if (DEBUG_CHILD_RPC) {
		hit_eof = reply.resok.reply.eof;
		entries = reply.resok.reply.entries;
		for (i = 0; i < reply.resok.count; i++) {
		    (void) fprintf(stderr, "%s:READDIR (eof=%d) entry %s\n",
				    sfs_Myname, hit_eof, entries[i].name);
		}
		(void) fflush(stderr);
	    }
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: readdir call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr,
				"%s: readdir call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_readdir */


static int
op_readdirplus(void)
{
    sfs_op_type		*op_ptr;		/* per operation info */
    READDIRPLUS3args	args;
    READDIRPLUS3res	reply;			/* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    int			i;
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;			/* ret val == call success */
    bool_t		hit_eof;
	/* array of entries */
    entryplus3		entry_stream[SFS_MAXDIRENTS];
    entryplus3		*entries;

    op_ptr = &Ops[READDIRPLUS];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.dir, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));
    args.cookie._p._l = args.cookie._p._u = (uint32_t) 0;
    (void) memset((char *) args.cookieverf, '\0', NFS3_COOKIEVERFSIZE);
    (void) memset((char *) entry_stream, '\0',
				sizeof (entryplus3) * SFS_MAXDIRENTS);
    args.dircount = DEFAULT_MAX_BUFSIZE;
    args.maxcount = DEFAULT_MAX_BUFSIZE;

    /* Have lower layers fill in the data directly.  */
    reply.resok.count = SFS_MAXDIRENTS;
    reply.resok.reply.entries = entry_stream;

    /* make the call now */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_READDIRPLUS,
			xdr_READDIRPLUS3args, (char *) &args,
			xdr_READDIRPLUS3res, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {

	    if (DEBUG_CHILD_RPC) {
		hit_eof = reply.resok.reply.eof;
		entries = reply.resok.reply.entries;
		for (i = 0; i < reply.resok.count; i++) {
		    (void) fprintf(stderr, "%s:READDIR (eof=%d) entry %s\n",
				    sfs_Myname, hit_eof, entries[i].name);
		}
		(void) fflush(stderr);
	    }
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: readdir call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr,
				"%s: readdir call RPC error %d on file %d\n",
		    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_readdirplus */


static int
op_fsstat(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    FSSTAT3args		args;
    FSSTAT3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[FSSTAT];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.fsroot, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));

    /* make the call */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_FSSTAT,
			xdr_FSSTAT3args, (char *) &args,
			xdr_FSSTAT3args, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: fsstat call RPC error %d\n",
						sfs_Myname, rpc_stat);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_fsstat */


static int
op_fsinfo(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    FSINFO3args		args;
    FSINFO3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[FSINFO];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.fsroot, (char *) &Cur_file_ptr->dir->fh3,
		sizeof (nfs_fh3));

    /* make the call */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_FSINFO,
			xdr_FSINFO3args, (char *) &args,
			xdr_FSINFO3args, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: fsinfo call RPC error %d\n",
						sfs_Myname, rpc_stat);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_fsinfo */


static int
op_pathconf(void)
{
    sfs_op_type		*op_ptr;	/* per operation info */
    PATHCONF3args	args;
    PATHCONF3res	reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    struct ladtime	start;
    struct ladtime	stop;
    int			ret;		/* ret val == call success */

    op_ptr = &Ops[PATHCONF];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.object, (char *) &Cur_file_ptr->fh3,
		sizeof (nfs_fh3));

    /* make the call */
    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_PATHCONF,
			xdr_PATHCONF3args, (char *) &args,
			xdr_PATHCONF3args, (char *) &reply,
			(Current_test_phase < Warmup_phase)
				 ? Nfs_timers[Init]
				 : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr,
			"%s: pathconf call RPC error %d on file %d\n",
			sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }
    return(ret);

} /* op_pathconf */


static int
op_commit(void)
{
    sfs_op_type			*op_ptr;	/* per operation info */
    int32_t			size;		/* size of data write */
    COMMIT3args			args;
    COMMIT3res			reply;		/* the reply */
    enum clnt_stat		rpc_stat;	/* result from RPC call */
    struct ladtime		start;
    struct ladtime		stop;
    int				ret;		/* ret val == call success */

    op_ptr = &Ops[COMMIT];
    ret = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.file, (char *) &Cur_file_ptr->fh3,
		sizeof (nfs_fh3));
    args.offset._p._u = args.offset._p._l = (uint32_t) 0;
    args.count = Cur_file_ptr->attributes3.size._p._l;
    size = args.count;

    sfs_gettime(&start);
    rpc_stat = clnt_call(NFS_client, NFSPROC3_COMMIT,
				xdr_COMMIT3args, (char *) &args,
				xdr_COMMIT3res, (char *) &reply,
				(Current_test_phase < Warmup_phase)
				     ? Nfs_timers[Init]
				     : Nfs_timers[op_ptr->call_class]);
    sfs_gettime(&stop);
    Cur_time = stop;

    if (rpc_stat == RPC_SUCCESS) {
	if (reply.status == NFS3_OK) {
	    Cur_file_ptr->state = Exists;
	    (void) memmove((char *) &Cur_file_ptr->attributes3,
			(char *) &reply.resok.file_wcc.after.attr,
			sizeof(Cur_file_ptr->attributes3));
	    Cur_file_ptr->size = fh_size(Cur_file_ptr);

	    if (DEBUG_CHILD_RPC) {
		(void) fprintf(stderr, "%s: WRITE %s %ld bytes\n",
					   sfs_Myname, Cur_filename, size);
		(void) fflush(stderr);
	    }
	} else {
	    if (DEBUG_CHILD_ERROR) {
		 (void) fprintf(stderr,
			"%s: write call NFS error %s on file %d\n",
			sfs_Myname, nfs3_strerror(reply.status),
			Cur_file_ptr->unique_num);
	    }
	}
	sfs_elapsedtime(op_ptr, &start, &stop);
	op_ptr->results.good_calls++;
	Ops[TOTAL].results.good_calls++;
	ret++;
    } else {
	if (DEBUG_CHILD_ERROR) {
	     (void) fprintf(stderr, "%s: write call RPC error %d on file %d\n",
			    sfs_Myname, rpc_stat, Cur_file_ptr->unique_num);
	}
	op_ptr->results.bad_calls++;
	Ops[TOTAL].results.bad_calls++;
    }

    return(ret);

} /* op_commit */


#define LAD_RETRIABLE(stat) (((stat) == RPC_TIMEDOUT) || ((stat) == RPC_CANTDECODERES))

/*
 * Reliably lookup a file in the current directory
 * Return:
 *	-1	RPC error
 *	1	File doesn't exist
 *	0	File exists
 */
int
lad_lookup(sfs_fh_type *file_ptr, char *name)
{
    LOOKUP3args		args;
    LOOKUP3res		reply;		/* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */

    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_lookup: %lx[%lx] %s\n", sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, name);
	(void) fflush(stderr);
    }

    /* CONSTCOND */
    while (1) {
	/* set up the arguments */
        (void) memmove((char *) &args.what.dir, (char *) &file_ptr->dir->fh3,
							sizeof (nfs_fh3));
	args.what.name = name;
        (void) memset((char *) &reply.resok.object, '\0', sizeof (nfs_fh3));

	/* make the call */
	rpc_stat = clnt_call(NFS_client, NFSPROC3_LOOKUP,
			xdr_LOOKUP3args, (char *) &args,
			xdr_LOOKUP3res, (char *) &reply,
			Nfs_timers[Init]);

	if (rpc_stat == RPC_SUCCESS) 
		break;
	if (rpc_stat != RPC_TIMEDOUT) {
		(void) fprintf(stderr, "lad_lookup(%s) RPC call failed : %s\n",
				name, clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
    }

    if (reply.status == NFS3ERR_NOENT) {
	return(1);
    }

    if (reply.status != NFS3_OK) {
	(void) fprintf(stderr, "lad_lookup(%s) NFS call failed : %s\n",
			name, nfs3_strerror(reply.status));
	return(-1);
    }

    file_ptr->state = Exists;
    (void) memmove((char *) &file_ptr->fh3,
			(char *) &reply.resok.object,
			sizeof (nfs_fh3));
    (void) strcpy(file_ptr->file_name, name);
    (void) memmove((char *) &file_ptr->attributes3,
				(char *) &reply.resok.obj_attributes.attr,
				sizeof (file_ptr->attributes3));
    file_ptr->size = fh_size(file_ptr);
    return(0);
}

/*
 * Reliably remove a file in the current directory
 */
int
lad_remove(sfs_fh_type *file_ptr, char *name)
{
    REMOVE3args         args;
    REMOVE3res          reply;          /* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    int			retried = 0;

    /*
     * This function presumes that the file name does exist
     */
    if (file_ptr->attributes3.type == NF3DIR)
	return (lad_rmdir(file_ptr, name));

    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_remove: %lx[%lx] %s\n", sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, name);
	(void) fflush(stderr);
    }

    /* CONSTCOND */
    while (1) {
	/* set up the arguments */
	args.object.name = name;
	(void) memmove((char *) &args.object.dir, (char *) &file_ptr->dir->fh3,
			sizeof(nfs_fh3));

	/* make the call now */
	rpc_stat = clnt_call(NFS_client, NFSPROC3_REMOVE,
                        xdr_REMOVE3args, (char *) &args,
                        xdr_REMOVE3res, (char *) &reply,
			Nfs_timers[Init]);

	if (rpc_stat == RPC_SUCCESS) 
		break;
	if (rpc_stat != RPC_TIMEDOUT) {
		(void) fprintf(stderr, "lad_remove(%s) RPC call failed : %s\n",
				name, clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
	retried++;
    }

    if (reply.status != NFS3_OK) {
	if (reply.status != NFS3ERR_NOENT || !retried) {
	    (void) fprintf(stderr, "lad_remove(%s) NFS call failed : %s\n",
			name, nfs3_strerror(reply.status));
	    return(-1);
	}
    }

    file_ptr->state = Nonexistent;

    return(0);
}

/*
 * Reliably remove a directory in the current directory
 */
int
lad_rmdir(sfs_fh_type *file_ptr, char *name)
{
    RMDIR3args          args;
    RMDIR3res           reply;          /* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */
    int			retried = 0;

    /*
     * This function presumes that the file name does exist and is empty
     */
    if (file_ptr->attributes3.type != NF3DIR)
	return (lad_remove(file_ptr, name));

    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_rmdir: %lx[%lx] %s\n", sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, name);
	(void) fflush(stderr);
    }

    /* CONSTCOND */
    while (1) {
	/* set up the arguments */
	args.object.name = name;
        (void) memmove((char *) &args.object.dir, (char *) &file_ptr->dir->fh3,
                sizeof (nfs_fh3));

	/* make the call now */
	rpc_stat = clnt_call(NFS_client, NFSPROC3_RMDIR,
                        xdr_RMDIR3args, (char *) &args,
                        xdr_RMDIR3res, (char *) &reply,
			Nfs_timers[Init]);

	if (rpc_stat == RPC_SUCCESS) 
		break;
	if (rpc_stat != RPC_TIMEDOUT) {
		(void) fprintf(stderr, "lad_rmdir(%s) RPC call failed : %s\n",
				name, clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
	retried++;
    }

    if (reply.status != NFS3_OK) {
	if (reply.status != NFS3ERR_NOENT || !retried) {
	    (void) fprintf(stderr, "lad_rmdir(%s) NFS call failed : %s\n",
			name, nfs3_strerror(reply.status));
	    return(-1);
	}
    }

    file_ptr->state = Nonexistent;

    return(0);
}

/*
 * Reliably create a symlink in the current directory
 */
int
lad_symlink(sfs_fh_type *file_ptr, char *target, char *name)
{
    SYMLINK3args        args;
    SYMLINK3res         reply;			/* the reply */
    char		sym_data[NFS_MAXPATHLEN];	/* symlink data */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    int			retried = 0;

    /*
     * This function presumes that the file name does not already exist
     */
    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_symlink: %lx[%lx] %s -> %s\n", sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, name, target);
	(void) fflush(stderr);
    }

    /* CONSTCOND */
    while (1) {
	/* set up the arguments */
         (void) memmove((char *) &args.where.dir, (char *) &file_ptr->dir->fh3,
                sizeof (nfs_fh3));
         args.where.name = name;

	(void) strcpy(sym_data, "./");
	(void) strcat(sym_data, target);
	args.symlink.symlink_attributes.size.set_it = TRUE;
	args.symlink.symlink_attributes.size.size._p._u = (uint32_t) 0;
	args.symlink.symlink_attributes.size.size._p._l = strlen(sym_data);
	args.symlink.symlink_data = sym_data;

	args.symlink.symlink_attributes.mode.set_it = TRUE;
	args.symlink.symlink_attributes.mode.mode = (NFSMODE_LNK | 0777);
	args.symlink.symlink_attributes.uid.set_it = TRUE;
	args.symlink.symlink_attributes.uid.uid = Cur_uid;
	args.symlink.symlink_attributes.gid.set_it = TRUE;
	args.symlink.symlink_attributes.gid.gid = Cur_gid;
	args.symlink.symlink_attributes.atime.set_it = TRUE;
	args.symlink.symlink_attributes.atime.atime.seconds = Cur_time.esec;
	args.symlink.symlink_attributes.atime.atime.nseconds =
                                                Cur_time.usec * 1000;
	args.symlink.symlink_attributes.mtime.set_it = TRUE;
	args.symlink.symlink_attributes.mtime.mtime.seconds = Cur_time.esec;
	args.symlink.symlink_attributes.mtime.mtime.nseconds =
                                                Cur_time.usec * 1000;


	/* make the call now */
	rpc_stat = clnt_call(NFS_client, NFSPROC3_SYMLINK,
                        xdr_SYMLINK3args, (char *) &args,
                        xdr_SYMLINK3res, (char *) &reply,
		 	Nfs_timers[Init]);
	if (rpc_stat == RPC_SUCCESS) 
		break;
	if (rpc_stat != RPC_TIMEDOUT) {
		(void) fprintf(stderr, "lad_symlink(%s) RPC call failed : %s\n",
				name, clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
	retried++;
    }

    if (reply.status != NFS3_OK) {
	if (reply.status != NFS3ERR_EXIST || !retried) {
	    (void) fprintf(stderr, "lad_symlink(%s, %s) NFS call failed : %s\n",
			target, name, nfs3_strerror(reply.status));
	    return(-1);
	}
    }

    /*
     * SYMLINK may not return a fh. If we try to
     * access this symlink (eg, remove(), readlink())
     * before we do a lookup, we won't have a fh to use.
     * So, we do a lookup call here.
     * If it fails, we fill in what we can.
     */  
    return (lad_lookup(file_ptr, name));
}

/*
 * Reliably create a directory in the current directory
 */
int
lad_mkdir(sfs_fh_type *file_ptr, char *name)
{
    MKDIR3args          args;
    MKDIR3res           reply;                  /* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    int			retried = 0;

    /*
     * This function presumes that the file name does not already exist
     */
    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_mkdir: %lx[%lx] %s\n", sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, name);
	(void) fflush(stderr);
    }

    /* CONSTCOND */
    while (1) {
	/* set up the arguments */
 	(void) memmove((char *) &args.where.dir, (char *) &file_ptr->dir->fh3,
                sizeof (nfs_fh3));
	args.where.name = name;
	args.attributes.mode.set_it = TRUE;
	args.attributes.mode.mode = (NFSMODE_DIR | 0777);
	args.attributes.uid.set_it = TRUE;
	args.attributes.uid.uid = Cur_uid;
	args.attributes.gid.set_it = TRUE;
	args.attributes.gid.gid = Cur_gid;
	args.attributes.size.set_it = TRUE;
	args.attributes.size.size._p._u = 0;
	args.attributes.size.size._p._l = 512;
	args.attributes.atime.set_it = TRUE;
	args.attributes.atime.atime.seconds = Cur_time.esec;
	args.attributes.atime.atime.nseconds = Cur_time.usec * 1000;
	args.attributes.mtime.set_it = TRUE;
	args.attributes.mtime.mtime.seconds = Cur_time.esec;
	args.attributes.mtime.mtime.nseconds = Cur_time.usec * 1000;

	/* make the call now */
	rpc_stat = clnt_call(NFS_client, NFSPROC3_MKDIR,
                        xdr_MKDIR3args, (char *) &args,
                        xdr_MKDIR3res, (char *) &reply,
			Nfs_timers[Init]);

	if (rpc_stat == RPC_SUCCESS) 
		break;
	if (rpc_stat != RPC_TIMEDOUT) {
		(void) fprintf(stderr, "lad_mkdir(%s) RPC call failed : %s\n",
				name, clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
	retried++;
    }

    if (!retried && reply.status == NFS3ERR_EXIST)
	return(1);

    if (reply.status != NFS3_OK) {
	if (reply.status != NFS3ERR_EXIST || !retried) {
	    (void) fprintf(stderr, "lad_mkdir(%s) NFS call failed : %s\n",
			name, nfs3_strerror(reply.status));
	    return(-1);
	}
	/*
	 * If the first mkdir suceeded but the reply as dropped and
	 * was retransmitted, we still need to lookup the attributes
	 */
	if (lad_lookup(file_ptr, name))
	   return (-1);
    } else {
	(void) memmove((char *) &file_ptr->fh3,
                   (char *) &reply.resok.obj.handle,
                   sizeof (nfs_fh3));
	(void) strcpy(file_ptr->file_name, name);
	(void) memmove((char *) &file_ptr->attributes3,
                   (char *) &reply.resok.obj_attributes.attr,
                   sizeof(file_ptr->attributes3));
	file_ptr->size = fh_size(file_ptr);
    }
    file_ptr->state = Empty_dir;

    return(0);
}

/*
 * Reliably commit a file
 */
static int
lad_commit(sfs_fh_type *file_ptr)
{
    COMMIT3args			args;
    COMMIT3res			reply;		/* the reply */
    enum clnt_stat		rpc_stat;	/* result from RPC call */

    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_commit: %lx[%lx]\n",
			sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir);
	(void) fflush(stderr);
    }

    /* set up the arguments */
    (void) memmove((char *) &args.file, (char *) &file_ptr->fh3,
		sizeof (nfs_fh3));
    args.offset._p._u = args.offset._p._l = (uint32_t) 0;
    args.count = file_ptr->attributes3.size._p._l;

    /* CONSTCOND */
    while (1) {
        rpc_stat = clnt_call(NFS_client, NFSPROC3_COMMIT,
				xdr_COMMIT3args, (char *) &args,
				xdr_COMMIT3res, (char *) &reply,
			 	Nfs_timers[Init]);
        if (rpc_stat == RPC_SUCCESS) 
	    break;
        if (rpc_stat != RPC_TIMEDOUT) {
	    (void) fprintf(stderr, "lad_commit() RPC call failed : %s\n",
				clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
    }

    return(0);
}

/*
 * Reliably write a file in the current directory
 */
int
lad_write(sfs_fh_type *file_ptr, int32_t offset, int32_t length)
{
    static char			*buf = NULL;	/* the data buffer */
    int32_t			size;		/* size of data write */
    int32_t			cur_cnt;
    WRITE3args                  args;
    WRITE3res                   reply;          /* the reply */
    enum clnt_stat		rpc_stat;	/* result from RPC call */
    int				async = 1;

    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_write: %lx[%lx] %ld %ld\n",
			sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, offset, length);
	(void) fflush(stderr);
    }

    /*
     * This function presumes that the file name does exist
     * Initialize write buffer to known value
     */
    if (buf == NULL) {
	buf = init_write_buffer();
    }

    /*
     * If a short file write don't bother with the commit, just write sync.
     */
    if (length <= Bytes_per_block)
	async = 0;

    /* set up the arguments */
    (void) memmove((char *) &args.file, (char *) &file_ptr->fh3,
                sizeof (nfs_fh3));
    args.offset._p._u = 0;
    args.offset._p._l = offset;
    if (async)
	    args.stable = UNSTABLE;
    else
	    args.stable = FILE_SYNC;

    size = Bytes_per_block;
    for (cur_cnt = 0; cur_cnt < length; cur_cnt += size) {
	if ((cur_cnt + size) > length)
		size = length - cur_cnt;

	if (size == 0)
	    break;

	args.count = size;
	args.data.data_len = size;
	args.data.data_val = buf;

	/* make the call now */
        /* CONSTCOND */
	while (1) {
	    rpc_stat = clnt_call(NFS_client, NFSPROC3_WRITE,
                                xdr_WRITE3args, (char *) &args,
                                xdr_WRITE3res, (char *) &reply,
				Nfs_timers[Init]);

	    if (rpc_stat == RPC_SUCCESS) 
		break;
	    if (rpc_stat != RPC_TIMEDOUT) {
		(void) fprintf(stderr, "lad_write() RPC call failed : %s\n",
				clnt_sperrno(rpc_stat));
	    }
	    if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	    }
	}
	if (reply.status != NFS3_OK) {
	    (void) fprintf(stderr, "lad_write() NFS call failed : %s\n",
			nfs3_strerror(reply.status));
	    return(-1);
	}
	file_ptr->state = Exists;
        (void) memmove((char *) &file_ptr->attributes3,
                                (char *) &reply.resok.file_wcc.after.attr,
                                sizeof (file_ptr->attributes3));
	file_ptr->size = fh_size(file_ptr);

        args.offset._p._l += size;
    }

    if (async)
	(void) lad_commit(file_ptr);
    return(0);
}

/*
 * Reliably create a file in the current directory
 */
int
lad_create(sfs_fh_type *file_ptr, char *name)
{
    CREATE3args         args;
    CREATE3res          reply;                  /* the reply */
    enum clnt_stat	rpc_stat;		/* result from RPC call */
    int			retried = 0;

    /*
     * This function presumes that the file name does not already exist
     */
    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_create: %lx[%lx] %s\n", sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, name);
	(void) fflush(stderr);
    }

    /* CONSTCOND */
    while (1) {
	/* set up the arguments */
	(void) memmove((char *) &args.where.dir, (char *) &file_ptr->dir->fh3,
                sizeof (nfs_fh3));
	args.where.name = name;
	args.how.mode = UNCHECKED;
	args.how.createhow3_u.obj_attributes.mode.set_it = TRUE;
	args.how.createhow3_u.obj_attributes.mode.mode = (NFSMODE_REG | 0666);
	args.how.createhow3_u.obj_attributes.uid.set_it = TRUE;
	args.how.createhow3_u.obj_attributes.uid.uid = Cur_uid;
	args.how.createhow3_u.obj_attributes.gid.set_it = TRUE;
	args.how.createhow3_u.obj_attributes.gid.gid = Cur_gid;
	args.how.createhow3_u.obj_attributes.atime.set_it = TRUE;
	args.how.createhow3_u.obj_attributes.atime.atime.seconds =
						Cur_time.esec;
	args.how.createhow3_u.obj_attributes.atime.atime.nseconds =
                                                Cur_time.usec * 1000;
	args.how.createhow3_u.obj_attributes.mtime.set_it = TRUE;
	args.how.createhow3_u.obj_attributes.mtime.mtime.seconds =
						Cur_time.esec;
	args.how.createhow3_u.obj_attributes.mtime.mtime.nseconds =
                                                Cur_time.usec * 1000;
	args.how.createhow3_u.obj_attributes.size.set_it = TRUE;
	args.how.createhow3_u.obj_attributes.size.size._p._u =
						(uint32_t) 0;
	args.how.createhow3_u.obj_attributes.size.size._p._l =
						(uint32_t) 0;

	/* make the call now */
	rpc_stat = clnt_call(NFS_client, NFSPROC3_CREATE,
                        xdr_CREATE3args, (char *) &args,
                        xdr_CREATE3res, (char *) &reply,
			Nfs_timers[Init]);

	if (rpc_stat == RPC_SUCCESS) 
		break;
	if (rpc_stat != RPC_TIMEDOUT) {
		(void) fprintf(stderr, "lad_create(%s) RPC call failed : %s\n",
				name, clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
	retried++;
    }

    if (!retried && reply.status == NFS3ERR_EXIST) {
	return(1);
    }

    if (reply.status != NFS3_OK) {
	if (reply.status != NFS3ERR_EXIST || !retried) {
	    (void) fprintf(stderr, "lad_create(%s) NFS call failed : %s\n",
			name, nfs3_strerror(reply.status));
	    return(-1);
	}
        /*
         * If the first create suceeded but the reply as dropped and
         * was retransmitted, we still need to lookup the attributes
         */
        if (lad_lookup(file_ptr, name))
           return (-1);
    } else {
	(void) memmove((char *) &file_ptr->fh3,
                   (char *) &reply.resok.obj.handle,
                   sizeof (nfs_fh3));
	(void) strcpy(file_ptr->file_name, name);
	(void) memmove((char *) &file_ptr->attributes3,
                   (char *) &reply.resok.obj_attributes.attr,
                   sizeof(file_ptr->attributes3));
	file_ptr->size = fh_size(file_ptr);
    }

    file_ptr->state = Exists;
    /*
     * Directories are created as Empty_dir, when a file is created it
     * becomes an Exists.
     */
    file_ptr->dir->state = Exists;

    return(0);
}

/*
 * Reliably set the size of a file in the current directory
 */
int
lad_truncate(sfs_fh_type *file_ptr, int32_t size)
{
    SETATTR3args        args;
    SETATTR3res         reply;          /* the reply */
    enum clnt_stat	rpc_stat;	/* result from RPC call */

    /*
     * This function presumes that the file name already exists
     */
    if (DEBUG_CHILD_RPC) {
	(void) fprintf(stderr, "%s:lad_truncate: %lx[%lx] %ld\n", sfs_Myname,
			(int32_t) file_ptr, (int32_t) file_ptr->dir, size);
	(void) fflush(stderr);
    }

    /* CONSTCOND */
    while (1) {
	/*
	 * set up the arguments
	 * Set the mode and times as well
	 */
	(void) memmove((char *) &args.object, (char *) &file_ptr->fh3,
                                                        sizeof (nfs_fh3));
	args.new_attributes.mode.set_it = TRUE;
	args.new_attributes.mode.mode = (uint32_t) 0666;
	args.new_attributes.uid.set_it = FALSE;
	args.new_attributes.uid.uid = (uint32_t) -1;
	args.new_attributes.gid.set_it = FALSE;
	args.new_attributes.gid.gid = (uint32_t) -1;
	args.new_attributes.size.set_it = TRUE;
	args.new_attributes.size.size._p._u = 0;
	args.new_attributes.size.size._p._l = size;
	args.new_attributes.atime.set_it = TRUE;
	args.new_attributes.atime.atime.seconds = Cur_time.esec;
	args.new_attributes.atime.atime.nseconds = Cur_time.usec * 1000;
	args.new_attributes.mtime.set_it = TRUE;
	args.new_attributes.mtime.mtime.seconds = Cur_time.esec;
	args.new_attributes.mtime.mtime.nseconds = Cur_time.usec * 1000;
	args.guard.check = FALSE;

	/* make the call */
	rpc_stat = clnt_call(NFS_client, NFSPROC3_SETATTR,
                        xdr_SETATTR3args, (char *) &args,
                        xdr_SETATTR3res, (char *) &reply,
			Nfs_timers[Init]);

	if (rpc_stat == RPC_SUCCESS) 
		break;
	if (rpc_stat != RPC_TIMEDOUT) {
		(void)fprintf(stderr,
				"lad_truncate(%ld) RPC call failed : %s\n",
				size, clnt_sperrno(rpc_stat));
	}
	if (!LAD_RETRIABLE(rpc_stat)) {
		return(-1);
	}
    }

    if (reply.status != NFS3_OK) {
	(void) fprintf(stderr, "lad_truncate(%ld) NFS call failed : %s\n",
			size, nfs3_strerror(reply.status));
	return(-1);
    }
    (void) memmove(&file_ptr->attributes3,
                   &reply.resok.obj_wcc.after.attr,
                   sizeof (file_ptr->attributes3));
    file_ptr->size = fh_size(file_ptr);

    return(0);
}

static char *
nfs3_strerror(int status)
{
    static char str[40];
    switch (status) {
	case NFS3_OK:
	    (void) strcpy(str, "no error");
	    break;
	case NFS3ERR_PERM:
	    (void) strcpy(str, "Not owner");
	    break;
	case NFS3ERR_NOENT:
	    (void) strcpy(str, "No such file or directory");
	    break;
	case NFS3ERR_IO:
	    (void) strcpy(str, "I/O error");
	    break;
	case NFS3ERR_NXIO:
	    (void) strcpy(str, "No such device or address");
	    break;
	case NFS3ERR_ACCES:
	    (void) strcpy(str, "Permission denied");
	    break;
	case NFS3ERR_EXIST:
	    (void) strcpy(str, "File exists");
	    break;
	case NFS3ERR_XDEV:
	    (void) strcpy(str, "Cross-device link");
	    break;
	case NFS3ERR_NODEV:
	    (void) strcpy(str, "No such device");
	    break;
	case NFS3ERR_NOTDIR:
	    (void) strcpy(str, "Not a directory");
	    break;
	case NFS3ERR_ISDIR:
	    (void) strcpy(str, "Is a directory");
	    break;
	case NFS3ERR_INVAL:
	    (void) strcpy(str, "Invalid argument");
	    break;
	case NFS3ERR_FBIG:
	    (void) strcpy(str, "File too large");
	    break;
	case NFS3ERR_NOSPC:
	    (void) strcpy(str, "No space left on device");
	    break;
	case NFS3ERR_ROFS:
	    (void) strcpy(str, "Read-only file system");
	    break;
	case NFS3ERR_MLINK:
	    (void) strcpy(str, "Too many links");
	    break;
	case NFS3ERR_NAMETOOLONG:
	    (void) strcpy(str, "File name too long");
	    break;
	case NFS3ERR_NOTEMPTY:
	    (void) strcpy(str, "Directory not empty");
	    break;
	case NFS3ERR_DQUOT:
	    (void) strcpy(str, "Disc quota exceeded");
	    break;
	case NFS3ERR_STALE:
	    (void) strcpy(str, "Stale NFS file handle");
	    break;
	case NFS3ERR_REMOTE:
	    (void) strcpy(str, "Object is remote");
	    break;
	case NFS3ERR_BADHANDLE:
	    (void) strcpy(str, "Bad file handle");
	    break;
	case NFS3ERR_NOT_SYNC:
	    (void) strcpy(str, "Not sync write");
	    break;
	case NFS3ERR_BAD_COOKIE:
	    (void) strcpy(str, "Bad cookie");
	    break;
	case NFS3ERR_NOTSUPP:
	    (void) strcpy(str, "Operation not supported");
	    break;
	case NFS3ERR_TOOSMALL:
	    (void) strcpy(str, "Value too small");
	    break;
	case NFS3ERR_SERVERFAULT:
	    (void) strcpy(str, "Server fault");
	    break;
	case NFS3ERR_BADTYPE:
	    (void) strcpy(str, "Bad type");
	    break;
	case NFS3ERR_JUKEBOX:
	    (void) strcpy(str, "Jukebox");
	    break;
	case NFS3ERR_RFS_TIMEOUT:
		(void) strcpy(str, "RFS timeout");
	default:
	    (void) sprintf(str, "Unknown status %d", status);
	    break;
    }
    return (str);
}

/* sfs_3_ops.c */