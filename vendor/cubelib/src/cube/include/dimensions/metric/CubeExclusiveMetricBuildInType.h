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
 * \file
 * \brief Defines a metric class, which uses build in type as a data type
 *
 */
#ifndef CUBE_EXCLUSIVE_BUILDIN_METRIC_H
#define CUBE_EXCLUSIVE_BUILDIN_METRIC_H


#include <iosfwd>
#include <map>
#include <cstring>
#include <limits>
#include <iostream>

#include "CubeIDdeliverer.h"
#include "CubeDeepSearchEnumerator.h"

#include "CubeMetricBuildInType.h"
#include "CubeServices.h"

/*
 *----------------------------------------------------------------------------
 *
 * class BuildInTypeMetric
 *
   =======
 */


namespace cube
{
class Connection;
class CubeProxy;

template <class T>
class ExclusiveBuildInTypeMetric : public BuildInTypeMetric<T>
{
public:

    ExclusiveBuildInTypeMetric( const std::string& disp_name,
                                const std::string& uniq_name,
                                const std::string& dtype,
                                const std::string& uom,
                                const std::string& val,
                                const std::string& url,
                                const std::string& descr,
                                FileFinder*        ffinder,
                                Metric*            parent,
                                uint32_t           id = 0,
                                VizTypeOfMetric    is_ghost = CUBE_METRIC_NORMAL
                                )
        :
        BuildInTypeMetric< T >( disp_name,
                                uniq_name,
                                dtype,
                                uom,
                                val,
                                url,
                                descr,
                                ffinder,
                                parent,
                                id,
                                is_ghost )
    {
    }

    /// @brief
    ///     Factory method to create an intrinsic-type inclusive metric via a Cube connection.
    /// @param connection
    ///     Active Cube connection.
    /// @param cubeProxy
    ///     Cube proxy for internal cross-referencing.
    /// @return
    ///     New metric.
    ///
    static Serializable*
    create( Connection&      connection,
            const CubeProxy& cubeProxy )
    {
        return new ExclusiveBuildInTypeMetric<T>( connection, cubeProxy );
    }

    virtual
    ~ExclusiveBuildInTypeMetric()
    {
    }

    /// @copydoc Serializable::get_serialization_key
    ///
    virtual std::string
    get_serialization_key() const
    {
        return get_static_serialization_key();
    }

    /// @copydoc Serializable::get_serialization_key
    ///
    static std::string
    get_static_serialization_key()
    {
        return "Metric|Exclusive|" + BuildInTypeMetric<T>::get_type_string();
    }

    virtual TypeOfMetric
    get_type_of_metric() const
    {
        return CUBE_METRIC_EXCLUSIVE;
    }

    virtual IndexFormat
    get_index_format()
    {
        return CUBE_INDEX_FORMAT_SPARSE;
    }

    virtual std::string
    get_metric_kind() const
    {
        return "EXCLUSIVE";
    };


    virtual row_of_objects_t*
    create_calltree_id_maps( IDdeliverer*,
                             Cnode*,
                             row_of_objects_t*  );                               // /< Creates enumeration of Callpath-Tree according the "deep search" prescription. It is optimal for the calculation of inclusive values in exclusive metric.


    using cube::BuildInTypeMetric<T>::get_sev;

    // /  Calculates inclusive value for given thread. Expands only calltree. Returns double representation of Value if possible.
    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv,
             Sysres*            sys,
             CalculationFlavour sf );


    // / Calculates inclusive value for given cnode and over all threads. Sums up over all threads. Assumed that all threads have continueos ids. Expands only calltree. Returns double representation of Value if possible.
    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv );


    virtual char*
    get_sevs_raw( Cnode*             cnode,
                  CalculationFlavour cfv
                  );

    virtual double*
    get_sevs( Cnode*             cnode,
              CalculationFlavour cfv
              );

protected:

    /// @brief
    ///     Constructs a Metric object via a Cube connection.
    /// @param connection
    ///     Active Cube connection.
    /// @param cubeProxy
    ///     Cube proxy for internal cross-referencing.
    ///
    /// @note
    ///     This constructor is called by the factory method.
    ///
    ExclusiveBuildInTypeMetric( Connection&      connection,
                                const CubeProxy& cubeProxy )
        : BuildInTypeMetric<T>( connection, cubeProxy )
    {
        /// @note The full packing and unpacking is done by the Metric base class;
    }
};




