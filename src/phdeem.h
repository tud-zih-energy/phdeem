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

#ifndef PHDEEM_H
#define PHDEEM_H

#include <hdeem.h>
#include <mpi.h>

#include <time.h>


/**
 * Stores necessary information about the calling process.
 *
 * You just need to pass an empty phdeem_info_t to the init function call.
 */
typedef struct phdeem_info
{
    /** The hash of the name of the host of the caller */
    unsigned int node_hash;
    /** The rank of the caller on it's host */
    int node_rank;
    /** The sub communicator the caller is in */
    MPI_Comm sub_comm;
} phdeem_info_t;

/**
 * Stores the return values from hdeem and MPI.
 */
typedef struct phdeem_status
{
    int mpi_ret_value;
    int hdeem_ret_value;
} phdeem_status_t;

/**
 * Return values.
 *
 * When no errors occurred, PHDEEM_SUCCESS will be returned. If the caller isn't the root process,
 * PHDEEM_NOT_ROOT will be returned. On errors, PHDEEM_HDEEM_ERROR or PHDEEM_MPI_ERROR resp. will be
 * returned. On errors, take a look at the phdeem_status_t passed.
 */
enum phdeem_return_values
{
    PHDEEM_SUCCESS          = 0,
    PHDEEM_NOT_ROOT         = 1,
    PHDEEM_HDEEM_ERROR      = 2,
    PHDEEM_MPI_ERROR        = 3
};

/**
 * Initializes the phdeem library.
 *
 * This function creates a MPI subcommunicator for each node the program is run on and groups all
 * processes in their corresponding communicators. Afterwards hdeem_init() is called for the root
 * processes in the new communicators.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_init().
 * @param info          An empty phdeem_info_t. All necessary information will be stored in this
 *                      struct.
 * @param current_comm  The process' current MPI communicator.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_init( hdeem_bmc_data_t* hdeem_data, phdeem_info_t* info, MPI_Comm current_comm,
                 phdeem_status_t* ret_val );

/**
 * Finalizes the phdeem library.
 *
 * Calls hdeem_close.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_close().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_close( hdeem_bmc_data_t* hdeem_data, phdeem_info_t* info, phdeem_status_t* ret_val );

/**
 * Calls hdeem_start().
 *
 * For further information please read the hdeem.h code comments.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_start().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_start( hdeem_bmc_data_t* hdeem_data, const phdeem_info_t* info,
                  phdeem_status_t* ret_val );

/**
 * Calls hdeem_stop().
 *
 * For further information please read the hdeem.h code comments.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_stop().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_stop( hdeem_bmc_data_t* hdeem_data, const phdeem_info_t* info,
                 phdeem_status_t* ret_val );

/**
 * Calls hdeem_check_status().
 *
 * For further information please read the hdeem.h code comments.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_check_status().
 * @param hdeem_stats   Information that otherwise would have been passed to hdeem_check_status().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_check_status( hdeem_bmc_data_t* hdeem_data, hdeem_status_t* hdeem_stats,
                         const phdeem_info_t* info, phdeem_status_t* ret_val );

/**
 * Calls hdeem_get_global().
 *
 * For further information please read the hdeem.h code comments.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_get_global().
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_get_global().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_get_global( hdeem_bmc_data_t* hdeem_data, hdeem_global_reading_t* hdeem_read,
                       const phdeem_info_t* info, phdeem_status_t* ret_val );

/**
 * Calls hdeem_get_stats().
 *
 * For further information please read the hdeem.h code comments. Seems to do something french.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_get_stats().
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_get_stats().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_get_stats( hdeem_bmc_data_t* hdeem_data, hdeem_stats_reading_t* hdeem_read,
                      const phdeem_info_t* info, phdeem_status_t* ret_val );

/**
 * Calls hdeem_data_free().
 *
 * For further information please read the hdeem.h code comments.
 *
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_data_free().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_data_free( hdeem_global_reading_t* hdeem_read, const phdeem_info_t* info,
                      phdeem_status_t* ret_val );

/**
 * Calls hdeem_stats_free().
 *
 * For further information please read the hdeem.h code comments.
 *
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_stats_free().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_stats_free( hdeem_stats_reading_t* hdeem_read, const phdeem_info_t* info,
                       phdeem_status_t* ret_val );

/**
 * Calls hdeem_clear().
 *
 * For further information please read the hdeem.h code comments.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_clear().
 * @param info          phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_status_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_clear( hdeem_bmc_data_t* hdeem_data, const phdeem_info_t* info,
                  phdeem_status_t* ret_val );

#endif /* PHDEEM_H */
