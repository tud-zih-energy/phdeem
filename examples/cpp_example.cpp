#include <mpi.h>

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
        
    }

    MPI_Finalize( );

    return 0;
}
