/**
  Copyright (c) 2016, Technische Universität Dresden, Germany
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted
  provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this list of conditions
     and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this list of
     conditions and the following disclaimer in the documentation and/or other materials provided
     with the distribution.

  3. Neither the name of the copyright holder nor the names of its contributors may be used to
     endorse or promote products derived from this software without specific prior written
     permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <hdeem.h>
#include "phdeem.h"
#include <mpi.h>
#include <time.h>


/**
 * Gives an unsigned int hash for a given string.
 *
 * It uses the hash function from sdbm found at <http://www.cse.yorku.ca/~oz/hash.html>.
 *
 * @param str   The string to hash.
 *
 * @return      The hash.
 */
static unsigned int _phdeem_hash( const char* str )
{
    unsigned int hash = 0;
    int c;

    while( ( c = *str++ ) )
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

int phdeem_init( hdeem_bmc_data_t* hdeem_data, phdeem_info_t* info, MPI_Comm current_comm,
                 phdeem_status_t* ret_val )
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
    info->node_hash = _phdeem_hash( hostname );

    // Get the current rank
    ret_val->mpi_ret_value = MPI_Comm_rank( current_comm, &info->node_rank );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }

    // Split the communicator
    ret_val->mpi_ret_value = MPI_Comm_split( current_comm, info->node_hash, info->node_rank,
                    &info->sub_comm );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }

    // Save the caller's new rank into the struct
    ret_val->mpi_ret_value = MPI_Comm_rank( info->sub_comm, &info->node_rank );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }

    // If the split leads to the position where the caller is not root, exit.
    if( info->node_rank != 0 )
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

int phdeem_close( hdeem_bmc_data_t* hdeem_data, phdeem_info_t* info, phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_close() and return
    hdeem_close( hdeem_data );

    // Free the node local communicator
    ret_val->mpi_ret_value = MPI_Comm_free( &info->sub_comm );
    if( ret_val->mpi_ret_value != MPI_SUCCESS )
    {
        return PHDEEM_MPI_ERROR;
    }

    // Nobody should be root after this anymore.
    info->node_rank = -1;
    return PHDEEM_SUCCESS;
}

int phdeem_start( hdeem_bmc_data_t* hdeem_data, const phdeem_info_t* info,
                  phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
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

int phdeem_stop( hdeem_bmc_data_t* hdeem_data, const phdeem_info_t* info,
                 phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
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

int phdeem_check_status( hdeem_bmc_data_t* hdeem_data, hdeem_status_t* hdeem_stats,
                         const phdeem_info_t* info, phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
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

int phdeem_get_global( hdeem_bmc_data_t* hdeem_data, hdeem_global_reading_t* hdeem_read,
                       const phdeem_info_t* info, phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
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

int phdeem_get_stats( hdeem_bmc_data_t* hdeem_data, hdeem_stats_reading_t* hdeem_read,
                      const phdeem_info_t* info, phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
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

int phdeem_data_free( hdeem_global_reading_t* hdeem_read, const phdeem_info_t* info,
                      phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_data_free() and return
    hdeem_data_free( hdeem_read );
    return PHDEEM_SUCCESS;
}

int phdeem_stats_free( hdeem_stats_reading_t* hdeem_read, const phdeem_info_t* info,
                       phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
    {
        return PHDEEM_NOT_ROOT;
    }

    // Else, call hdeem_stats_free() and return
    hdeem_stats_free( hdeem_read );
    return PHDEEM_SUCCESS;
}

int phdeem_clear( hdeem_bmc_data_t* hdeem_data, const phdeem_info_t* info,
                  phdeem_status_t* ret_val )
{
    // Reset the return values
    ret_val->hdeem_ret_value = 0;
    ret_val->mpi_ret_value = MPI_SUCCESS;

    // If we're not root, exit immediately
    if( info->node_rank != 0 )
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
