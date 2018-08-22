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

#include  <cstdlib>
#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeValue.h"
#include "CubeMetric.h"
#include "CubeRegion.h"
#include "Cube.h"
#include "CubeServices.h"

using namespace std;
using namespace cube;





// ------------------- ANALYSIS PART --------------------------

Value**
Cube::get_sevs_adv( Metric*            metric,
                    CalculationFlavour mf,
                    Cnode*             cnode,
                    CalculationFlavour cnf )
{
    Value** v = NULL;
    if ( metric->isBuildIn() )
    {
        char* _ret =  metric->get_sevs_raw( cnode, cnf );
        if ( _ret == NULL )
        {
            return NULL;
        }
        v = services::transform_raw_to_values( _ret, metric->its_value(), locationv.size() );
        services::delete_raw_row( _ret );
    }
    else
    {
        v = metric->get_sevs_adv( cnode, cnf );
    }

    if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
    {
        for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                            // recursiv call for  children of the metric
        {
            Metric* _met = metric->get_child( i );
            Value** _v   = get_sevs_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, cnode, cnf );
            if ( _v == NULL )
            {
                continue;
            }
            for ( size_t i = 0; i < locationv.size(); ++i )
            {
                Value* __v = _v[ i ];
                if ( __v == NULL )
                {
                    continue;
                }
                v[ i ]->
                operator-=( __v );
            }
            services::delete_row_of_values( _v, locationv.size() );
        }
    }
    return v;
}

double*
Cube::get_sevs( Metric*            metric,
                CalculationFlavour mf,
                Cnode*             cnode,
                CalculationFlavour cnf )
{
    if ( mf == CUBE_CALCULATE_INCLUSIVE && metric->isBuildIn() )
    {
        return metric->get_sevs( cnode, cnf );
    }
    Value** v     = get_sevs_adv( metric, mf, cnode, cnf );
    double* v_ret = services::transform_values_to_doubles( v, locationv.size() );
    services::delete_row_of_values( v, locationv.size() );
    return v_ret;
}



char*
Cube::get_sevs_raw( Metric*            metric,
                    CalculationFlavour mf,
                    Cnode*             cnode,
                    CalculationFlavour cnf )
{
    if ( mf == CUBE_CALCULATE_INCLUSIVE && metric->isBuildIn() )
    {
        char* _v     =  metric->get_sevs_raw( cnode, cnf );
        char* to_ret = services::create_raw_row( metric->its_value()->getSize() * locationv.size() ); // have to create copy
        if ( _v != NULL )
        {
            memcpy( to_ret, _v,  metric->its_value()->getSize() * locationv.size() );
        }
        return to_ret;
    }
    Value** v     = get_sevs_adv( metric, mf, cnode, cnf );
    char*   v_ret = services::transform_values_to_raw( v, metric->its_value(), locationv.size() );
    services::delete_row_of_values( v, locationv.size() );
    return v_ret;
}







char*
Cube::get_sevs_raw( Metric*            metric,
                    CalculationFlavour mf,
                    Region*            region,
                    CalculationFlavour rf )
{
    double* _v    = get_sevs( metric, mf, region, rf );
    char*   v_ret = services::transform_doubles_to_raw( _v );
    return v_ret;
}




Value**
Cube::get_sevs_adv( Metric*            metric,
                    CalculationFlavour mf,
                    Region*            region,
                    CalculationFlavour rf )
{
    if ( metric->isBuildIn() )
    {
        char*   _ret =  get_sevs_raw( metric, mf, region, rf );
        Value** v    = services::transform_raw_to_values( _ret, metric->its_value(), locationv.size() );
        services::delete_raw_row( _ret );  // == delete[] _v;
        return v;
    }
    else
    {
        CalculationFlavour _rf = rf;

        Value** value =  services::create_row_of_values( locationv.size() );  // === new Value*[locationv.size()];
        // collectiong cnodes, which have to be calculated for region
        std::vector<Cnode*> v;
        std::vector<Cnode*> cnodes = get_cnodev();
        if ( region->is_subroutines() )                                                                                        // subrouties have to be calculated
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                // if the call item's callee is the selected region
                // then take all its subroutines
                if ( ( *citer )->get_callee() == region )
                {
                    for ( unsigned j = 0; j < ( *citer )->num_children(); j++ )
                    {
                        if (  ( ( *citer )->get_child( j ) )->get_callee() != region )
                        {
                            v.push_back( ( *citer )->get_child( j ) );
                        }
                    }
                }
            }
            for ( unsigned i = 0; i < v.size(); i++ )
            {
                for ( unsigned j = 0; j < v[ i ]->num_children(); j++ )
                {
                    if ( ( v[ i ]->get_child( j ) )->get_callee() != region )
                    {
                        v.push_back( v[ i ]->get_child( j ) );
                    }
                }
            }
            _rf = cube::CUBE_CALCULATE_EXCLUSIVE;                                                          // force the exclusve calculation it the case of subroutines calculation.
        }
        else                                                                                               // region
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                if ( ( *citer )->get_callee() == region )
                {
                    v.push_back( *citer );
                }
            }
        }
        unsigned flag = 0;
        for ( std::vector<Cnode*>::iterator viter = v.begin(); viter != v.end(); ++viter, flag++ )
        {
            Value** _v = get_sevs_adv( metric, cube::CUBE_CALCULATE_INCLUSIVE, ( *viter ), _rf );
            if ( _v == NULL )
            {
                continue;
            }
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


        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                Value** _v   = get_sevs_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, region, rf );
                for ( size_t i = 0; i < locationv.size(); ++i )
                {
                    Value* __v = _v[ i ];
                    if ( __v == NULL )
                    {
                        continue;
                    }
                    value[ i ]->
                    operator-=( __v );
                }
                services::delete_row_of_values( _v, locationv.size() );
            }
        }
        return value;
    }
}







