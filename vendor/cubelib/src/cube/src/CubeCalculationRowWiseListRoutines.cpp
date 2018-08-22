/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2017                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2015                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/


/**
 * \file Cube.cpp
 * \brief Defines methods of the class cube and IO-interface.
 *
 */

// PGI compiler replaces all ofstream.open() calls by open64 and then cannot find a propper one. Result -> compilation error
#if !defined( __PGI ) && !defined( __CRAYXT )  && !defined( __CRAYXE )
#define _FILE_OFFSET_BITS 64
#endif


#include "config.h"
#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeValue.h"
#include "CubeMetric.h"
#include "CubeRegion.h"
#include "Cube.h"
#include "CubeServices.h"

using namespace std;
using namespace cube;






// --------------- calls with lists--------------------------------------------------------------------------------




Value**
Cube::get_sevs_adv( const list_of_metrics& metrics,
                    const list_of_cnodes&  cnodes )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sevs_adv(list_of_metrics, list_of_cnodes): No metrics are spezified. Empty list." );
    }
    if ( cnodes.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sevs_adv(list_of_metrics, list_of_cnodes): No cnodes are spezified. Empty list." );
    }
    Value**  value = NULL;
    unsigned flag  = 0;
    for ( unsigned i = 0; i < metrics.size(); i++, flag++ )
    {
        for ( unsigned j = 0; j < cnodes.size(); j++, flag++ )
        {
            if ( value == NULL )
            {
                value = get_sevs_adv( metrics[ i ].first, metrics[ i ].second,  cnodes[ j ].first, cnodes[ j ].second );
            }
            else
            {
                Value** _v =  get_sevs_adv( metrics[ i ].first, metrics[ i ].second,  cnodes[ j ].first, cnodes[ j ].second );
                for ( size_t i = 0; i < locationv.size(); ++i )
                {
                    Value* __v = _v[ i ];
                    if ( flag == 0 )
                    {
                        value[ i ] = __v;
                    }
                    else
                    {
                        if ( __v != NULL )
                        {
                            value[ i ]->
                            operator+=( __v );
                        }
                    }
                }
                if ( flag != 0 )
                {
                    services::delete_row_of_values( _v, locationv.size() );
                }
            }
        }
    }
    return value;
}


double*
Cube::get_sevs( list_of_metrics& metrics,
                list_of_cnodes&  cnodes )
{
    Value** v     = get_sevs_adv( metrics, cnodes );
    double* v_ret = services::transform_values_to_doubles( v, locationv.size() );
    services::delete_row_of_values( v, locationv.size() );       // == delete[] _v;
    return v_ret;
}

Value**
Cube::get_sevs_adv( list_of_metrics& metrics )
{
    list_of_cnodes list_of_roots;
    for ( std::vector<Cnode*>::iterator citer = root_cnodev.begin(); citer != root_cnodev.end(); ++citer )
    {
        std::pair<Cnode*, CalculationFlavour> _pair( *citer, cube::CUBE_CALCULATE_INCLUSIVE );
        list_of_roots.push_back( _pair );
    }
    return get_sevs_adv( metrics, list_of_roots );
}

double*
Cube::get_sevs( list_of_metrics& metrics )
{
    Value** v     = get_sevs_adv( metrics );
    double* v_ret = services::transform_values_to_doubles( v, locationv.size() );
    services::delete_row_of_values( v, locationv.size() );    // == delete[] _v;
    return v_ret;
}
