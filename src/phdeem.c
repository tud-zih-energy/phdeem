/**
 * Compile with:
 *  1. mpicc -fPIC -Wall -Werror -I../hdeem_2.1.5 -lfreeipmi -L../hdeem_2.1.5 -lhdeem -c phdeem.c -o phdeem.o
 *
 *  2. mpicc -shared -Wl,-soname,libphdeem.so -o libphdeem.so phdeem.o
 */
#include <mpi.h>
#include <time.h>

#include "hdeem.h"
#include "phdeem.h"

unsigned int _phdeem_hash( char* str )
{
    unsigned int hash = 0;
    int c;

    while( ( c = *str++ ) )
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

int phdeem_init( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller, MPI_Comm current_comm )
{
    // Reset error type
    phdeem_last_error_type = PHDEEM_NO_ERROR;

    int resultlen;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    // Get, hash and store the hostname
    phdeem_ret_store = MPI_Get_processor_name( hostname, &resultlen );
    if( phdeem_ret_store != MPI_SUCCESS )
    {
        phdeem_last_error_type = PHDEEM_MPI_ERROR;
        return phdeem_ret_store;
    }
    caller->node_hash = _phdeem_hash( hostname );

    // Get the current rank
    phdeem_ret_store = MPI_Comm_rank( current_comm, &caller->node_rank );
    if( phdeem_ret_store != MPI_SUCCESS )
    {
        phdeem_last_error_type = PHDEEM_MPI_ERROR;
        return phdeem_ret_store;
    }

    // Split the communicator
    phdeem_ret_store = MPI_Comm_split( current_comm, caller->node_hash, caller->node_rank,
                    &caller->sub_comm );
    if( phdeem_ret_store != MPI_SUCCESS )
    {
        phdeem_last_error_type = PHDEEM_MPI_ERROR;
        return phdeem_ret_store;
    }

    // Save the caller's new rank into the struct
    phdeem_ret_store = MPI_Comm_rank( caller->sub_comm, &caller->node_rank );
    if( phdeem_ret_store != MPI_SUCCESS )
    {
        phdeem_last_error_type = PHDEEM_MPI_ERROR;
        return phdeem_ret_store;
    }

    // If the split leads to the position where the caller is not root, exit.
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, initialize the HDEEM library
    phdeem_ret_store = hdeem_init( hdeem_data );

    // Errors?
    if( phdeem_ret_store != 0 )
    {
        phdeem_last_error_type = PHDEEM_HDEEM_ERROR;
        return phdeem_ret_store;
    }

    // Else return success
    return PHDEEM_SUCCESS;
}

int phdeem_close( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_close() and return
    hdeem_close( hdeem_data );
    return PHDEEM_SUCCESS;
}

int phdeem_start( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_start() and return w/ or w/o error
    phdeem_ret_store = hdeem_start( hdeem_data );
    if( phdeem_ret_store != 0 )
    {
        phdeem_last_error_type = PHDEEM_HDEEM_ERROR;
        return phdeem_ret_store;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_stop( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_stop() and return w/ or w/o error
    phdeem_ret_store = hdeem_stop( hdeem_data );
    if( phdeem_ret_store != 0 )
    {
        phdeem_last_error_type = PHDEEM_HDEEM_ERROR;
        return phdeem_ret_store;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_check_status( hdeem_bmc_data_t* hdeem_data, hdeem_status_t* hdeem_stats,
                         caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_check_status() and return w/ or w/o error
    phdeem_ret_store = hdeem_check_status( hdeem_data, hdeem_stats );
    if( phdeem_ret_store != 0 )
    {
        phdeem_last_error_type = PHDEEM_HDEEM_ERROR;
        return phdeem_ret_store;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_get_global( hdeem_bmc_data_t* hdeem_data, hdeem_global_reading_t* hdeem_read,
                       caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_get_global() and return w/ or w/o error
    phdeem_ret_store = hdeem_get_global( hdeem_data, hdeem_read );
    if( phdeem_ret_store != 0 )
    {
        phdeem_last_error_type = PHDEEM_HDEEM_ERROR;
        return phdeem_ret_store;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_get_stats( hdeem_bmc_data_t* hdeem_data, hdeem_stats_reading_t* hdeem_read,
                      caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_get_stats() and return w/ or w/o error
    phdeem_ret_store = hdeem_get_stats( hdeem_data, hdeem_read );
    if( phdeem_ret_store != 0 )
    {
        phdeem_last_error_type = PHDEEM_HDEEM_ERROR;
        return phdeem_ret_store;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_data_free( hdeem_global_reading_t* hdeem_read, caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_data_free() and return
    hdeem_data_free( hdeem_read );
    return PHDEEM_SUCCESS;
}

int phdeem_stats_free( hdeem_stats_reading_t* hdeem_read, caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_stats_free() and return
    hdeem_stats_free( hdeem_read );
    return PHDEEM_SUCCESS;
}

int phdeem_clear( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller )
{
    phdeem_last_error_type = PHDEEM_NO_ERROR;
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        phdeem_last_error_type = PHDEEM_PHDEEM_ERROR;
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_clear() and return w/ or w/o error
    phdeem_ret_store = hdeem_clear( hdeem_data );
    if( phdeem_ret_store != 0 )
    {
        phdeem_last_error_type = PHDEEM_HDEEM_ERROR;
        return phdeem_ret_store;
    }
    
    return PHDEEM_SUCCESS;
}
