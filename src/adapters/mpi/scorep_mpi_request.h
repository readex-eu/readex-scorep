/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2011,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2011,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2011,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2013, 2017,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2011,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2011,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 */

#ifndef SCOREP_MPI_REQUEST_H
#define SCOREP_MPI_REQUEST_H

/**
   @file
   @ingroup    MPI_Wrapper

   @brief Contains the declarations for the handling of MPI Reqests.
 */

#include <SCOREP_Types.h>
#include <SCOREP_DefinitionHandles.h>
#include <mpi.h>

typedef enum scorep_mpi_request_type
{
    SCOREP_MPI_REQUEST_TYPE_NONE,
    SCOREP_MPI_REQUEST_TYPE_SEND,
    SCOREP_MPI_REQUEST_TYPE_RECV,
    SCOREP_MPI_REQUEST_TYPE_IO_READ,
    SCOREP_MPI_REQUEST_TYPE_IO_WRITE,
    SCOREP_MPI_REQUEST_TYPE_RMA,
    SCOREP_MPI_REQUEST_TYPE_COLL_COMM,
    SCOREP_MPI_REQUEST_TYPE_COLL_SYNC
} scorep_mpi_request_type;

enum scorep_mpi_requests_flags
{
    SCOREP_MPI_REQUEST_FLAG_NONE          = 0x00,
    SCOREP_MPI_REQUEST_FLAG_IS_PERSISTENT = 0x01,
    SCOREP_MPI_REQUEST_FLAG_DEALLOCATE    = 0x02,
    SCOREP_MPI_REQUEST_FLAG_IS_ACTIVE     = 0x10,
    SCOREP_MPI_REQUEST_FLAG_ANY_TAG       = 0x20,
    SCOREP_MPI_REQUEST_FLAG_ANY_SRC       = 0x40,
    SCOREP_MPI_REQUEST_FLAG_CAN_CANCEL    = 0x80
};

typedef uint64_t scorep_mpi_request_flag;

typedef struct
{
    int                              tag;
    int                              dest;
    uint64_t                         bytes;
    MPI_Datatype                     datatype;
    SCOREP_InterimCommunicatorHandle comm_handle;
    void*                            online_analysis_pod;
} scorep_mpi_request_p2p_data;

typedef struct
{
    MPI_Request             request;
    scorep_mpi_request_type request_type;
    scorep_mpi_request_flag flags;
    union
    {
        scorep_mpi_request_p2p_data p2p;
    } payload;
    SCOREP_MpiRequestId id;
} scorep_mpi_request;


/**
 * @brief Return a new request id
 */
SCOREP_MpiRequestId
scorep_mpi_get_request_id( void );

/**
 * @brief Create entry for a given MPI request handle
 * @param request   MPI request handle
 * @param type      Type of request
 * @param flags     Bitmask containing flags set for this request
 * @param tag       MPI tag for this request
 * @param dest      Destination rank of request
 * @param bytes     Number of bytes transfered in request
 * @param datatype  MPI datatype handle
 * @param comm      MPI communicator handle
 * @param id        Request id
 */
void
scorep_mpi_request_p2p_create( MPI_Request             request,
                               scorep_mpi_request_type type,
                               scorep_mpi_request_flag flags,
                               int                     tag,
                               int                     dest,
                               uint64_t                bytes,
                               MPI_Datatype            datatype,
                               MPI_Comm                comm,
                               SCOREP_MpiRequestId     id );

/**
 * @brief  Retrieve internal request entry for an MPI request handle
 * @param  request MPI request handle
 * @return Pointer to corresponding internal request entry
 */
scorep_mpi_request*
scorep_mpi_request_get( MPI_Request request );

/**
 * @brief Free a request entry
 * @param req Pointer to request entry to be deleted
 */
void
scorep_mpi_request_free( scorep_mpi_request* req );
void
scorep_mpi_check_request( scorep_mpi_request* req,
                          MPI_Status*         status );
void
scorep_mpi_save_request_array( MPI_Request* arr_req,
                               int          arr_req_size );
scorep_mpi_request*
scorep_mpi_saved_request_get( int i );

/**
 * @internal
 * size of hash table
 */
#define SCOREP_MPI_REQUEST_TABLE_SIZE 256

/**
 * @internal
 * Hash access structure
 */
struct scorep_mpi_request_hash
{
    struct scorep_mpi_request_block* head_block;
    struct scorep_mpi_request_block* last_block;
    scorep_mpi_request*              lastreq;
    int                              lastidx;
};

/**
 * @internal
 * size of element list behind a hash entry
 */
#define SCOREP_MPI_REQUEST_BLOCK_SIZE 16

/**
 * @internal
 * Block of linked list of request blocks
 */
struct scorep_mpi_request_block
{
    scorep_mpi_request               req[ SCOREP_MPI_REQUEST_BLOCK_SIZE ];
    struct scorep_mpi_request_block* next;
    struct scorep_mpi_request_block* prev;
};


#endif /* SCOREP_MPI_REQUEST_H */
