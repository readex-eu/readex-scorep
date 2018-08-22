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
 * \file CubeInclusiveMetric.cpp
 * \brief Defines methods to calculate incl/exclusve values if the metric contains only inclusive values

 ********************************************/


#include "config.h"
#include <iostream>
#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeSystemTreeNode.h"

#include "CubeInclusiveMetric.h"
#include "CubeServices.h"


using namespace std;
using namespace cube;


InclusiveMetric::~InclusiveMetric()
{
}


row_of_objects_t*
InclusiveMetric::create_calltree_id_maps( IDdeliverer* ids, Cnode* root, row_of_objects_t*   _row )
{
    WideSearchEnumerator enumerator;
    ids->reset();
    _row = enumerator.get_objects_to_enumerate( root, _row );

    for ( row_of_objects_t::iterator iter = _row->begin(); iter < _row->end(); ++iter )
    {
        if ( calltree_local_ids.size() <= ( *iter )->get_id() )
        {
            calltree_local_ids.resize( ( *iter )->get_id() + 1 );
        }
        calltree_local_ids[ ( *iter )->get_id() ] = ids->get_next_id();
    }

    return _row;
}




Value*
InclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )   //  if value == VOID
    {
        return adv_sev_mat->getValue();
    }
    Value* v = NULL;
    if ( isCacheable() )
    {
        v = cache->getCachedValue( cnode, cnf );
        if ( v != NULL )
        {
            return v;
        }
    }
    if ( !cnode->is_clustered() )
    {
        v = adv_sev_mat->sumRow( calltree_local_ids[ cnode->get_remapping_cnode()->get_id() ] );
    }
    else
    {
        size_t sysv_size = sysv.size();
        for ( size_t i = 0; i < sysv_size; ++i )
        {
            Thread*            _thrd        = sysv[ i ];
            Process*           _proc        = _thrd->get_parent();
            int64_t            process_rank = _proc->get_rank();
            const cube::Cnode* _mapped      =  cnode->get_remapping_cnode( process_rank  );
            Value*             tmp          = ( _mapped == NULL ) ? metric_value->clone() : ( adv_sev_mat->getValue( calltree_local_ids[ _mapped->get_id() ], _thrd->get_id() ) );
            int64_t            _norm        = cnode->get_cluster_normalization( process_rank );
            if ( _norm > 0 )
            {
                tmp->normalizeWithClusterCount( ( uint64_t )_norm );
            }
            if ( v == NULL )
            {
                v = tmp;
            }
            else
            {
                v->
                operator+=( tmp );

                tmp->Free();
            }
        }
    }


    if ( cnf == CUBE_CALCULATE_EXCLUSIVE &&  ( cnode->num_children() > 0 ) )
    {
        std::vector<Cnode*>_cnodes;   // collection of cnodes for the calculation
        for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
        {
            Cnode* __c = cnode->get_child( cid );
            if ( !__c->isHidden() )
            {
                _cnodes.push_back( __c );
            }
        }
        for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
        {
            Cnode* __c = *_iter;
            if ( !__c->is_clustered() )
            {
                Value* tmp = adv_sev_mat->sumRow( calltree_local_ids[ __c->get_remapping_cnode()->get_id() ] );
                v->
                operator-=( tmp );

                tmp->Free();
            }
            else
            {
                size_t sysv_size = sysv.size();
                for ( size_t i = 0; i < sysv_size; ++i )
                {
                    Thread*            _thrd        = sysv[ i ];
                    Process*           _proc        = _thrd->get_parent();
                    int64_t            process_rank = _proc->get_rank();
                    const cube::Cnode* _mapped      =  __c->get_remapping_cnode( process_rank  );
                    Value*             tmp          = ( _mapped == NULL ) ? metric_value->clone() : ( adv_sev_mat->getValue( calltree_local_ids[ _mapped->get_id() ], _thrd->get_id() ) );
                    int64_t            _norm        = __c->get_cluster_normalization( process_rank );
                    if ( _norm > 0 )
                    {
                        tmp->normalizeWithClusterCount( ( uint64_t )_norm );
                    }
                    v->
                    operator-=( tmp );

                    tmp->Free();
                }
            }
        }
    }
    if ( isCacheable() )
    {
        cache->setCachedValue( v,  cnode, cnf );
    }
    return v;
}






