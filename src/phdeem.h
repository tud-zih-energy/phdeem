#ifndef PHDEEM_H
#define PHDEEM_H

#define PHDEEM_NOT_ROOT -127
#define PHDEEM_SUCCESS 0

#include <mpi.h>
#include <time.h>

#include "hdeem.h"

/**
 * Stores necessary information about the calling process.
 */
typedef struct caller_info
{
    /** The hash of the name of the host of the caller */
    unsigned int node_hash;
    /** The rank of the caller on it's host */
    int node_rank;
    /** The sub communicator the caller is in */
    MPI_Comm sub_comm;
} caller_info_t;

/**
 * Error types.
 *
 * The value of phdeem_last_error_type defines which part of the library caused the return value of
 * the last called function.
 */
enum _phdeem_error_types
{
    PHDEEM_NO_ERROR         = 0,
    PHDEEM_PHDEEM_ERROR     = 1,
    PHDEEM_HDEEM_ERROR      = 2,
    PHDEEM_MPI_ERROR        = 3
};


/**
 * @see _phdeem_error_types
 */
int phdeem_last_error_type = PHDEEM_NO_ERROR;

/**
 * Integer to store return values in.
 */
int phdeem_ret_store = 0;

/**
 * Gives an unsigned int hash for a given string.
 *
 * It uses the hash function from sdbm found at <http://www.cse.yorku.ca/~oz/hash.html>.
 *
 * @param str   The string to hash.
 *
 * @return      The hash.
 */
unsigned int _phdeem_hash( char* str );

/**
 * Initializes the phdeem library.
 *
 * This function creates a MPI subcommunicator for each node the program is run on and groups all
 * processes in their corresponding communicators. Afterwards hdeem_init() is called for the root
 * processes in the new communicators.
 *
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_init().
 * @param caller        An empty caller_info_t. All necessary information will be stored in this
 *                      struct.
 * @param current_comm  The process' current MPI communicator.
 *
 * @return              A MPI error code (phdeem_error_type = PHDEEM_MPI_ERROR), a hdeem error code
 *                      (phdeem_error_type = PHDEEM_HDEEM_ERROR), PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_init( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller, MPI_Comm current_comm );

/**
 * Finalizes the phdeem library.
 * 
 * Calls hdeem_close.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_close().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_close( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller );

/**
 * Calls hdeem_start().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_start().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              A hdeem error code (phdeem_error_type = PHDEEM_HDEEM_ERROR) in case of any
 *                      errors or else PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_start( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller );

/**
 * Calls hdeem_stop().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_stop().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              A hdeem error code (phdeem_error_type = PHDEEM_HDEEM_ERROR) in case of any
 *                      errors or else PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_stop( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller );

/**
 * Calls hdeem_check_status().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_check_status().
 * @param hdeem_stats   Information that otherwise would have been passed to hdeem_check_status().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              A hdeem error code (phdeem_error_type = PHDEEM_HDEEM_ERROR) in case of any
 *                      errors or else PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_check_status( hdeem_bmc_data_t* hdeem_data, hdeem_status_t* hdeem_stats,
                         caller_info_t* caller );

/**
 * Calls hdeem_get_global().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_get_global().
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_get_global().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              A hdeem error code (phdeem_error_type = PHDEEM_HDEEM_ERROR) in case of any
 *                      errors or else PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_get_global( hdeem_bmc_data_t* hdeem_data, hdeem_global_reading_t* hdeem_read,
                       caller_info_t* caller );

/**
 * Calls hdeem_get_stats().
 * 
 * For further information please read the hdeem.h code comments. Seems to do something french.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_get_stats().
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_get_stats().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              A hdeem error code (phdeem_error_type = PHDEEM_HDEEM_ERROR) in case of any
 *                      errors or else PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_get_stats( hdeem_bmc_data_t* hdeem_data, hdeem_stats_reading_t* hdeem_read,
                      caller_info_t* caller );

/**
 * Calls hdeem_data_free().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_data_free().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_data_free( hdeem_global_reading_t* hdeem_read, caller_info_t* caller );

/**
 * Calls hdeem_stats_free().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_read    Information that otherwise would have been passed to hdeem_stats_free().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_stats_free( hdeem_stats_reading_t* hdeem_read, caller_info_t* caller );

/**
 * Calls hdeem_clear().
 * 
 * For further information please read the hdeem.h code comments.
 * 
 * @param hdeem_data    Information that otherwise would have been passed to hdeem_clear().
 * @param caller        caller_info_t holding the caller's information.
 * 
 * @return              A hdeem error code (phdeem_error_type = PHDEEM_HDEEM_ERROR) in case of any
 *                      errors or else PHDEEM_NOT_ROOT or PHDEEM_SUCCESS.
 */
int phdeem_clear( hdeem_bmc_data_t* hdeem_data, caller_info_t* caller );

#endif /* PHDEEM_H */
