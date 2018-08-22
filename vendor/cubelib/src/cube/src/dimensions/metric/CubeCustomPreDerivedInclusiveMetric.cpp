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
 * \file CubeCustomPreDerivedInclusiveMetric.cpp
 * \brief Defines methods to calculate incl/exclusve values if the metric is defined as an expression. Expression gets calculated first, then the value gets aggregated.

 ********************************************/


#include "config.h"
#include <iostream>
#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeSystemTreeNode.h"
#include "CubeCustomPreDerivedInclusiveMetric.h"
#include "CubeServices.h"


using namespace std;
using namespace cube;


CustomPreDerivedInclusiveMetric::~CustomPreDerivedInclusiveMetric()
{
}

// inclusive value and exclusive value in this metric are same
double
CustomPreDerivedInclusiveMetric::get_sev( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    if ( !active )   //  if value == VOID
    {
        return 0.;
    }

    pre_calculation_preparation( cnode, sys );

    if ( isCacheable() )
    {
        Value* cached_value = cache->getCachedValue( cnode, cnf, sys, sf );
        if ( cached_value != NULL )
        {
            double _d = cached_value->getDouble();
            delete cached_value;
            return _d;
        }
    }
    double v =  0.;
    if ( evaluation != NULL )
    {
        if ( ( sys->isSystemTreeNode() || sys->isLocationGroup() ) && sf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            post_calculation_cleanup();
            return v;
        }
        bool first_call = true;
        if ( sys->isSystemTreeNode() )
        {
            SystemTreeNode* _sys = ( SystemTreeNode* )sys;
            // first add values of all sub system nodes
            for ( unsigned i = 0; i < _sys->num_children(); ++i )
            {
                double _v = get_sev( cnode, cnf, _sys->get_child( i ), cube::CUBE_CALCULATE_INCLUSIVE );
                if ( aggr_aggr_evaluation != NULL )
                {
                    if ( first_call )
                    {
                        v          = _v;
                        first_call = false;
                    }
                    else
                    {
                        v = aggr_aggr_evaluation->eval( v, _v );
                    }
                }
                else
                {
                    ( v ) += _v;
                }
            }
            // then add all values of all sub local groups
            for ( unsigned i = 0; i < _sys->num_groups(); ++i )
            {
                double _v = get_sev( cnode, cnf, _sys->get_location_group( i ), cube::CUBE_CALCULATE_INCLUSIVE );
                if ( aggr_aggr_evaluation != NULL )
                {
                    if ( first_call )
                    {
                        v          = _v;
                        first_call = false;
                    }
                    else
                    {
                        v = aggr_aggr_evaluation->eval( v, _v );
                    }
                }
                else
                {
                    ( v ) += _v;
                }
            }
        }
        if ( sys->isLocationGroup() )
        {
            LocationGroup* _lg = ( LocationGroup* )sys;
            // first add values of all locations
            for ( unsigned i = 0; i < _lg->num_children(); ++i )
            {
                double _v = get_sev( cnode, cnf, _lg->get_child( i ), cube::CUBE_CALCULATE_INCLUSIVE );
                if ( aggr_aggr_evaluation != NULL )
                {
                    if ( first_call )
                    {
                        v          = _v;
                        first_call = false;
                    }
                    else
                    {
                        v = aggr_aggr_evaluation->eval( v, _v );
                    }
                }
                else
                {
                    ( v ) += _v;
                }
            }
        }
        if ( sys->isLocation() )
        {
            Location* _loc      = ( Location* )sys;
            double    own_value = evaluation->eval( cnode, CUBE_CALCULATE_INCLUSIVE, _loc, sf );
            ( v ) = own_value;
            if ( cnf == CUBE_CALCULATE_EXCLUSIVE )
            {
                double sum_value = 0;
                for ( cnode_id_t cid = 0; cid < cnode->num_children(); ++cid )
                {
                    if ( cnode->get_child( cid )->isVisible() ) // add only visible children, coz the hidden have been added in excl value
                    {
                        double _v = get_sev(  cnode->get_child( cid ), CUBE_CALCULATE_INCLUSIVE, _loc, cube::CUBE_CALCULATE_INCLUSIVE   );
                        if ( aggr_plus_evaluation != NULL )
                        {
                            sum_value = ( cid == 0 ) ? sum_value :  aggr_plus_evaluation->eval( sum_value, _v );
                        }
                        else
                        {
                            ( sum_value ) += _v;
                        }
                    }
                }
                if ( aggr_minus_evaluation != NULL )
                {
                    v =   aggr_minus_evaluation->eval( own_value, sum_value );
                }
                else
                {
                    v = own_value -  sum_value;
                }
            }
        }
    }
    if ( isCacheable() )
    {
        Value* _v =  metric_value->clone();
        _v->
        operator=( v );

        cache->setCachedValue( _v, cnode, cnf, sys, sf );
        _v->Free();
    }
    post_calculation_cleanup();
    return v;
}




// inclusive value and exclusive value in this metric are same
Value*
CustomPreDerivedInclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    Value* _v =  metric_value->clone();
    _v->
    operator=( get_sev( cnode, cnf, sys, sf ) );

    return _v;
}



