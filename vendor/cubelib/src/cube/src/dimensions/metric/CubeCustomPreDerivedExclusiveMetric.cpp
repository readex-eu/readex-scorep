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
 * \file CubeCustomPreDerivedExclusiveMetric.cpp
 * \brief Defines methods to calculate incl/exclusve values if the metric is defined as an expression. Expression gets calculated first, then the value gets aggregated.

 ********************************************/


#include "config.h"
#include <iostream>
#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeSystemTreeNode.h"
#include "CubeCustomPreDerivedExclusiveMetric.h"
#include "CubeServices.h"

using namespace std;
using namespace cube;


CustomPreDerivedExclusiveMetric::~CustomPreDerivedExclusiveMetric()
{
}

double
CustomPreDerivedExclusiveMetric::get_sev( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    if ( !active )   //  if value == VOID
    {
        return 0.;
    }

    pre_calculation_preparation( cnode, sys );

    double v =  0.;
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
            Location* _loc   = ( Location* )sys;
            double    _value = evaluation->eval( cnode, CUBE_CALCULATE_EXCLUSIVE, _loc, sf );
            v = _value;
            for ( cnode_id_t cid = 0; cid < cnode->num_children(); ++cid )
            {
                if ( cnode->get_child( cid )->isHidden() ) // ad as well inclusive value of hidden cnodes
                {
                    double _v = get_sev(  cnode->get_child( cid ), CUBE_CALCULATE_INCLUSIVE, _loc, cube::CUBE_CALCULATE_INCLUSIVE   );
                    if ( aggr_plus_evaluation != NULL )
                    {
                        v = aggr_plus_evaluation->eval( v, _v );
                    }
                    else
                    {
                        ( v ) += _v;
                    }
                }
            }
            if ( cnf == CUBE_CALCULATE_INCLUSIVE )
            {
                for ( cnode_id_t cid = 0; cid < cnode->num_children(); ++cid )
                {
                    if ( cnode->get_child( cid )->isVisible() ) // add only visible children, coz the hidden have been added in excl value
                    {
                        double _v = get_sev( cnode->get_child( cid ), CUBE_CALCULATE_INCLUSIVE, _loc, cube::CUBE_CALCULATE_INCLUSIVE   );
                        if ( aggr_plus_evaluation != NULL )
                        {
                            v = aggr_plus_evaluation->eval( v, _v );
                        }
                        else
                        {
                            ( v ) += _v;
                        }
                    }
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
CustomPreDerivedExclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    Value* _v =  metric_value->clone();
    _v->
    operator=( get_sev( cnode, cnf, sys, sf ) );

    return _v;
}



double
CustomPreDerivedExclusiveMetric::get_sev( Cnode* cnode, CalculationFlavour cnf )
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
    double v = 0.;
    if ( evaluation != NULL )
    {
        bool first_call = true;
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
CustomPreDerivedExclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    Value* _v =  metric_value->clone();
    _v->
    operator=( get_sev( cnode, cnf ) );

    return _v;
}





double*
CustomPreDerivedExclusiveMetric::get_sevs( Cnode*             cnode,
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
            to_return = evaluation->eval_row( cnode, cube::CUBE_CALCULATE_EXCLUSIVE );
        }
        else
        {
            to_return = services::create_row_of_doubles( ntid );
            for ( size_t i = 0; i < ntid; ++i )
            {
                Location* _loc = sysv[ i ];
                to_return[ i ] = get_sev( cnode, cube::CUBE_CALCULATE_EXCLUSIVE, _loc,  cube::CUBE_CALCULATE_INCLUSIVE );
            }
        }

        bool* _first_calls = new bool[ ntid ];
        for ( uint64_t i = 0; i < ntid; i++ )
        {
            _first_calls[ i ] = true;
        }

        for ( cnode_id_t cid = 0; cid < cnode->num_children(); ++cid )
        {
            if ( cnode->get_child( cid )->isHidden() ) // ad as well inclusive value of hidden cnodes
            {
                double* _v = get_sevs(  cnode->get_child( cid ), CUBE_CALCULATE_INCLUSIVE );
                if ( _v == NULL )
                {
                    continue;
                }
                if ( to_return == NULL )
                {
                    to_return = services::create_row_of_doubles( ntid );
                }

                for ( unsigned i = 0; i < ntid; ++i )
                {
                    if ( aggr_plus_evaluation != NULL )
                    {
                        if ( _first_calls[ i ] )
                        {
                            to_return[ i ]    =  _v[ i ];
                            _first_calls[ i ] = false;
                        }
                        else
                        {
                            to_return[ i ] = aggr_plus_evaluation->eval( to_return[ i ], _v[ i ] );
                        }
                    }
                    else
                    {
                        ( to_return[ i ] ) += _v[ i ];
                    }
                }
                services::delete_raw_row( _v );
            }
        }
        if ( cfv == CUBE_CALCULATE_INCLUSIVE )
        {
            for ( cnode_id_t cid = 0; cid < cnode->num_children(); ++cid )
            {
                if ( cnode->get_child( cid )->isVisible() ) // add only visible children, coz the hidden have been added in excl value
                {
                    double* _v = get_sevs(  cnode->get_child( cid ), CUBE_CALCULATE_INCLUSIVE );
                    if ( _v == NULL )
                    {
                        continue;
                    }
                    if ( to_return == NULL )
                    {
                        to_return = services::create_row_of_doubles( ntid );
                    }
                    for ( unsigned i = 0; i < ntid; ++i )
                    {
                        if ( aggr_plus_evaluation != NULL )
                        {
                            if ( _first_calls[ i ] )
                            {
                                to_return[ i ]    =  _v[ i ];
                                _first_calls[ i ] = false;
                            }
                            else
                            {
                                to_return[ i ] = aggr_plus_evaluation->eval( to_return[ i ], _v[ i ] );
                            }
                        }
                        else
                        {
                            ( to_return[ i ] ) += _v[ i ];
                        }
                    }
                    services::delete_raw_row( _v );
                }
            }
        }
        delete[] _first_calls;
    }
    post_calculation_cleanup();
    return to_return;
}



Value**
CustomPreDerivedExclusiveMetric::get_sevs_adv( Cnode*             cnode,
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
CustomPreDerivedExclusiveMetric::get_sevs_raw( Cnode*             cnode,
                                               CalculationFlavour cfv
                                               )
{
    return ( char* )get_sevs( cnode, cfv );
}

Serializable*
cube::CustomPreDerivedExclusiveMetric::create( Connection&      connection,
                                               const CubeProxy& cubeProxy )
{
    return new CustomPreDerivedExclusiveMetric( connection, cubeProxy );
}

std::string
cube::CustomPreDerivedExclusiveMetric::get_serialization_key() const
{
    return get_static_serialization_key();
}

std::string
cube::CustomPreDerivedExclusiveMetric::get_static_serialization_key()
{
    return "CustomPreDerivedMetric|Exclusive";
}

cube::CustomPreDerivedExclusiveMetric::CustomPreDerivedExclusiveMetric( Connection&      connection,
                                                                        const CubeProxy& cubeProxy )
    : PreDerivedExclusiveMetric( connection, cubeProxy )
{
    /// @note Full packing and unpacking is done by Metric class.
}
