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
        conn.start( );
        conn.get_global( );
        usleep( 100000 );
        conn.get_global( );
        auto data = conn.get_hdeem_global( );

        std::cout << "Number of samples:" << data.nb_blade_values << std::endl;

        conn.stop( );
    }

    MPI_Finalize( );

    return 0;
}
