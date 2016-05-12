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

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

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
    phdeem_info_t caller;
    phdeem_int_ret_value_t int_rets;
    hdeem_bmc_data_t hdeem_data;
    hdeem_data.hasGPIO = 1;
    hdeem_data.host = "";
    hdeem_data.user = "";
    hdeem_data.password = "";

    ret = phdeem_init( &hdeem_data, &caller, MPI_COMM_WORLD, &int_rets );
    // Print result of initializing
    printf( "'Initializing' from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    // Call hdeem_start
    ret = phdeem_start( &hdeem_data, &caller, &int_rets );
    printf( "'Start'        from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    // If we're root and there were no errors, let's collect some data
    if( ret == 0 )
    {
        hdeem_global_reading_t readings;

        sleep(1);

        for( int i = 0; i < 2; ++i )
        {
            ret = phdeem_get_global( &hdeem_data, &readings, &caller, &int_rets );

            printf( "'get_global'   from processor %s, global rank %d. Return value was %d\n",
                    processor_name, world_rank, ret );

            printf( "\n%s, read %llu values\n--------------\n", hdeem_data.name_blade_sensors[0],
                    readings.nb_blade_values);

            sleep(1);
        }

        phdeem_data_free( &readings, &caller, &int_rets );
        printf( "'Free data'    from processor %s, global rank %d. Return value was %d\n",
                processor_name, world_rank, ret );
    }

    // Collect some stats
    hdeem_stats_reading_t stats;
    ret = phdeem_get_stats( &hdeem_data, &stats, &caller, &int_rets );
    printf( "'Read stats'   from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    // Free the stats
    ret = phdeem_stats_free( &stats, &caller, &int_rets );
    printf( "'Free stats'   from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    // Call hdeem_stop
    ret = phdeem_stop( &hdeem_data, &caller, &int_rets );
    printf( "'Stop'         from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    // Check the hdeem status
    hdeem_status_t status;
    ret = phdeem_check_status( &hdeem_data, &status, &caller, &int_rets );
    printf( "'Check status' from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    // Clear the hdeem data
    ret = phdeem_clear( &hdeem_data, &caller, &int_rets );
    printf( "'Clear data'   from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    // Finalize phdeem
    ret = phdeem_close( &hdeem_data, &caller, &int_rets );
    printf( "'Finalizing'   from processor %s, global rank %d. Return value was %d\n",
            processor_name, world_rank, ret );

    MPI_Finalize( );

    return 0;
}
