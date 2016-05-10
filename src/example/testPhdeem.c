/**
 * Compile with:
 * 
 *  1. mpicc -fPIC -Wall -Werror -I.. -I../../hdeem_2.1.5 -lfreeipmi -L.. -lphdeem -L../../hdeem_2.1.5 -lhdeem testPhdeem.c
 */

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define min(a,b) ( (a<b) ? a : b)

#include "phdeem.h"

int main( int argc, char** argv )
{
    // Initializing MPI
    MPI_Init( &argc, &argv );

    int world_rank, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name(processor_name, &name_len);

    // Initializing phdeem
    int ret;
    caller_info_t caller;
    hdeem_bmc_data_t hdeem_data;
    hdeem_data.hasGPIO = 1;
    hdeem_data.host = "";
    hdeem_data.user = "";
    hdeem_data.password = "";
    
    ret = phdeem_init( &hdeem_data, &caller, MPI_COMM_WORLD );
    // Print result of initializing
    printf( "'Initializing' from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    // Call hdeem_start
    ret = phdeem_start( &hdeem_data, &caller );
    printf( "'Start'        from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    // If we're root and there were no errors, let's collect some data
    if( ret == 0 )
    {
        hdeem_global_reading_t readings;
        int i, j = 0;
        
        sleep(1);
        
        while( j < 2 )
        {
            ret = phdeem_get_global( &hdeem_data, &readings, &caller );
            
            printf( "'get_global'   from processor %s, global rank %d. Return value was %d with following "
                    "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);
    
            printf( "\n%s, %llu values\n--------------\n", hdeem_data.name_blade_sensors[0],
                                                           readings.nb_blade_values);
            // Gratefully lend from the hdeem sample C file.
            for( i = 1; i <= min( 20000, readings.nb_blade_values ); i++ )
            {
                if( i % 10 == 1 )
                    printf( "\n%8lld : ", i + readings.first_blade_value - 1 );
                printf( "  %9.3f", readings.blade_power[i-1].value[0] );
            }
            printf( "\n" );
            
            sleep(1);
            j += 1;
        }
        
        phdeem_data_free( &readings, &caller );
        printf( "'Free data'    from processor %s, global rank %d. Return value was %d with following "
                "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);
    }

    // Collect some stats
    hdeem_stats_reading_t stats;
    ret = phdeem_get_stats( &hdeem_data, &stats, &caller );
    printf( "'Read stats'   from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    // Free the stats
    ret = phdeem_stats_free( &stats, &caller );
    printf( "'Free stats'   from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    // Call hdeem_stop
    ret = phdeem_stop( &hdeem_data, &caller );
    printf( "'Stop'         from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    // Check the hdeem status
    hdeem_status_t status;
    ret = phdeem_check_status( &hdeem_data, &status, &caller );
    printf( "'Check status' from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    // Clear the hdeem data
    ret = phdeem_clear( &hdeem_data, &caller );
    printf( "'Clear data'   from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    // Finalize phdeem
    ret = phdeem_close( &hdeem_data, &caller );
    printf( "'Finalizing'   from processor %s, global rank %d. Return value was %d with following "
            "error type: %d\n", processor_name, world_rank, ret, phdeem_last_error_type);

    MPI_Finalize( );
    
    return 0;
}
