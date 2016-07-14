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