template <class T>
row_of_objects_t*
ExclusiveBuildInTypeMetric<T>::create_calltree_id_maps( IDdeliverer* ids, Cnode* root, row_of_objects_t* _row  )
{
    ids->reset();
    DeepSearchEnumerator enumerator;
    _row = enumerator.get_objects_to_enumerate( root, _row );
    for ( row_of_objects_t::iterator iter = _row->begin(); iter < _row->end(); ++iter )
    {
        if ( this->calltree_local_ids.size() <=  ( *iter )->get_id() )
        {
            this->calltree_local_ids.resize( ( *iter )->get_id() + 1 );
        }
        this->calltree_local_ids[ ( *iter )->get_id() ] = ids->get_next_id();
    }

    return _row;
}




// / ------------------------ Sum over all threads for given cnode and all subcnodes



template <class T>
double
ExclusiveBuildInTypeMetric<T>::get_sev( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !( this->active ) )
    {
        return 0.;
    }
    T v = static_cast<T>( 0 );
    if ( this->isCacheable() && ( ( this->t_cache )->isTCachedValue( cnode, cnf ) ) )
    {
        return ( this->t_cache )->getTCachedValue( cnode, cnf );
    }

    if ( this->adv_sev_mat == NULL )
    {
        return v;
    }


    std::vector<Cnode*> _cnodes;  // collection of cnodes for the calculation
    _cnodes.push_back( cnode );
    for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
    {
        Cnode* __c  = cnode->get_child( cid );
        bool   _add = ( __c->isHidden() ) || ( cnf == CUBE_CALCULATE_INCLUSIVE ); // either it is hidden, or we calculate inclusive value
        if ( _add )
        {
            _cnodes.push_back( __c );
            std::vector<Cnode*>& _vc = __c->get_whole_subtree();
            for ( std::vector<Cnode*>::const_iterator _iter = _vc.begin(); _iter != _vc.end(); ++_iter )
            {
                _cnodes.push_back( *_iter );
            }
        }
    }

// sum up the values
    for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
    {
        Cnode* __c = *_iter;
        if ( !__c->is_clustered() )
        {
            v += this->adv_sev_mat->template sumBuildInTypeRow<T>( this->calltree_local_ids[ __c->get_remapping_cnode()->get_id() ] );
        }
        else
        {
            size_t sysv_size = this->sysv.size();
            for ( size_t i = 0; i < sysv_size; i++ )
            {
                Thread*            _thrd        = this->sysv[ i ];
                Process*           _proc        = _thrd->get_parent();
                int64_t            process_rank = _proc->get_rank();
                const cube::Cnode* _mapped      =  __c->get_remapping_cnode( process_rank  );
                T                  tmp          = ( _mapped == NULL ) ? static_cast<T>( 0 ) : ( ( this->adv_sev_mat )->template getBuildInValue<T>( this->calltree_local_ids[ _mapped->get_id() ], _thrd->get_id() ) );
                int64_t            _norm        = __c->get_cluster_normalization( process_rank );
                if ( _norm > 0 )
                {
                    tmp /= ( ( uint64_t )_norm );
                }
                ( v ) += tmp;
            }
        }
    }
    if ( this->isCacheable()  )
    {
        ( this->t_cache )->setTCachedValue( v,  cnode, cnf );
    }
    return static_cast<double>( v );
}



template <class T>
double
ExclusiveBuildInTypeMetric<T>::get_sev( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    if ( !( this->active ) )
    {
        return 0.;
    }


    T v = static_cast<T>( 0 );
    if ( ( this->adv_sev_mat == NULL ) || ( ( sys->isSystemTreeNode() || sys->isLocationGroup() ) && ( sf == cube::CUBE_CALCULATE_EXCLUSIVE ) ) )
    {
        return 0.;
    }

    if ( this->isCacheable() && ( ( this->t_cache )->isTCachedValue( cnode, cnf, sys, sf )  ) )
    {
        return ( this->t_cache )->getTCachedValue( cnode, cnf, sys, sf );
    }

    std::vector<Sysres*> _locations = sys->get_whole_subtree(); // collection of locations of this sysres
    std::vector<Cnode*>  _cnodes;                               // collection of cnodes for the calculation
    _cnodes.push_back( cnode );
    for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
    {
        Cnode* __c  = cnode->get_child( cid );
        bool   _add = ( __c->isHidden() ) || ( cnf == CUBE_CALCULATE_INCLUSIVE ); // either it is hidden, or we calculate inclusive value
        if ( _add )
        {
            _cnodes.push_back( __c );
            std::vector<Cnode*>& _vc = __c->get_whole_subtree();
            for ( std::vector<Cnode*>::const_iterator _iter = _vc.begin(); _iter != _vc.end(); ++_iter )
            {
                _cnodes.push_back( *_iter );
            }
        }
    }
    for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
    {
        Cnode* __c = *_iter;

        for ( std::vector<Sysres*>::const_iterator _siter = _locations.begin(); _siter != _locations.end(); ++_siter )
        {
            Location*          _loc    = ( Location* )( *_siter );
            LocationGroup*     _lg     = _loc->get_parent();
            int64_t            lg_rank = _lg->get_rank();
            const cube::Cnode* _mapped =  __c->get_remapping_cnode( lg_rank  );
            T                  _v      = ( _mapped == NULL ) ? static_cast<T>( 0 ) : ( ( this->adv_sev_mat )->template getBuildInValue<T>( this->calltree_local_ids[ _mapped->get_id() ], _loc->get_id() ) );
            int64_t            _norm   = __c->get_cluster_normalization( lg_rank );
            if ( _norm > 0 )
            {
                _v /= ( ( uint64_t )_norm );
            }
            ( v ) += _v;
        }
    }
    if ( this->isCacheable()  )
    {
        ( this->t_cache )->setTCachedValue( v, cnode, cnf, sys, sf );
    }
    return static_cast<double>( v );
}