double
CustomPreDerivedInclusiveMetric::get_sev( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )   //  if value == VOID
    {
        return 0.;
    }

    pre_calculation_preparation( cnode );
    if ( isCacheable() )
    {
        Value* cached_value = cache->getCachedValue( cnode, cnf );
        if ( cached_value != NULL )
        {
            double _d = cached_value->getDouble();
            delete cached_value;
            return _d;
        }
    }
    double v          = 0.;
    bool   first_call = true;
    if ( evaluation != NULL )
    {
        for ( std::vector<Location*>::iterator iter = sysv.begin(); iter != sysv.end(); ++iter )
        {
            Location* _loc = *iter;
            pre_calculation_preparation( cnode, _loc );
            double _value = evaluation->eval( cnode, cnf, _loc, CUBE_CALCULATE_EXCLUSIVE );
            if ( aggr_aggr_evaluation != NULL )
            {
                if ( first_call )
                {
                    v          = _value;
                    first_call = false;
                }
                else
                {
                    v = aggr_aggr_evaluation->eval( v, _value );
                }
            }
            else
            {
                ( v ) += _value;
            }
            post_calculation_cleanup();
        }
    }
    if ( isCacheable() )
    {
        Value* _v =  metric_value->clone();
        _v->
        operator=( v );

        cache->setCachedValue( _v, cnode, cnf );
        _v->Free();
    }
    post_calculation_cleanup();
    return v;
}



Value*
CustomPreDerivedInclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    Value* _v =  metric_value->clone();
    _v->
    operator=( get_sev( cnode, cnf ) );

    return _v;
}



double*
CustomPreDerivedInclusiveMetric::get_sevs( Cnode*             cnode,
                                           CalculationFlavour cfv
                                           )
{
    if ( !active )   //  if value == VOID
    {
        return NULL;
    }

    pre_calculation_preparation( cnode );


    double* to_return = NULL;
    if ( evaluation != NULL )
    {
        if ( isRowWise() )
        {
            to_return = evaluation->eval_row( cnode, cube::CUBE_CALCULATE_INCLUSIVE );
        }
        else
        {
            to_return = services::create_row_of_doubles( ntid );
            for ( size_t i = 0; i < ntid; ++i )
            {
                Location* _loc = sysv[ i ];
                to_return[ i ] = get_sev( cnode, cube::CUBE_CALCULATE_INCLUSIVE, _loc,  cube::CUBE_CALCULATE_INCLUSIVE );
            }
        }

        if ( to_return == NULL )
        {
            to_return = services::create_row_of_doubles( ntid );
        }
        bool* _first_calls = new bool[ ntid ];
        for ( uint64_t i = 0; i < ntid; i++ )
        {
            _first_calls[ i ] = true;
        }

        if ( cfv == CUBE_CALCULATE_EXCLUSIVE )
        {
            double* _sums =  services::create_row_of_doubles( ntid );
            for ( cnode_id_t cid = 0; cid < cnode->num_children(); ++cid )
            {
                if ( cnode->get_child( cid )->isVisible() ) // add only visible children, coz the hidden have been added in excl value
                {
                    double* _v = get_sevs(  cnode->get_child( cid ), CUBE_CALCULATE_INCLUSIVE );
                    if ( _v == NULL )
                    {
                        continue;
                    }
                    for ( unsigned i = 0; i < ntid; ++i )
                    {
                        if ( aggr_plus_evaluation != NULL )
                        {
                            if ( _first_calls[ i ] )
                            {
                                _sums[ i ]        =  _v[ i ];
                                _first_calls[ i ] = false;
                            }
                            else
                            {
                                _sums[ i ] = aggr_plus_evaluation->eval( _sums[ i ], _v[ i ] );
                            }
                        }
                        else
                        {
                            ( _sums[ i ] ) += _v[ i ];
                        }
                    }
                    services::delete_raw_row( _v );
                }
            }
            for ( unsigned i = 0; i < ntid; ++i )
            {
                if ( aggr_minus_evaluation != NULL )
                {
                    to_return[ i ] = aggr_minus_evaluation->eval( to_return[ i ], _sums[ i ] );
                }
                else
                {
                    ( to_return[ i ] ) -= _sums[ i ];
                }
            }
            services::delete_raw_row( _sums );
        }
        delete[] _first_calls;
    }
    post_calculation_cleanup();
    return to_return;
}





Value**
CustomPreDerivedInclusiveMetric::get_sevs_adv( Cnode*             cnode,
                                               CalculationFlavour cfv
                                               )
{
    char*   _v         = ( char* )get_sevs( cnode, cfv );
    Value** _to_return = NULL;
    if ( _v != NULL )
    {
        _to_return = services::transform_raw_to_values( _v, metric_value, ntid );
    }
    services::delete_raw_row( _v );
    return _to_return;
}

char*
CustomPreDerivedInclusiveMetric::get_sevs_raw( Cnode*             cnode,
                                               CalculationFlavour cfv
                                               )
{
    return ( char* )get_sevs( cnode, cfv );
}

cube::Serializable*
cube::CustomPreDerivedInclusiveMetric::create( Connection&      connection,
                                               const CubeProxy& cubeProxy )
{
    return new CustomPreDerivedInclusiveMetric( connection, cubeProxy );
}

std::string
cube::CustomPreDerivedInclusiveMetric::get_serialization_key() const
{
    return get_static_serialization_key();
}

std::string
cube::CustomPreDerivedInclusiveMetric::get_static_serialization_key()
{
    return "CustomPreDerivedMetric|Inclusive";
}

cube::CustomPreDerivedInclusiveMetric::CustomPreDerivedInclusiveMetric( Connection&      connection,
                                                                        const CubeProxy& cubeProxy )
    : PreDerivedInclusiveMetric( connection, cubeProxy )
{
    /// @note Full packing and unpacking is done by Metric class.
}