// inclusive value and exclusive value in this metric are same
Value*
InclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    Value* v = NULL;

    if ( !active )   //  if value == VOID
    {
        return v;
    }

    if ( ( sys->isSystemTreeNode() || sys->isLocationGroup() ) && sf == cube::CUBE_CALCULATE_EXCLUSIVE )
    {
        return v;
    }
    if ( isCacheable() )
    {
        Value* cached_value = cache->getCachedValue( cnode, cnf, sys, sf );
        if ( cached_value != NULL )
        {
            if ( v != NULL )
            {
                v->Free();
            }
            return cached_value;
        }
    }
    if ( sys->isSystemTreeNode() )
    {
        SystemTreeNode* _sys = ( SystemTreeNode* )sys;
        // first add values of all sub system nodes
        for ( unsigned i = 0; i < _sys->num_children(); ++i )
        {
            Value* _v = get_sev_adv( cnode, cnf, _sys->get_child( i ), cube::CUBE_CALCULATE_INCLUSIVE );
            if ( v == NULL )
            {
                v = _v;
            }
            else
            {
                v->
                operator+=( _v );

                _v->Free();
            }
        }
        // then add all values of all sub local groups
        for ( unsigned i = 0; i < _sys->num_groups(); ++i )
        {
            Value* _v = get_sev_adv( cnode, cnf, _sys->get_location_group( i ), cube::CUBE_CALCULATE_INCLUSIVE );
            if ( v == NULL )
            {
                v = _v;
            }
            else
            {
                v->
                operator+=( _v );

                _v->Free();
            }
        }
    }
    if ( sys->isLocationGroup() )
    {
        LocationGroup* _lg = ( LocationGroup* )sys;
        for ( unsigned i = 0; i < _lg->num_children(); ++i )
        {
            Value* _v = get_sev_adv( cnode, cnf, _lg->get_child( i ), cube::CUBE_CALCULATE_INCLUSIVE );
            if ( v == NULL )
            {
                v = _v;
            }
            else
            {
                v->
                operator+=( _v );

                _v->Free();
            }
        }
    }
    if ( sys->isLocation() )
    {
        Location*          _loc    = ( Location* )sys;
        LocationGroup*     _lg     = _loc->get_parent();
        int64_t            lg_rank = _lg->get_rank();
        const cube::Cnode* _mapped =  cnode->get_remapping_cnode( lg_rank  );
        Value*             _v      = ( _mapped == NULL ) ? metric_value->clone() : ( adv_sev_mat->getValue( calltree_local_ids[  _mapped->get_id() ], _loc->get_id() ) );
        int64_t            _norm   = cnode->get_cluster_normalization( lg_rank );
        if ( _norm > 0 )
        {
            _v->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        v = _v;

        if ( cnf == CUBE_CALCULATE_EXCLUSIVE &&  ( cnode->num_children() > 0 ) )
        {
            std::vector<Cnode*>_cnodes;   // collection of cnodes for the calculation
            for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
            {
                Cnode* __c  = cnode->get_child( cid );
                bool   _add = ( !__c->isHidden() );
                if ( _add )
                {
                    _cnodes.push_back( __c );
                }
            }
            for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
            {
                Cnode*             __c     = *_iter;
                const cube::Cnode* _mapped =  __c->get_remapping_cnode( lg_rank  );
                _v = ( _mapped == NULL ) ? metric_value->clone() : ( adv_sev_mat->getValue( calltree_local_ids[  _mapped->get_id() ], _loc->get_id() ) );
                int64_t _norm = __c->get_cluster_normalization( lg_rank );
                if ( _norm > 0 )
                {
                    _v->normalizeWithClusterCount( ( uint64_t )_norm );
                }
                v->
                operator+=( _v );
            }
        }
    }
    if ( isCacheable() )
    {
        cache->setCachedValue( v, cnode, cnf, sys, sf );
    }
    return v;
}



