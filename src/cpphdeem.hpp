#pragma once

#include <mpi.h>
#include <cstdint>

extern "C"
{
#include "hdeem.h"
#include "phdeem.h"
}

namespace cpphdeem
{
    /**
     * Stores the return values from phdeem.
     * 
     * This is just a type safe mapping from the original phdeem_return_values.
     */
    enum class phdeem_return_value : std::uint8_t
    {
        SUCCESS,
        NOT_ROOT,
        HDEEM_ERROR,
        MPI_ERROR,
    };

    class connection
    {
    public:
        connection( )
        : m_root( false ), hdeem_data( ), internal_status( ), return_values( )
        { }

        ~connection( )
        { }

        phdeem_return_value init( MPI_Comm current_comm )
        {
            phdeem_return_value ret = map_to_enum_class( phdeem_init( &hdeem_data,
                                                                      &internal_status,
                                                                      current_comm,
                                                                      &return_values ) );

            if( ret != phdeem_return_value::NOT_ROOT )
            {
                m_root = true;
            }

            return ret;
        }

        phdeem_return_value close( )
        {
            return map_to_enum_class( phdeem_close( &hdeem_data,
                                                    &internal_status,
                                                    &return_values ) );
        }

        inline bool root( ) const
        {
            return m_root;
        }

        hdeem_bmc_data_t& get_hdeem_data( )
        {
            return hdeem_data;
        }

        phdeem_status_t get_return_values( ) const
        {
            return return_values;
        }

    private:
        inline phdeem_return_value map_to_enum_class( int ret_val ) const
        {
            switch( ret_val )
            {
                case 0:
                    return phdeem_return_value::SUCCESS;
                case 1:
                    return phdeem_return_value::NOT_ROOT;
                case 2:
                    return phdeem_return_value::HDEEM_ERROR;
                case 3:
                default:
                    return phdeem_return_value::MPI_ERROR;
            }
        }

        bool m_root;

        hdeem_bmc_data_t hdeem_data;
        phdeem_info_t internal_status;
        phdeem_status_t return_values;
    };
}
