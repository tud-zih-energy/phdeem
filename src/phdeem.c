/**
 * Compile with: 
 */
#include <mpi.h>
#include <time.h>

#include "hdeem.h"
#include "phdeem.h"

unsigned int _phdeem_hash( char* str )
{
    unsigned int hash = 0;
    int c;

    while( c = *str++ )
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

int phdeem_init( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller, MPI_Comm current_comm )
{
    int resultlen;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    // Get, hash and store the hostname
    MPI_Get_processor_name( hostname, &resultlen );
    caller->node_hash = _phdeem_hash( hostname );

    // Get the current rank
    MPI_Comm_rank( current_comm, &caller->node_rank );

    // Split the communicator
    MPI_Comm_split( current_comm, caller->node_hash, caller->node_rank,
                    &caller->sub_comm );
    
    // Save the caller's new rank into the struct
    MPI_Comm_rank( caller->sub_comm, &caller->node_rank );
                    
    // If the split leads to the position where the caller is not root, exit.
    if( caller->node_rank != 0 )
    {
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
    // If we're not root, exit immediately
    if( caller->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }
    
    // Else, call hdeem_close() and return
    hdeem_close( hdeem_data );
    return PHDEEM_SUCCESS;
}
