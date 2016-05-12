#ifndef PHDEEM_H
#define PHDEEM_H

#include <mpi.h>
#include <time.h>

#include "hdeem.h"

/**
 * Stores necessary information about the calling process.
 * 
 * You just need to pass an empty phdeem_info_t to the function calls.
 */
typedef struct phdeem_info phdeem_info_t;

/**
 * Stores the return values from hdeem and MPI.
 */
typedef struct phdeem_int_ret_value
{
    int mpi_ret_value;
    int hdeem_ret_value;
} phdeem_int_ret_value_t;

/**
 * Return values.
 *
 * When no errors occurred, PHDEEM_SUCCESS will be returned. If the caller isn't the root process,
 * PHDEEM_NOT_ROOT will be returned. On errors, PHDEEM_HDEEM_ERROR or PHDEEM_MPI_ERROR resp. will be
 * returned. On errors, take a look at the phdeem_int_ret_value_t passed.
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
 * @param caller        An empty phdeem_info_t. All necessary information will be stored in this
 *                      struct.
 * @param current_comm  The process' current MPI communicator.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_init( hdeem_bmc_data_t *const hdeem_data, phdeem_info_t *const caller,
                 MPI_Comm current_comm, phdeem_int_ret_value_t *const ret_val );

/**
 * Finalizes the phdeem library.
 * 
 * Calls hdeem_close.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_close().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_close( hdeem_bmc_data_t *const hdeem_data, phdeem_info_t *const caller,
                  phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_start().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_start().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_start( hdeem_bmc_data_t *const hdeem_data, const phdeem_info_t *const caller,
                  phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_stop().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_stop().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_stop( hdeem_bmc_data_t *const hdeem_data, const phdeem_info_t *const caller,
                 phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_check_status().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_check_status().
 * @param hdeem_stats   Information that otherwise would have been passed to hdeem_check_status().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_check_status( hdeem_bmc_data_t *const hdeem_data, hdeem_status_t *const hdeem_stats,
                         const phdeem_info_t *const caller, phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_get_global().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_get_global().
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_get_global().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_get_global( hdeem_bmc_data_t *const hdeem_data, hdeem_global_reading_t *const hdeem_read,
                       const phdeem_info_t *const caller, phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_get_stats().
 * 
 * For further information please read the hdeem.h code comments. Seems to do something french.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_get_stats().
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_get_stats().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_get_stats( hdeem_bmc_data_t *const hdeem_data, hdeem_stats_reading_t *const hdeem_read,
                      const phdeem_info_t *const caller, phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_data_free().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_data_free().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_data_free( hdeem_global_reading_t *const hdeem_read, const phdeem_info_t *const caller,
                      phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_stats_free().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_stats_free().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_stats_free( hdeem_stats_reading_t *const hdeem_read, const phdeem_info_t *const caller,
                       phdeem_int_ret_value_t *const ret_val );

/**
 * Calls hdeem_clear().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_clear().
 * @param caller        phdeem_info_t holding the caller's information.
 * @param ret_val       The phdeem_int_ret_value_t the return values are stored in.
 *
 * @return              A phdeem return value.
 */
int phdeem_clear( hdeem_bmc_data_t *const hdeem_data, const phdeem_info_t *const caller,
                  phdeem_int_ret_value_t *const ret_val );

#endif /* PHDEEM_H */
