/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/*  --------------------------------------------------------------- */
/* Licensed Materials - Property of IBM                             */
/* Blue Gene/Q 5765-PER 5765-PRP                                    */
/*                                                                  */
/* (C) Copyright IBM Corp. 2011, 2012 All Rights Reserved           */
/* US Government Users Restricted Rights -                          */
/* Use, duplication, or disclosure restricted                       */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */
/*  (C)Copyright IBM Corp.  2007, 2011  */
/**
 * \file include/mpidimpl.h
 * \brief API MPID additions to MPI functions and structures
 */

#ifndef __include_mpidimpl_h__
#define __include_mpidimpl_h__

#include <mpiimpl.h>
#include "mpid_sched.h"
#include "pamix.h"
#include <mpix.h>


#ifdef DYNAMIC_TASKING

#define MPIDI_MAX_KVS_VALUE_LEN    4096

typedef struct MPIDI_PG
{
    /* MPIU_Object field.  MPIDI_PG_t objects are not allocated using the
       MPIU_Object system, but we do use the associated reference counting
       routines.  Therefore, handle must be present, but is not used
       except by debugging routines */
    MPIU_OBJECT_HEADER; /* adds handle and ref_count fields */

    /* Next pointer used to maintain a list of all process groups known to
       this process */
    struct MPIDI_PG * next;

    /* Number of processes in the process group */
    int size;

    /* VC table.  At present this is a pointer to an array of VC structures.
       Someday we may want make this a pointer to an array
       of VC references.  Thus, it is important to use MPIDI_PG_Get_vc()
       instead of directly referencing this field. */
    MPID_VCR vct;

    /* Pointer to the process group ID.  The actual ID is defined and
       allocated by the process group.  The pointer is kept in the
       device space because it is necessary for the device to be able to
       find a particular process group. */
    void * id;

    /* Replacement abstraction for connection information */
    /* Connection information needed to access processes in this process
       group and to share the data with other processes.  The items are
       connData - pointer for data used to implement these functions
                  (e.g., a pointer to an array of process group info)
       getConnInfo( rank, buf, bufsize, self ) - function to store into
                  buf the connection information for rank in this process
                  group
       connInfoToString( buf_p, size, self ) - return in buf_p a string
                  that can be sent to another process to recreate the
                  connection information (the info needed to support
                  getConnInfo)
       connInfoFromString( buf, self ) - setup the information needed
                  to implement getConnInfo
       freeConnInfo( self ) - free any storage or resources associated
                  with the connection information.

       See ch3/src/mpidi_pg.c
    */
    void *connData;
    int  (*getConnInfo)( int, char *, int, struct MPIDI_PG * );
    int  (*connInfoToString)( char **, int *, struct MPIDI_PG * );
    int  (*connInfoFromString)( const char *,  struct MPIDI_PG * );
    int  (*freeConnInfo)( struct MPIDI_PG * );
}
MPIDI_PG_t;

typedef int (*MPIDI_PG_Compare_ids_fn_t)(void * id1, void * id2);
typedef int (*MPIDI_PG_Destroy_fn_t)(MPIDI_PG_t * pg);


typedef MPIDI_PG_t * MPIDI_PG_iterator;

typedef struct conn_info {
  int                rem_world_id;
  int                ref_count;
  int                *rem_taskids;  /* The last member of this array is -1 */
  struct conn_info   *next;
}conn_info;


/*--------------------------
  BEGIN MPI PORT SECTION
  --------------------------*/
/* These are the default functions */
int MPIDI_Comm_connect(const char *, struct MPID_Info *, int, struct MPID_Comm *, struct MPID_Comm **);
int MPIDI_Comm_accept(const char *, struct MPID_Info *, int, struct MPID_Comm *, struct MPID_Comm **);

int MPIDI_Comm_spawn_multiple(int, char **, char ***, int *, struct MPID_Info **,
                              int, struct MPID_Comm *, struct MPID_Comm **, int *);


typedef struct MPIDI_Port_Ops {
    int (*OpenPort)( struct MPID_Info *, char *);
    int (*ClosePort)( const char * );
    int (*CommAccept)( const char *, struct MPID_Info *, int, struct MPID_Comm *,
                       struct MPID_Comm ** );
    int (*CommConnect)( const char *, struct MPID_Info *, int, struct MPID_Comm *,
                        struct MPID_Comm ** );
} MPIDI_PortFns;


#define MPIDI_VC_add_ref( _vc )                                 \
    do { MPIU_Object_add_ref( _vc ); } while (0)

#define MPIDI_PG_add_ref(pg_)                   \
do {                                            \
    MPIU_Object_add_ref(pg_);                   \
} while (0)
#define MPIDI_PG_release_ref(pg_, inuse_)       \
do {                                            \
    MPIU_Object_release_ref(pg_, inuse_);       \
} while (0)

#define MPIDI_VC_release_ref( _vc, _inuse ) \
    do { MPIU_Object_release_ref( _vc, _inuse ); } while (0)


/* Initialize a new VC */
int MPIDI_PG_Create_from_string(const char * str, MPIDI_PG_t ** pg_pptr,
				int *flag);
int MPIDI_PG_Get_size(MPIDI_PG_t * pg);
#define MPIDI_PG_Get_size(pg_) ((pg_)->size)
#endif  /** DYNAMIC_TASKING **/

#endif
