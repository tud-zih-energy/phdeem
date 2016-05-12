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

struct phdeem_info
{
    /** The hash of the name of the host of the caller */
    unsigned int node_hash;
    /** The rank of the caller on it's host */
    int node_rank;
    /** The sub communicator the caller is in */
    MPI_Comm sub_comm;
};

/**
 * Gives an unsigned int hash for a given string.
 *
 * It uses the hash function from sdbm found at <http://www.cse.yorku.ca/~oz/hash.html>.
 *
 * @param str   The string to hash.
 *
 * @return      The hash.
 */
unsigned int _phdeem_hash( const char* str )
{
    unsigned int hash = 0;
    int c;

    while( ( c = *str++ ) )
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

int phdeem_init( hdeem_bmc_data_t *const hdeem_data, phdeem_info_t *const caller,
                 MPI_Comm current_comm, phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    int resultlen;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    // Get, hash and store the hostname
    ret_val->mpi_ret_value = MPI_Get_processor_name( hostname, &resultlen );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }
    caller->node_hash = _phdeem_hash( hostname );

    // Get the current rank
    ret_val->mpi_ret_value = MPI_Comm_rank( current_comm, &caller->node_rank );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }

    // Split the communicator
    ret_val->mpi_ret_value = MPI_Comm_split( current_comm, caller->node_hash, caller->node_rank,
                    &caller->sub_comm );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }

    // Save the caller's new rank into the struct
    ret_val->mpi_ret_value = MPI_Comm_rank( caller->sub_comm, &caller->node_rank );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }

    // If the split leads to the position where the caller is not root, exit.
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, initialize the HDEEM library
    ret_val->hdeem_ret_value = hdeem_init( hdeem_data );

    // Errors?
    if( ret_val->hdeem_ret_value != 0 )
    {
        return PHDEEM_HDEEM_ERROR;
    }

    // Else return success
    return PHDEEM_SUCCESS;
}

int phdeem_close( hdeem_bmc_data_t *const hdeem_data, phdeem_info_t *const caller,
                  phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_close() and return
    hdeem_close( hdeem_data );
    
    // Free the node local communicator
    ret_val->mpi_ret_value = MPI_Comm_free( &caller->sub_comm );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }
    
    // Nobody should be root anymore after this.
    caller->node_rank = -1;
    return PHDEEM_SUCCESS;
}

int phdeem_start( hdeem_bmc_data_t *const hdeem_data, const phdeem_info_t *const caller,
                  phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_start() and return w/ or w/o error
    ret_val->hdeem_ret_value = hdeem_start( hdeem_data );
    if( ret_val->hdeem_ret_value != 0 )
    {
        return PHDEEM_HDEEM_ERROR;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_stop( hdeem_bmc_data_t *const hdeem_data, const phdeem_info_t *const caller,
                 phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_stop() and return w/ or w/o error
    ret_val->hdeem_ret_value = hdeem_stop( hdeem_data );
    if( ret_val->hdeem_ret_value != 0 )
    {
        return PHDEEM_HDEEM_ERROR;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_check_status( hdeem_bmc_data_t *const hdeem_data, hdeem_status_t *const hdeem_stats,
                         const phdeem_info_t *const caller, phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_check_status() and return w/ or w/o error
    ret_val->hdeem_ret_value = hdeem_check_status( hdeem_data, hdeem_stats );
    if( ret_val->hdeem_ret_value != 0 )
    {
        return PHDEEM_HDEEM_ERROR;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_get_global( hdeem_bmc_data_t *const hdeem_data, hdeem_global_reading_t *const hdeem_read,
                       const phdeem_info_t *const caller, phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_get_global() and return w/ or w/o error
    ret_val->hdeem_ret_value = hdeem_get_global( hdeem_data, hdeem_read );
    if( ret_val->hdeem_ret_value != 0 )
    {
        return PHDEEM_HDEEM_ERROR;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_get_stats( hdeem_bmc_data_t *const hdeem_data, hdeem_stats_reading_t *const hdeem_read,
                      const phdeem_info_t *const caller, phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_get_stats() and return w/ or w/o error
    ret_val->hdeem_ret_value = hdeem_get_stats( hdeem_data, hdeem_read );
    if( ret_val->hdeem_ret_value != 0 )
    {
        return PHDEEM_HDEEM_ERROR;
    }
    
    return PHDEEM_SUCCESS;
}

int phdeem_data_free( hdeem_global_reading_t *const hdeem_read, const phdeem_info_t *const caller,
                      phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_data_free() and return
    hdeem_data_free( hdeem_read );
    return PHDEEM_SUCCESS;
}

int phdeem_stats_free( hdeem_stats_reading_t *const hdeem_read, const phdeem_info_t *const caller,
                       phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_stats_free() and return
    hdeem_stats_free( hdeem_read );
    return PHDEEM_SUCCESS;
}

int phdeem_clear( hdeem_bmc_data_t *const hdeem_data, const phdeem_info_t *const caller,
                  phdeem_int_ret_value_t *const ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;
    
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_clear() and return w/ or w/o error
    ret_val->hdeem_ret_value = hdeem_clear( hdeem_data );
    if( ret_val->hdeem_ret_value != 0 )
    {
        return PHDEEM_HDEEM_ERROR;
    }
    
    return PHDEEM_SUCCESS;
}
