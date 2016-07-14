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

#pragma once

#include <cstdint>
#include <mpi.h>
#include <stdexcept>

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
        : m_root( false ), hdeem_data( ), hdeem_global( ), hdeem_stats_reading( ), hdeem_stats( ),
          internal_status( ), return_values( )
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

        phdeem_return_value start( )
        {
            return map_to_enum_class( phdeem_start( &hdeem_data,
                                                    &internal_status,
                                                    &return_values ) );
        }

        phdeem_return_value stop( )
        {
            return map_to_enum_class( phdeem_stop( &hdeem_data,
                                                   &internal_status,
                                                   &return_values ) );
        }

        phdeem_return_value check_status( )
        {
            return map_to_enum_class( phdeem_check_status( &hdeem_data,
                                                           &hdeem_stats,
                                                           &internal_status,
                                                           &return_values ) );
        }

        phdeem_return_value get_global( )
        {
            return map_to_enum_class( phdeem_get_global( &hdeem_data,
                                                         &hdeem_global,
                                                         &internal_status,
                                                         &return_values ) );
        }

        phdeem_return_value get_stats( )
        {
            return map_to_enum_class( phdeem_get_stats( &hdeem_data,
                                                        &hdeem_stats_reading,
                                                        &internal_status,
                                                        &return_values ) );
        }

        phdeem_return_value data_free( )
        {
            return map_to_enum_class( phdeem_data_free( &hdeem_global,
                                                        &internal_status,
                                                        &return_values ) );
        }

        phdeem_return_value stats_free( )
        {
            return map_to_enum_class( phdeem_stats_free( &hdeem_stats_reading,
                                                         &internal_status,
                                                         &return_values ) );
        }

        phdeem_return_value clear( )
        {
            return map_to_enum_class( phdeem_clear( &hdeem_data,
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

        hdeem_global_reading_t get_hdeem_global( ) const
        {
            return hdeem_global;
        }

        hdeem_stats_reading_t get_hdeem_stats_reading( ) const
        {
            return hdeem_stats_reading;
        }

        hdeem_status_t get_hdeem_status( ) const
        {
            return hdeem_stats;
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
                    return phdeem_return_value::MPI_ERROR;
                default:
                    throw std::invalid_argument( "cpphdeem: Unrecognized return value in "
                                                 "'map_to_enum_class'" );
            }
        }

        bool m_root;

        hdeem_bmc_data_t hdeem_data;
        hdeem_global_reading_t hdeem_global;
        hdeem_stats_reading_t hdeem_stats_reading;
        hdeem_status_t hdeem_stats;
        phdeem_info_t internal_status;
        phdeem_status_t return_values;
    };
}