template <class T>
char*
ExclusiveBuildInTypeMetric<T>::get_sevs_raw( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !( this->active ) )
    {
        return NULL;
    }

    if ( this->adv_sev_mat == NULL )
    {
        return NULL;
    }
    if ( this->isCacheable() )
    {
        char* v = ( this->t_cache )->getCachedRow( cnode, cnf );
        if ( v != NULL )
        {
            return v;
        }
    }


    std::vector<Cnode*> _cnodes;  // collection of cnodes for the calculation
    _cnodes.push_back( cnode );
    for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
    {
        Cnode* __c  = cnode->get_child( cid );
        bool   _add = ( __c->isHidden() ) || ( cnf == CUBE_CALCULATE_INCLUSIVE ); // either it is hidden, or we calculate inclusive value
        if ( _add )
        {
            _cnodes.push_back( __c );
            std::vector<Cnode*>& _vc = __c->get_whole_subtree();
            for ( std::vector<Cnode*>::const_iterator _iter = _vc.begin(); _iter != _vc.end(); ++_iter )
            {
                _cnodes.push_back( *_iter );
            }
        }
    }

    size_t sysv_size  = this->sysv.size();
    char*  to_return  = services::create_raw_row( sysv_size * this->metric_value->getSize() );
    T*     _to_return = ( T* )to_return;

    for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
    {
        Cnode* __c = *_iter;

        if ( !__c->is_clustered() )
        {
            char* _tmp =    this->adv_sev_mat->getRow( this->calltree_local_ids[ __c->get_remapping_cnode()->get_id() ] );


            T* tmp = ( T* )_tmp;
            if ( tmp != NULL )
            {
                for ( size_t i = 0; i < sysv_size; i++ )
                {
                    _to_return[ i ] += tmp[ i ];
                }
            }
        }
        else
        {
            T* _to_return = ( T* )to_return;
            for ( size_t i = 0; i < sysv_size; i++ )
            {
                Thread*            _thrd        = this->sysv[ i ];
                Process*           _proc        = _thrd->get_parent();
                int64_t            process_rank = _proc->get_rank();
                const cube::Cnode* _mapped      =  __c->get_remapping_cnode( process_rank  );
                T                  tmp          = ( _mapped == NULL ) ? static_cast<T>( 0 ) : ( ( this->adv_sev_mat )->template getBuildInValue<T>( this->calltree_local_ids[ _mapped->get_id() ], _thrd->get_id() ) );
                int64_t            _norm        = __c->get_cluster_normalization( process_rank );
                if ( _norm > 0 )
                {
                    tmp /= ( ( uint64_t )_norm );
                }
                _to_return[ i ] += tmp;
            }
        }
    }
    if ( this->isCacheable() )
    {
        ( this->t_cache )->setCachedRow( to_return,  cnode, cnf );
    }
    return to_return;
}

template <class T>
double*
ExclusiveBuildInTypeMetric<T>::get_sevs( Cnode*             cnode,
                                         CalculationFlavour cfv
                                         )
{
    char*   _v        =   this->get_sevs_raw( cnode, cfv );
    T*      __v       = ( T* )_v;
    size_t  sysv_size = this->sysv.size();
    double* to_return = services::create_row_of_doubles( sysv_size );
    if ( _v == NULL )
    {
        return to_return;
    }
    for ( unsigned i = 0; i < sysv_size; ++i )
    {
        to_return[ i ] = ( double )__v[ i ];
    }
    services::delete_raw_row( _v );
    return to_return;
}
}


#endif