double*
Cube::get_sevs( Metric* metric, CalculationFlavour mf, Region* region, CalculationFlavour rnf )
{
    if ( !metric->isBuildIn() )
    {
        Value** v = get_sevs_adv( metric, mf, region, rnf );

        double* v_ret = services::transform_values_to_doubles( v, locationv.size() );
        services::delete_row_of_values( v, locationv.size() );     // == delete[] _v;
        return v_ret;
    }
    else
    {
        CalculationFlavour _rf = rnf;

        double* value =  services::create_row_of_doubles( locationv.size() );  // === new double[locationv.size()](0.); ;
        // collectiong cnodes, which have to be calculated for region
        std::vector<Cnode*> v;
        std::vector<Cnode*> cnodes = get_cnodev();
        if ( region->is_subroutines() )                                                                                        // subrouties have to be calculated
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                // if the call item's callee is the selected region
                // then take all its subroutines
                if ( ( *citer )->get_callee() == region )
                {
                    for ( unsigned j = 0; j < ( *citer )->num_children(); j++ )
                    {
                        if (  ( ( *citer )->get_child( j ) )->get_callee() != region )
                        {
                            v.push_back( ( *citer )->get_child( j ) );
                        }
                    }
                }
            }
            for ( unsigned i = 0; i < v.size(); i++ )
            {
                for ( unsigned j = 0; j < v[ i ]->num_children(); j++ )
                {
                    if ( ( v[ i ]->get_child( j ) )->get_callee() != region )
                    {
                        v.push_back( v[ i ]->get_child( j ) );
                    }
                }
            }
            _rf = cube::CUBE_CALCULATE_EXCLUSIVE;                                                          // force the exclusve calculation it the case of subroutines calculation.
        }
        else                                                                                               // region
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                if ( ( *citer )->get_callee() == region )
                {
                    v.push_back( *citer );
                }
            }
        }
        unsigned flag = 0;
        for ( std::vector<Cnode*>::iterator viter = v.begin(); viter != v.end(); ++viter, flag++ )
        {
            double* _v = get_sevs( metric, cube::CUBE_CALCULATE_INCLUSIVE, ( *viter ), _rf );
            if ( _v == NULL )
            {
                continue;
            }
            for ( size_t i = 0; i < locationv.size(); ++i )
            {
                value[ i ] += _v[ i ];
            }
            services::delete_raw_row( _v );
        }


        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                double* _v   = get_sevs( _met, cube::CUBE_CALCULATE_INCLUSIVE, region, _rf );
                if ( _v == NULL )
                {
                    continue;
                }
                for ( size_t i = 0; i < locationv.size(); ++i )
                {
                    value[ i ] -= _v[ i ];
                }
                services::delete_raw_row( _v );
            }
        }
        return value;
    }
}




// ------------------- aggregation over root cnodes -----------------------------


Value**
Cube::get_sevs_adv( Metric*            metric,
                    CalculationFlavour mf )
{
    if ( metric->isBuildIn() )
    {
        char*   _ret =  get_sevs_raw( metric, mf );
        Value** v    = services::transform_raw_to_values( _ret, metric->its_value(), locationv.size() );
        services::delete_raw_row( _ret );  // == delete[] _v;
        return v;
    }
    else
    {
        Value**  value =  services::create_row_of_values( locationv.size() ); // === new Value*[locationv.size()];
        unsigned flag  = 0;
        for ( std::vector<Cnode*>::iterator viter = root_cnodev.begin(); viter != root_cnodev.end(); ++viter, flag++ )
        {
            Value** _v = get_sevs_adv( metric, mf, ( *viter ), cube::CUBE_CALCULATE_INCLUSIVE );
            if ( _v == NULL )
            {
                continue;
            }
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
        return value;
    }
}

double*
Cube::get_sevs( Metric*            metric,
                CalculationFlavour mf )
{
    if ( !metric->isBuildIn() )
    {
        Value** _ret  =  get_sevs_adv( metric, mf );
        double* v_ret = services::transform_values_to_doubles( _ret, locationv.size() );
        services::delete_row_of_values( _ret, locationv.size() );         // == delete[] _v;
        return v_ret;
    }
    else
    {
        double* value =  services::create_row_of_doubles( locationv.size() );  // === new double[locationv.size()](0);
        for ( std::vector<Cnode*>::iterator viter = root_cnodev.begin(); viter != root_cnodev.end(); ++viter )
        {
            double* _v = get_sevs( metric, mf, ( *viter ), cube::CUBE_CALCULATE_INCLUSIVE );
            if ( _v == NULL )
            {
                continue;
            }

            for ( size_t i = 0; i < locationv.size(); ++i )
            {
                value[ i ] += _v[ i ];
            }
            services::delete_raw_row( _v );
        }
        return value;
    }
}

char*
Cube::get_sevs_raw( Metric*            metric,
                    CalculationFlavour mf )
{
    double* _v    = get_sevs( metric, mf );
    char*   v_ret = services::transform_doubles_to_raw( _v );
    return v_ret;
}
