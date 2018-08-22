/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2018                                                **
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
 * \file CubePostDerivedMetric.cpp
 * \brief Defines methods to calculate incl/exclusve values if the metric is defined as an expression

 ********************************************/


#include "config.h"
#include <iostream>

#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeSystemTreeNode.h"
#include "CubePostDerivedMetric.h"
#include "CubeServices.h"

using namespace std;
using namespace cube;


PostDerivedMetric::~PostDerivedMetric()
{
}


void
PostDerivedMetric::initialize( void )
{
    if ( evaluation != NULL )
    {
        evaluation->setRowSize( ntid );
    }
    if ( init_evaluation != NULL )
    {
        init_evaluation->setRowSize( ntid );
        init_evaluation->eval();
        delete init_evaluation;
        init_evaluation = NULL;
    }
}




double
PostDerivedMetric::get_sev( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    if ( !active )   //  if value == VOID
    {
        return 0.;
    }
    Value* _value = get_sev_adv( cnode, cnf, sys, sf );
    double _d     = _value->getDouble();
    _value->Free();
    return _d;
}



// inclusive value and exclusive value in this metric are same
Value*
PostDerivedMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    if ( !active )   //  if value == VOID
    {
        return metric_value->clone();
    }
    if ( isCacheable() )
    {
        Value* cached_value = cache->getCachedValue( cnode, cnf, sys, sf );
        if ( cached_value != NULL )
        {
            return cached_value;
        }
    }
    pre_calculation_preparation( cnode, sys );
    Value* _v =  metric_value->clone();
    if ( evaluation != NULL )
    {
        double _value = evaluation->eval( cnode, cnf, sys, sf );
        _v->
        operator=( _value );
    }
    if ( isCacheable() )
    {
        cache->setCachedValue( _v, cnode, cnf, sys, sf );
    }
    post_calculation_cleanup();
    return _v;
}


double
PostDerivedMetric::get_sev( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )   //  if value == VOID
    {
        return 0.;
    }
    Value* _value = get_sev_adv( cnode, cnf );
    double _d     = _value->getDouble();
    _value->Free();
    return _d;
}




Value*
PostDerivedMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )   //  if value == VOID
    {
        return metric_value->clone();
    }
    if ( isCacheable() )
    {
        Value* cached_value = cache->getCachedValue( cnode, cnf );
        if ( cached_value != NULL )
        {
            return cached_value;
        }
    }
    pre_calculation_preparation( cnode );
    Value* _v     = metric_value->clone();
    double _value = 0.;
    if ( evaluation != NULL )
    {
        _value = evaluation->eval( cnode, cnf );
        _v->
        operator=( _value );
    }
    if ( isCacheable() )
    {
        cache->setCachedValue( _v, cnode, cnf );
    }
    post_calculation_cleanup();
    return _v;
}


double*
PostDerivedMetric::get_sevs( Cnode*             cnode,
                             CalculationFlavour cfv
                             )
{
    if ( !active )   //  if value == VOID
    {
        return NULL;
    }

    pre_calculation_preparation( cnode );

    double* _value = NULL;
    if ( evaluation != NULL )
    {
        if ( isRowWise() )
        {
            _value = evaluation->eval_row( cnode, cube::CUBE_CALCULATE_INCLUSIVE );
        }
        else
        {
            _value = services::create_row_of_doubles( ntid );
            for ( size_t i = 0; i < ntid; ++i )
            {
                Location* _loc = sysv[ i ];
                _value[ i ] = get_sev( cnode, cube::CUBE_CALCULATE_INCLUSIVE, _loc,  cube::CUBE_CALCULATE_INCLUSIVE );
            }
        }
    }
    post_calculation_cleanup();
    return _value;
}


Value**
PostDerivedMetric::get_sevs_adv( Cnode*             cnode,
                                 CalculationFlavour cfv
                                 )
{
    if ( !active )   //  if value == VOID
    {
        return NULL;
    }
    double* _v = ( double* )get_sevs( cnode, cfv );
    if ( _v == NULL )
    {
        _v = services::create_row_of_doubles( ntid );
    }
    Value** _to_return = NULL;
    _to_return = services::transform_doubles_to_values( _v, metric_value, ntid );
    services::delete_raw_row( _v );
    return _to_return;
}

char*
PostDerivedMetric::get_sevs_raw( Cnode*             cnode,
                                 CalculationFlavour cfv
                                 )
{
    double* _d_row = get_sevs( cnode, cfv );
    char*   _row   = NULL;
    if ( _d_row != NULL )
    {
        switch ( get_data_type() )
        {
            case CUBE_DATA_TYPE_UINT8:
                _row = ( char* )( services::transform_doubles_to_t<uint8_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_INT8:
                _row = ( char* )( services::transform_doubles_to_t<int8_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_UINT16:
                _row = ( char* )( services::transform_doubles_to_t<uint16_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_INT16:
                _row = ( char* )( services::transform_doubles_to_t<int16_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_UINT32:
                _row = ( char* )( services::transform_doubles_to_t<uint32_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_INT32:
                _row = ( char* )( services::transform_doubles_to_t<int32_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_UINT64:
                _row = ( char* )( services::transform_doubles_to_t<uint64_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_INT64:
                _row = ( char* )( services::transform_doubles_to_t<int64_t>( _d_row, ntid ) );
                break;
            case CUBE_DATA_TYPE_DOUBLE:
            default:
                _row = ( char* )( services::transform_doubles_to_t<double>( _d_row, ntid ) );
                break;
        }
        services::delete_raw_row( _d_row );
    }
    return _row;
}

Serializable*
cube::PostDerivedMetric::create( Connection&      connection,
                                 const CubeProxy& cubeProxy )
{
    return new PostDerivedMetric( connection, cubeProxy );
}

std::string
cube::PostDerivedMetric::get_serialization_key() const
{
    return get_static_serialization_key();
}

std::string
cube::PostDerivedMetric::get_static_serialization_key()
{
    return "PostDerivedMetric";
}

cube::PostDerivedMetric::PostDerivedMetric( Connection&      connection,
                                            const CubeProxy& cubeProxy )
    : Metric( connection, cubeProxy )
{
}
