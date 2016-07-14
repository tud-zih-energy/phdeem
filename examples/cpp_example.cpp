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

#include <iostream>
#include <mpi.h>
#include <unistd.h>

#include "cpphdeem.hpp"

int main( int argc, char** argv )
{
    // Initializing MPI
    MPI_Init( &argc, &argv );

    int world_rank, name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name(processor_name, &name_len);

    // Use the C++ interface
    cpphdeem::connection conn;
    conn.init( MPI_COMM_WORLD );

    // If we're not root, we don't need to call the following functions
    if( conn.root( ) )
    {
        // Start measurement
        conn.start( );

        for( int i = 0; i <= 2; ++i )
        {
            // Wait a little for data to accumulate
            usleep( 1000000 );

            // Read data
            conn.get_global( );

            auto data = conn.get_hdeem_data( );
            auto readings = conn.get_hdeem_global( );

            std::cout << "Node: " << data.name_blade_sensors[0]
                      << "Number of samples:" << readings.nb_blade_values
                      << std::endl;
        }

        // Finalize the connection
        conn.stop( );
        conn.clear( );
    }

    // Finalize MPI
    MPI_Finalize( );

    return 0;
}