Value**
InclusiveMetric::get_sevs_adv( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )   //  if value == VOID
    {
        return NULL;
    }

    Value** to_return = services::create_row_of_values( ntid );

    if ( !cnode->is_clustered() )
    {
        char* _v    =   adv_sev_mat->getRow( calltree_local_ids[ cnode->get_remapping_cnode()->get_id() ] );
        char* start = _v;
        for ( unsigned i = 0; i < ntid; ++i )
        {
            Value* _val = metric_value->clone();
            if ( start != NULL )
            {
                start = _val->fromStream( start );
            }
            to_return[ i ] = _val;
        }
    }
    else
    {
        size_t sysv_size = sysv.size();
        for ( size_t i = 0; i < sysv_size; ++i )
        {
            Thread*            _thrd        = sysv[ i ];
            Process*           _proc        = _thrd->get_parent();
            int64_t            process_rank = _proc->get_rank();
            const cube::Cnode* _mapped      =  cnode->get_remapping_cnode( process_rank  );
            Value*             tmp          =  ( _mapped == NULL ) ? metric_value->clone() : ( adv_sev_mat->getValue( calltree_local_ids[ _mapped->get_id() ], _thrd->get_id() ) );
            int64_t            _norm        = cnode->get_cluster_normalization( process_rank );
            if ( _norm > 0 )
            {
                tmp->normalizeWithClusterCount( ( uint64_t )_norm );
            }
            to_return[ i ] = tmp;
        }
    }


    if ( cnf == CUBE_CALCULATE_EXCLUSIVE &&  ( cnode->num_children() > 0 ) )
    {
        std::vector<Cnode*>_cnodes;   // collection of cnodes for the calculation
        for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
        {
            Cnode* __c  = cnode->get_child( cid );
            bool   _add = ( !__c->isHidden() );
            if ( _add )
            {
                _cnodes.push_back( __c );
            }
        }

        for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
        {
            Cnode* __c = *_iter;
            if ( !__c->is_clustered() )
            {
                char* _v    =   adv_sev_mat->getRow( calltree_local_ids[ __c->get_remapping_cnode()->get_id() ] );
                char* start = _v;
                for ( unsigned i = 0; i < ntid; ++i )
                {
                    Value* _val = metric_value->clone();
                    if ( start != NULL )
                    {
                        start = _val->fromStream( start );
                    }
                    to_return[ i ]->
                    operator-=( _val );

                    _val->Free();
                }
            }
            else
            {
                size_t sysv_size = sysv.size();
                for ( size_t i = 0; i < sysv_size; ++i )
                {
                    Thread*            _thrd        = sysv[ i ];
                    Process*           _proc        = _thrd->get_parent();
                    int64_t            process_rank = _proc->get_rank();
                    const cube::Cnode* _mapped      =  __c->get_remapping_cnode( process_rank  );
                    Value*             tmp          = ( _mapped == NULL ) ? metric_value->clone() : ( adv_sev_mat->getValue( calltree_local_ids[ _mapped->get_id() ], _thrd->get_id() ) );
                    int64_t            _norm        = __c->get_cluster_normalization( process_rank );
                    if ( _norm > 0 )
                    {
                        tmp->normalizeWithClusterCount( ( uint64_t )_norm );
                    }
                    to_return[ i ]->
                    operator-=( tmp );

                    tmp->Free();
                }
            }
        }
    }



    return to_return;
}

Serializable*
cube::InclusiveMetric::create( Connection&      connection,
                               const CubeProxy& cubeProxy )
{
    return new InclusiveMetric( connection, cubeProxy );
}

std::string
cube::InclusiveMetric::get_serialization_key() const
{
    return get_static_serialization_key();
}

std::string
cube::InclusiveMetric::get_static_serialization_key()
{
    return "Metric|Inclusive|Generic";
}

cube::InclusiveMetric::InclusiveMetric( Connection&      connection,
                                        const CubeProxy& cubeProxy )
    : Metric( connection, cubeProxy )
{
}
