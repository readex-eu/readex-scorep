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
**  Copyright (c) 2015                                                     **
**  RWTH Aachen University, JARA-HPC                                       **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/



/**
 * \file CubeMetric.cpp
 * \brief Defines a methods of a Metric

 ********************************************/

#include "config.h"

#include "CubeMetric.h"

#include <cassert>
#include <iostream>

#include "CubeCaches.h"
#include "CubeCnode.h"
#include "CubeConnection.h"
#include "CubeCustomPreDerivedExclusiveMetric.h"
#include "CubeCustomPreDerivedInclusiveMetric.h"
#include "CubeError.h"
#include "CubeExclusiveMetric.h"
#include "CubeExclusiveMetricBuildInType.h"
#include "CubeFileFinder.h"
#include "CubeGeneralEvaluation.h"
#include "CubeIDdeliverer.h"
#include "CubeInclusiveMetric.h"
#include "CubeInclusiveMetricBuildInType.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeObjectsEnumerator.h"
#include "CubePLMemoryManager.h"
#include "CubePostDerivedMetric.h"
#include "CubePreDerivedExclusiveMetric.h"
#include "CubePreDerivedInclusiveMetric.h"
#include "CubeProxy.h"
#include "CubeRegion.h"
#include "CubeRowWiseMatrix.h"
#include "CubeServices.h"
#include "CubeSerializablesFactory.h"
#include "CubeStrategies.h"
#include "CubeSysres.h"
#include "CubeSystemTreeNode.h"
#include "CubeTypes.h"

using namespace std;
using namespace cube;
using namespace services;

Metric::Metric( const std::string& disp_name,
                const std::string& uniq_name,
                const std::string& dtype,
                const std::string& uom,
                const std::string& val,
                const std::string& url,
                const std::string& descr,
                FileFinder*        ffinder,
                Metric*            parent,
                uint32_t           id,
                const std::string& _expression,
                const std::string& _expression_init,
                const std::string& _expression_aggr_plus,
                const std::string& _expression_aggr_minus,
                const std::string& _expression_aggr_aggr,
                bool               rowwise,
                VizTypeOfMetric    _is_ghost
                )
    : SerializableVertex( parent, id ),
    disp_name( disp_name ),
    uniq_name( uniq_name ),
    dtype( dtype ),
    uom( uom ),
    val( val ),
    url( url ),
    descr( descr ),
    filefinder( ffinder ),
    expression( _expression ),
    expression_init( _expression_init ),
    expression_aggr_plus( _expression_aggr_plus ),
    expression_aggr_minus( _expression_aggr_minus ),
    expression_aggr_aggr( _expression_aggr_aggr ),
    is_rowwise( rowwise ),
    is_ghost( _is_ghost )
{
    own_data_type         = get_data_type();
    adv_sev_mat           = NULL;
    cache                 = NULL;
    initialized           = false;
    evaluation            = NULL;
    init_evaluation       = NULL;
    aggr_plus_evaluation  = NULL;
    aggr_minus_evaluation = NULL;
    aggr_aggr_evaluation  = NULL;
    cubepl_memory_manager = NULL;
    active                = true;
    used_by_others        = false;
    setConvertible( true );
    setCacheable( true );
    my_strategy  = BasicStrategy::selectInitialStrategy();          // cube::CUBE_ALL_IN_MEMORY_STRATEGY;
    metric_value = selectValueOnDataType( own_data_type );
    metric_value->processArguments( dtype_arg_vec );
    metric_value->init_new();
}



Metric::~Metric()
{
    delete cache;
    delete adv_sev_mat;
    delete evaluation;
    delete init_evaluation;
    delete aggr_plus_evaluation;
    delete aggr_minus_evaluation;
    delete aggr_aggr_evaluation;
    metric_value->Free();
}


void
Metric::writeData()
{
    if ( adv_sev_mat != NULL )
    {
        adv_sev_mat->writeData();
    }
}


std::string
Metric::get_val() const
{
    Metric* _parent = get_parent();
    if ( val != "" || ( _parent == NULL ) )
    {
        return val;
    }
    else
    {
        return _parent->get_val();
    }
}

// ----------------- predicates ----------------------
bool
is_int8( const string& datatype )
{
    return datatype == "INT8";
}

bool
is_uint8( const string& datatype )
{
    return ( datatype == "UINT8" )
           || ( datatype == "CHAR" );
}

bool
is_int16( const string& datatype )
{
    return ( datatype == "INT16" )
           || ( datatype == "SIGNED SHORT INT" )
           || ( datatype == "SHORT INT" );
}

bool
is_uint16( const string& datatype )
{
    return ( datatype == "UINT16" )
           || ( datatype == "UNSIGNED SHORT INT" );
}

bool
is_int32( const string& datatype )
{
    return ( datatype == "INT32" )
           || ( datatype == "SIGNED INT" )
           || ( datatype == "INT" );
}

bool
is_uint32( const string& datatype )
{
    return ( datatype == "UINT32" )
           || ( datatype == "UNSIGNED INT" );
}

bool
is_int64( const string& datatype )
{
    return ( datatype == "INT64" )
           || ( datatype == "SIGNED INTEGER" )
           || ( datatype == "INTEGER" );
}

bool
is_uint64( const string& datatype )
{
    return ( datatype == "UINT64" )
           || ( datatype == "UNSIGNED INTEGER" );
}

bool
is_double( const string& datatype )
{
    return ( datatype == "DOUBLE" )
           || ( datatype == "FLOAT" );
}

bool
is_complex( const string& datatype )
{
    return datatype == "COMPLEX";
}

bool
is_min_double( const string& datatype )
{
    return datatype == "MINDOUBLE";
}

bool
is_max_double( const string& datatype )
{
    return datatype == "MAXDOUBLE";
}

bool
is_rate( const string& datatype )
{
    return datatype == "RATE";
}

bool
is_tau_atomic( const string& datatype )
{
    return datatype == "TAU_ATOMIC";
}

bool
is_histogram( const string& datatype )
{
    return datatype == "HISTOGRAM";
}

bool
is_ndoubles( const string& datatype )
{
    return datatype == "NDOUBLES";
}

bool
is_scale_func( const string& datatype )
{
    return datatype == "SCALE_FUNC";
}


bool
is_intrinsic_type( const string& datatype )
{
    return is_double( datatype )
           || is_int64( datatype )
           || is_uint64( datatype )
           || is_int32( datatype )
           || is_uint32( datatype )
           || is_int16( datatype )
           || is_uint16( datatype )
           || is_int8( datatype )
           || is_uint8( datatype );
}



// ------------------ adv part -----------------------


void
Metric::printLocalCallTreeIDs()
{
    cout << " ------- LOCAL IDS --------- " << endl;
    size_t idx = 0;
    for ( vector<signed int>::iterator iter = calltree_local_ids.begin(); iter != calltree_local_ids.end(); ++iter, ++idx )
    {
        cout << idx << " -> " << ( *iter ) << endl;
    }
    cout << " --------------------------- " << endl;
}



void
Metric::setStrategy( CubeStrategy strategy )
{
    my_strategy = strategy; // store my own strategy for postponed initiailzation: in create_data_container
    if ( adv_sev_mat != NULL )
    {
        adv_sev_mat->setStrategy( strategy );
    }
}


void
Metric::dropRow( Cnode* cnode )
{
    if ( !isInitialized() )
    {
        return;
    }
    if ( cnode->is_clustered() )
    {
        for ( size_t i = 0; i < sysv.size(); ++i )
        {
            adv_sev_mat->dropRow( calltree_local_ids[ cnode->get_remapping_cnode( sysv[ i ]->get_parent()->get_rank() )->get_id() ] );
        }
    }
    else
    {
        adv_sev_mat->dropRow( calltree_local_ids[ cnode->get_remapping_cnode()->get_id() ] );
    }
}

void
Metric::dropAllRows()
{
    if ( used_by_others )
    {
        return;
    }
    if ( isInitialized() && adv_sev_mat != NULL )
    {
        adv_sev_mat->dropAllRows();
    }
    if ( cache )
    {
        cache->invalidate();
    }
}

void
Metric::create_data_container( uint32_t n_cid, uint32_t n_tid /*, bool existing */ )
{
    // Do not proceed without a valid filefinder pointer.
    // A NULL-pointer may indicate Metric creation apart from an actual Cube file.
    if ( !filefinder )
    {
        return;
    }

    if ( initialized )
    {
        return;
    }

    ncid = n_cid;
    ntid = n_tid;
    if ( get_type_of_metric() == CUBE_METRIC_POSTDERIVED ||
         get_type_of_metric() == CUBE_METRIC_PREDERIVED_INCLUSIVE ||
         get_type_of_metric() == CUBE_METRIC_PREDERIVED_EXCLUSIVE
         )
    {
        if ( evaluation != NULL )
        {
            evaluation->setRowSize( ntid );
        }
        if ( init_evaluation != NULL )
        {
            init_evaluation->setRowSize( ntid );
        }
        return;
    }
    if ( adv_sev_mat != NULL )
    {
        delete adv_sev_mat;
        adv_sev_mat = NULL;
    }
    if ( adv_sev_mat == NULL )
    {
        cache_selection( ncid, ntid, get_type_of_metric() );
        adv_sev_mat =
            new RowWiseMatrix( filefinder->getMetricData( this ), filefinder->getMetricIndex( this ), ncid, n_tid, metric_value );
        setStrategy( my_strategy );
    }
    initialized = true;
}



row_of_objects_t*
Metric::create_calltree_id_maps( IDdeliverer* ids, Cnode* root, row_of_objects_t* _row )
{
    ObjectsEnumerator enumerator;
    ids->reset();
    row_of_objects_t* row = enumerator.get_objects_to_enumerate( root, _row );

    int i = 0;
    for ( row_of_objects_t::iterator iter = row->begin(); iter < row->end(); ++iter, ++i )
    {
        calltree_local_ids[ ( *iter )->get_id() ] = ids->get_next_id();
    }
    return row;
}



void
Metric::set_dimensions( std::vector<Cnode*>&          _cnodev,
                        std::vector<Cnode*>&          _roots_cnodev,
                        std::vector<SystemTreeNode*>& _root_stnv,
                        std::vector<SystemTreeNode*>& _stnv,
                        std::vector<LocationGroup*>&  _lgv,
                        std::vector<Location*>&       _locv,
                        std::vector<Sysres*>&         _sysresv )
{
    cnodev       = _cnodev;
    roots_cnodev = _roots_cnodev;
    roots_sysv   = _root_stnv;
    stnv         = _stnv;
    lgv          = _lgv;
    sysv         = _locv;
    sysresv      = _sysresv;
//     initialized  = true;
}


std::vector<std::string>
Metric::split_dtype_args()
{
    vector<string>v;
    string        arg_str;
    size_t        pos = 0;
    // find opening bracket, if not present return empty vector
    for (; ( pos < dtype.size() ) && ( dtype[ pos ] != '(' ); ++pos )
    {
        ;                                                       // { }
    }
    if ( pos == dtype.size() )
    {
        return v;
    }
    // opening brace found, split string into base and argument part
    dtype_arg_str = dtype.substr( pos, string::npos ); // argument string, save for reuse on writing the cube file
    dtype.erase( pos, string::npos );                  // base data type
    arg_str = dtype_arg_str.substr( 1 );
    if ( arg_str[ arg_str.size() - 1 ] != ')' )
    {
        return v;                                     // malformed argument string, don't parse
    }
    // tokenize arguments into vector. not very robust, also does not
    // trim whitespace.
    pos = 0;
    string tmp;
    for (; pos < arg_str.size(); ++pos )
    {
        if ( ( arg_str[ pos ] == ',' ) || ( arg_str[ pos ] == ')' ) )
        {
            tmp = arg_str.substr( 0, pos );
            arg_str.erase( 0, pos + 1 );
            v.push_back( tmp );
            pos = 0;
        }
    }
    return v;
}

// @todo Refactor get_data_type() implementation to not rely on string comparisons.
DataType
Metric::get_data_type()
{
    if ( is_int8( dtype ) )
    {
        return CUBE_DATA_TYPE_INT8;
    }
    if ( is_uint8( dtype ) )
    {
        return CUBE_DATA_TYPE_UINT8;
    }
    if ( is_int16( dtype ) )
    {
        return CUBE_DATA_TYPE_INT16;
    }
    if ( is_uint16( dtype ) )
    {
        return CUBE_DATA_TYPE_UINT16;
    }
    if ( is_int32( dtype ) )
    {
        return CUBE_DATA_TYPE_INT32;
    }
    if ( is_uint32( dtype ) )
    {
        return CUBE_DATA_TYPE_UINT32;
    }
    if ( is_int64( dtype ) )
    {
        return CUBE_DATA_TYPE_INT64;
    }
    if ( is_uint64( dtype ) )
    {
        return CUBE_DATA_TYPE_UINT64;
    }
    if ( is_double( dtype ) )
    {
        return CUBE_DATA_TYPE_DOUBLE;
    }
    if ( is_complex( dtype ) )
    {
        return CUBE_DATA_TYPE_COMPLEX;
    }
    if ( is_tau_atomic( dtype ) )
    {
        return CUBE_DATA_TYPE_TAU_ATOMIC;
    }
    if ( is_min_double( dtype ) )
    {
        return CUBE_DATA_TYPE_MIN_DOUBLE;
    }
    if ( is_max_double( dtype ) )
    {
        return CUBE_DATA_TYPE_MAX_DOUBLE;
    }
    if ( is_rate( dtype ) )
    {
        return CUBE_DATA_TYPE_RATE;
    }
    if ( is_scale_func( dtype ) )
    {
        return CUBE_DATA_TYPE_SCALE_FUNC;
    }
    // from  here
    dtype_arg_vec = split_dtype_args();

    if ( is_histogram( dtype ) )
    {
        return CUBE_DATA_TYPE_HISTOGRAM;
    }
    if ( is_ndoubles( dtype ) )
    {
        return CUBE_DATA_TYPE_NDOUBLES;
    }

    cerr << "WARNING: Unrecognized datatype '" << dtype << "'; defaulting to 'double'." << endl;
    return CUBE_DATA_TYPE_DOUBLE;
}


IndexFormat
Metric::get_index_format()
{
    return CUBE_INDEX_FORMAT_DENSE;
}


void
Metric::setMemoryManager( CubePLMemoryManager* _manager )
{
    cubepl_memory_manager = _manager;
    cubepl_memory_manager->set_local_memory_for_metric( get_id() );
}

// /< Cube parses expression and creates an hierarchy of evaluators, which is saved here

void
Metric::setEvaluation( GeneralEvaluation* _ev )
{
    delete evaluation;
    invalidateCache();
    evaluation = _ev;
    if ( evaluation != NULL )
    {
        evaluation->set_metric_id( get_id() );
    }
}

void
Metric::setInitEvaluation( GeneralEvaluation* _ev )
{
    delete init_evaluation;
    invalidateCache();
    init_evaluation = _ev;
    if ( init_evaluation != NULL )
    {
        init_evaluation->set_metric_id( get_id() );
    }
}

void
Metric::setAggrPlusEvaluation( GeneralEvaluation* _ev )
{
    delete aggr_plus_evaluation;
    invalidateCache();
    aggr_plus_evaluation = _ev;
    if ( aggr_plus_evaluation != NULL )
    {
        aggr_plus_evaluation->set_metric_id( get_id() );
    }
}

void
Metric::setAggrMinusEvaluation( GeneralEvaluation* _ev )
{
    delete aggr_minus_evaluation;
    invalidateCache();
    aggr_minus_evaluation = _ev;
    if ( aggr_minus_evaluation != NULL )
    {
        aggr_minus_evaluation->set_metric_id( get_id() );
    }
}

void
Metric::setAggrAggrEvaluation( GeneralEvaluation* _ev )
{
    delete aggr_aggr_evaluation;
    invalidateCache();
    aggr_aggr_evaluation = _ev;
    if ( aggr_aggr_evaluation != NULL )
    {
        aggr_aggr_evaluation->set_metric_id( get_id() );
    }
}



vector<Cnode*>
Metric::get_optimal_sequence( const vector<Cnode*>& calltree )
{
    vector<Cnode*>v( calltree.size(), NULL );
    for ( vector<Cnode*>::const_iterator iter = calltree.begin(); iter < calltree.end(); ++iter )
    {
        v[ calltree_local_ids[ ( *iter )->get_id() ] ] = *iter;
    }
    return v;
}

void
Metric::set_adv_sev_adv( Cnode* cnode, Thread* thrd, Value* value )
{
    if ( cnode != NULL && thrd != NULL && adv_sev_mat != NULL )
    {
        int64_t process_rank = thrd->get_parent()->get_rank();
        adv_sev_mat->setValue( value, calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ], thrd->get_id() );
    }
}

Value*
Metric::get_adv_sev_adv( Cnode* cnode, Thread* thrd )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }
    int64_t process_rank = thrd->get_parent()->get_rank();
    Value*  v            =  adv_sev_mat->getValue( calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ], thrd->get_id() );
    int64_t _norm        = cnode->get_cluster_normalization( process_rank );
    if ( _norm > 0 )
    {
        v->normalizeWithClusterCount( ( uint64_t )_norm );
    }
    return v;
}




void
Metric::set_adv_sev( Cnode* cnode, Thread* thrd, double value )
{
    if ( cnode != NULL && thrd != NULL && adv_sev_mat != NULL )
    {
        int64_t process_rank = thrd->get_parent()->get_rank();
        adv_sev_mat->setValue( value, calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ], thrd->get_id() );
    }
    else
    {
        cerr << " Something is wrong in Metric::set_adv_set(" << cnode << ", " << thrd << ", " << value << ")" << endl;
    }
}

double
Metric::get_adv_sev( Cnode* cnode, Thread* thrd )
{
    Value* v = get_adv_sev_adv( cnode, thrd );
    if ( v == NULL )
    {
        return 0.;
    }
    double d = v->getDouble();
    v->Free();
    return d;
}


// / ------------------------ Sum over all threads for given cnode and all subcnodes


double
Metric::get_sev( Cnode* cnode, CalculationFlavour cnf )
{
    double ret = 0.;
    Value* v   = get_sev_adv( cnode, cnf );
    if ( v == NULL )
    {
        return 0.;
    }
    ret = v->getDouble();
    v->Free();
    return ret;
}



double
Metric::get_sev( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    double ret = 0.;
    Value* v   = get_sev_adv( cnode, cnf, sys, sf );
    if ( v == NULL )
    {
        return 0.;
    }
    ret = v->getDouble();
    v->Free();
    return ret;
}

// inclusive value and exclusive value in this metric are same
Value*
Metric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    Value* v =  metric_value->clone();
    for ( unsigned i = 0; i < ntid; ++i )
    {
        const cube::Cnode* _mapped =  cnode->get_remapping_cnode( sysv[ i ]->get_parent()->get_rank()   );
        Value*             _v      =    ( _mapped == NULL ) ? v->clone() : ( adv_sev_mat->getValue( calltree_local_ids[ _mapped->get_id() ], i ) );
        int64_t            _norm   = cnode->get_cluster_normalization( sysv[ i ]->get_parent()->get_rank() );
        if ( _norm > 0 )
        {
            _v->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        v->
        operator+=( _v );

        _v->Free();
    }
    return v;
}






// inclusive value and exclusive value in this metric are same
//
Value*
Metric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    Value* v = NULL;
    v = adv_sev_mat->getValue( calltree_local_ids[ cnode->get_id() ], sys->get_id() );
    return v;
}

// // // ---------------------- rowwise calls ---------------------

// --------// inclusive value and exclusive value in this metric are same
Value**
Metric::get_sevs_adv( Cnode* cnode, CalculationFlavour cnf )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }
    char* _v =   adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
    if ( _v == NULL )
    {
        return NULL;
    }
    Value** to_return = services::create_row_of_values( ntid );
    char*   start     = _v;
    for ( unsigned i = 0; i < ntid; ++i )
    {
        Value* _val = metric_value->clone();
        start = _val->fromStream( start );

        int64_t _norm = cnode->get_cluster_normalization( sysv[ i ]->get_parent()->get_rank() );
        if ( _norm > 0 )
        {
            _val->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        to_return[ i ] = _val;
    }
    return to_return;
}









//  --massive calculation of a system tree . stub
void
Metric::get_system_tree_sevs_adv( Cnode*               cnode,
                                  CalculationFlavour   cnf,
                                  std::vector<Value*>& inclusive_values,
                                  std::vector<Value*>& exclusive_values
                                  )
{
    inclusive_values.resize( sysresv.size(), NULL );
    exclusive_values.resize( sysresv.size(), NULL );
    Value** _locs = NULL;
    if ( !isBuildIn() )
    {
        _locs = get_sevs_adv( cnode, cnf );
    }
    else
    {
        double* __tmp_locs =  get_sevs( cnode, cnf );
        _locs = cube::services::create_row_of_values( ntid );
        for ( size_t i = 0; i < ntid; ++i )
        {
            Value* _tmp_value = metric_value->clone();
            _tmp_value->
            operator=( ( __tmp_locs != NULL ) ? __tmp_locs[ i ] : 0. );

            _locs[ i ] = _tmp_value;
        }
        cube::services::delete_raw_row( __tmp_locs );
    }
    // first copy locations
    for ( size_t i = 0; i < ntid; ++i )
    {
        exclusive_values[ sysv[ i ]->get_sys_id() ] = _locs[ i ];
        inclusive_values[ sysv[ i ]->get_sys_id() ] = _locs[ i ]->copy();
    }
    // set non-location elements to zero
    for ( size_t i = 0; i < exclusive_values.size(); ++i )
    {
        if ( exclusive_values[ i ] == NULL )
        {
            exclusive_values[ i ] = metric_value->clone();
        }
        if ( inclusive_values[ i ] == NULL )
        {
            inclusive_values[ i ] = metric_value->clone();
        }
    }
    if ( !isCustom() && get_type_of_metric() != cube::CUBE_METRIC_POSTDERIVED )
    {
        // calculate  location groups
        for ( size_t i = 0; i < lgv.size(); ++i )
        {
            LocationGroup* lg = lgv[ i ];
            for ( size_t j = 0; j < lg->num_children(); j++ )
            {
                Location* _loc = lg->get_child( j );
                inclusive_values[ lg->get_sys_id() ]->operator+=( _locs[ _loc->get_id() ] );

                //add up to all parents ... need this coz of hybrid character of
                // the system tree model
                SystemTreeNode* _lg_parent = lg->get_parent();
                while ( _lg_parent != NULL )
                {
                    inclusive_values[ _lg_parent->get_sys_id() ]->operator+=( _locs[ _loc->get_id() ] );
                    _lg_parent = _lg_parent->get_parent();
                }
            }
        }
    }
    else
    {
        // calculate  location groups
        for ( size_t i = 0; i < lgv.size(); ++i )
        {
            LocationGroup* lg = lgv[ i ];
            inclusive_values[ lg->get_sys_id() ]->operator=( this->get_sev( cnode, cnf, lg, cube::CUBE_CALCULATE_INCLUSIVE ) );
        }
        for ( size_t i = 0; i < stnv.size(); ++i )
        {
            SystemTreeNode* stn = stnv[ i ];
            inclusive_values[ stn->get_sys_id() ]->operator=( this->get_sev( cnode, cnf, stn, cube::CUBE_CALCULATE_INCLUSIVE ) );
        }
    }
    return;
}

//  --massive calculation of a system tree. stub
void
Metric::get_system_tree_sevs( Cnode*               cnode,
                              CalculationFlavour   cnf,
                              std::vector<double>& inclusive_values,
                              std::vector<double>& exclusive_values
                              )
{
    inclusive_values.resize( sysresv.size(), 0. );
    exclusive_values.resize( sysresv.size(), 0. );
    vector<Value*>_inclusive_values;
    vector<Value*>_exclusive_values;
    get_system_tree_sevs_adv( cnode, cnf, _inclusive_values, _exclusive_values );

    for ( size_t i = 0; i < exclusive_values.size(); ++i )
    {
        exclusive_values[ i ] = _exclusive_values[ i ]->getDouble();
        _exclusive_values[ i ]->Free();
    }
    for ( size_t i = 0; i < inclusive_values.size(); ++i )
    {
        inclusive_values[ i ] = _inclusive_values[ i ]->getDouble();
        _inclusive_values[ i ]->Free();
    }
    return;
}




char*
Metric::get_sevs_raw( Cnode* cnode, CalculationFlavour cnf )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }

    return adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
}




double*
Metric::get_sevs( Cnode* cnode, CalculationFlavour cnf )
{
    Value** _v        =   get_sevs_adv( cnode, cnf );
    double* to_return = services::create_row_of_doubles( ntid );
    if ( _v == NULL )
    {
        return to_return;
    }
    for ( unsigned i = 0; i < ntid; ++i )
    {
        to_return[ i ] = _v[ i ]->getDouble();
    }
    services::delete_row_of_values( _v, ntid );
    return to_return;
}



Value**
Metric::get_saved_sevs_adv( Cnode* cnode )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }

    char* _v =   adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
    if ( _v == NULL )
    {
        return NULL;
    }
    Value** to_return = services::create_row_of_values( ntid );
    char*   start     = _v;
    for ( unsigned i = 0; i < ntid; ++i )
    {
        Value* _val = metric_value->clone();
        start = _val->fromStream( start );

        int64_t _norm = cnode->get_cluster_normalization( sysv[ i ]->get_parent()->get_rank() );
        if ( _norm > 0 )
        {
            _val->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        to_return[ i ] = _val;
    }
    return to_return;
}


char*
Metric::get_saved_sevs_raw( Cnode* cnode )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }

    return adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
}




double*
Metric::get_saved_sevs( Cnode* cnode )
{
    Value** _v        =   get_saved_sevs_adv( cnode );
    double* to_return = services::create_row_of_doubles( ntid );
    if ( _v == NULL )
    {
        return to_return;
    }
    for ( unsigned i = 0; i < ntid; ++i )
    {
        to_return[ i ] = _v[ i ]->getDouble();
    }
    services::delete_row_of_values( _v, ntid );
    return to_return;
}



void
Metric::set_sevs_raw( Cnode* cnode,
                      char*  _val
                      )
{
    if ( adv_sev_mat == NULL )
    {
        return;
    }

    adv_sev_mat->setRow( _val, calltree_local_ids[ cnode->get_id() ] );
}

// ------------------ adv part end -------------------

// ------------------OLD PART ---- -
void
Metric::set_sev( Cnode* cnode, Thread* thrd, double value )
{
// #warning "Backward compatibility" .
    set_adv_sev( cnode, thrd, value );
}

double
Metric::get_sev( Cnode* cnode, Thread* thrd )
{
    double ret = 0.;
    Value* v   = get_sev_adv( cnode, thrd );
    if ( v == NULL )
    {
        return 0.;
    }
    ret = v->getDouble();
    v->Free();
    return ret;
}


Value*
Metric::get_sev_adv( Cnode* cnode, Thread* thrd )
{
    if ( adv_sev_mat != NULL )
    {
        Value*             v            =  metric_value->clone();
        int64_t            process_rank = thrd->get_parent()->get_rank();
        const cube::Cnode* _mapped      =  cnode->get_remapping_cnode( process_rank  );
        Value*             _v           = ( _mapped == NULL ) ? v->clone() : ( adv_sev_mat->getValue(
                                                                                   calltree_local_ids[ _mapped->get_id() ],
                                                                                   thrd->get_id() ) );
        int64_t _norm = cnode->get_cluster_normalization( process_rank );
        if ( ( _norm > 0 ) &&  ( _v != NULL ) )
        {
            _v->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        v->Free();
        return _v;
    }
    Value* v =  metric_value->clone();
    return v;
}



double
Metric::sum()
{
// #warning "SUM is not implemented yet. The meaning of this method is malicious for inclusive of simple metrics. In the future will disapear."
    cerr << " Operation SUM over whole metric is not implemented yet. It returns always \"12345\"." << endl;
    return 12345.;
}

void
Metric::clear()
{
// #warning "clear() is not implemented yet"
    cerr << " clear() of metric is not implemented yet. Neither SCALASCA, nor GUI calls it. " << endl;
}






void
Metric::writeXML_data( ostream& out,  std::vector<Cnode*>& _cnodev,  std::vector<Thread*>& _thrdv )
{
    if ( get_val() == "VOID" )
    {
        return;
    }

    std::vector<Thread*>__thrd_sorted = _thrdv;
    sort( __thrd_sorted.begin(), __thrd_sorted.end(), &IdentObject::id_compare );



    out << "<matrix metricId=\"" << get_id() << "\">" <<  '\n';

    for ( std::vector<Cnode*>::iterator citer = _cnodev.begin(); citer < _cnodev.end(); ++citer )
    {
        if ( ( *citer )->isHidden() )  // do not write hidden cnodes. Their value is included in the exclusive value of parent.
        {
            continue;
        }
        out << "<row cnodeId=\"" << ( *citer )->get_id() << "\">" <<  '\n';
        std::vector<Thread*>::iterator siter = __thrd_sorted.begin();
        for (; siter < __thrd_sorted.end(); ++siter )
        {
            Value* v = get_sev_adv( *citer, CUBE_CALCULATE_EXCLUSIVE, *siter, CUBE_CALCULATE_EXCLUSIVE );  // / CUBE 3 saves exclusive values alond Calltree
            if ( v != NULL )
            {
                out << v->getString() <<  '\n';
                v->Free();
            }
            else
            {
                out << "0" <<  '\n';
            }
        }
        out << "</row>" << flush <<  '\n';
    }
    out << "</matrix>" << flush <<  '\n';
}

void
Metric::writeXML( ostream& out, bool cube3_export ) const
{
    out << indent() << "    <metric id=\"" << get_id();
    if ( !cube3_export )
    {
        out << "\" type=\"" << get_metric_kind(); // / CUBE 3 conform. no spezification about type of metric
        if ( isGhost() )
        {
            out << "\" viztype=\"GHOST";
        }
        if ( !isConvertible() )
        {
            out << "\" convertible=\"false";
        }
        if ( !isCacheable() )
        {
            out << "\" cacheable=\"false";
        }
    }
    out << "\">" <<  '\n';

    out << indent() << "      <disp_name>" << escapeToXML( get_disp_name() ) << "</disp_name>" <<  '\n';
    out << indent() << "      <uniq_name>" << escapeToXML( get_uniq_name() ) << "</uniq_name>" <<  '\n';
    out << indent() << "      <dtype>" << escapeToXML( get_dtype_with_args() ) << "</dtype>" <<  '\n';
    out << indent() << "      <uom>"  << escapeToXML( get_uom() ) << "</uom>" <<  '\n';
    if ( !get_val().empty() )
    {
        out << indent() << "      <val>" << escapeToXML( get_val() ) << "</val>\n";
    }
    out << indent() << "      <url>" << escapeToXML( get_url() ) << "</url>" <<  '\n';
    out << indent() << "      <descr>" << escapeToXML( get_descr() ) << "</descr>" <<  '\n';
    if ( !cube3_export  && ( get_type_of_metric() == CUBE_METRIC_POSTDERIVED || get_type_of_metric() == CUBE_METRIC_PREDERIVED_INCLUSIVE ||  get_type_of_metric() == CUBE_METRIC_PREDERIVED_EXCLUSIVE ) )
    {
        out << indent() << "      <cubepl";
        if ( is_rowwise == false )
        {
            out << " rowwise=false";
        }
        out << ">" << escapeToXML( get_expression() ) << "</cubepl>" <<  '\n';
        if ( get_init_expression() != "" )
        {
            out << indent() << "      <cubeplinit>" << escapeToXML( get_init_expression() ) << "</cubeplinit>" <<  '\n';
        }
        if ( get_aggr_plus_expression() != "" )
        {
            out << indent() << "      <cubeplaggr cubeplaggrtype=plus>" << escapeToXML( get_aggr_plus_expression() ) << "</cubeplaggr>" <<  '\n';
        }
        if ( get_aggr_minus_expression() != "" )
        {
            out << indent() << "      <cubeplaggr cubeplaggrtype=minus>" << escapeToXML( get_aggr_minus_expression() ) << "</cubeplaggr>" <<  '\n';
        }
        if ( get_aggr_aggr_expression() != "" )
        {
            out << indent() << "      <cubeplaggr cubeplaggrtype=aggr>" << escapeToXML( get_aggr_aggr_expression() ) << "</cubeplaggr>" <<  '\n';
        }
    }

    writeAttributes( out,  indent() + "      ", cube3_export  );


    for ( unsigned int i = 0; i < num_children(); ++i )
    {
        const Metric* p = get_child( i );
        p->writeXML( out, cube3_export );
    }
    out << indent() << "    </metric>" <<  '\n';
}


// --------- preparation calls for derived metrics---- defined here, used from derived metrics

void
Metric::post_calculation_cleanup()
{
    cubepl_memory_manager->get_memory_initializer()->memory_cleanup( get_id() );
}
void
Metric::pre_calculation_preparation( Cnode* cnode )
{
    setup_cubepl_memory();
    setup_cubepl_memory_for_itself();
    setup_cubepl_memory_for_cnode( cnode );
}


void
Metric::pre_calculation_preparation( Cnode* cnode, Sysres* sys )
{
    setup_cubepl_memory();
    setup_cubepl_memory_for_itself();
    setup_cubepl_memory_for_cnode( cnode );

    setup_cubepl_memory_for_sysres( sys );
}

void
Metric::setup_cubepl_memory()
{
    cubepl_memory_manager->get_memory_initializer()->memory_new( get_id() );
}

void
Metric::setup_cubepl_memory_for_itself()
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup( this, get_id() );
}

void
Metric::setup_cubepl_memory_for_cnode( Cnode* cnode )
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup(  cnode, get_id() );
    setup_cubepl_memory_for_region( cnode->get_callee() );
}

void
Metric::setup_cubepl_memory_for_region( Region* region )
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup( region, get_id()  );
}

void
Metric::setup_cubepl_memory_for_sysres( Sysres* sysres )
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup(  sysres, get_id() );
}





// -------------------- GET TYPE OF METRIC
Value*
Metric::get_metric_value()
{
    return metric_value->clone();
}

Value*
Metric::its_value()
{
    return metric_value;
}



bool
Metric::asInclusiveMetric()
{
    return metric_value->asInclusiveMetric();
}

bool
Metric::asExclusiveMetric()
{
    return metric_value->asExclusiveMetric();
}


TypeOfMetric
Metric::get_type_of_metric( std::string s_kind )
{
    if ( s_kind == "" )
    {
        return CUBE_METRIC_EXCLUSIVE;
    }
    if ( s_kind == "POSTDERIVED"  || s_kind == "DERIVED" )
    {
        return CUBE_METRIC_POSTDERIVED;
    }
    if ( s_kind == "PREDERIVED_INCLUSIVE" )
    {
        return CUBE_METRIC_PREDERIVED_INCLUSIVE;
    }
    if ( s_kind == "PREDERIVED_EXCLUSIVE" )
    {
        return CUBE_METRIC_PREDERIVED_EXCLUSIVE;
    }
    if ( s_kind == "EXCLUSIVE" )
    {
        return CUBE_METRIC_EXCLUSIVE;
    }
    if ( s_kind == "INCLUSIVE" )
    {
        return CUBE_METRIC_INCLUSIVE;
    }
    if ( s_kind == "SIMPLE" )
    {
        return CUBE_METRIC_SIMPLE;
    }
    return CUBE_METRIC_EXCLUSIVE;
};


VizTypeOfMetric
Metric::get_viz_type_of_metric( const std::string& s_kind )
{
    std::string _s_kind = services::lowercase( s_kind );
    if ( _s_kind == "" || _s_kind == "normal" )
    {
        return CUBE_METRIC_NORMAL;
    }
    if ( _s_kind == "ghost"  )
    {
        return CUBE_METRIC_GHOST;
    }
    return CUBE_METRIC_NORMAL;
};

bool
Metric::isIntegerType()
{
    switch ( get_data_type() )
    {
        case CUBE_DATA_TYPE_UINT64:
        case CUBE_DATA_TYPE_INT64:
        case CUBE_DATA_TYPE_UINT32:
        case CUBE_DATA_TYPE_INT32:
        case CUBE_DATA_TYPE_UINT16:
        case CUBE_DATA_TYPE_INT16:
        case CUBE_DATA_TYPE_UINT8:
        case CUBE_DATA_TYPE_INT8:
            return true;
        default:
            return false;
    }
}


bool
Metric::weakEqual( Metric* _m )
{
    if ( get_uniq_name() != _m->get_uniq_name() )
    {
        return false;
    }
    return true;
}

bool
Metric::weakEqual( Vertex* _v )
{
    cerr << " Hook for weakEqual" << endl;
    return false;
}

void
cube::Metric::invalidateCachedValue( Cnode* cnode, CalculationFlavour cf, Sysres* sysres, CalculationFlavour sf )
{
    cache->invalidateCachedValue( cnode, cf, sysres, sf );
}


void
cube::Metric::invalidateCache( void )
{
    cache->invalidate();
}


void
cube::Metric::cache_selection( uint32_t n_cid, uint32_t n_tid, TypeOfMetric type )
{
    delete cache;
    cache =  new SimpleCache<double>( n_cid, n_tid, metric_value->getSize(), ( type == CUBE_METRIC_INCLUSIVE ) ? CUBE_CALCULATE_INCLUSIVE : CUBE_CALCULATE_EXCLUSIVE );
}

Metric*
cube::Metric::create( const string&   display_name,
                      const string&   unique_name,
                      const string&   datatype,
                      const string&   unit_of_measure,
                      const string&   val,
                      const string&   url,
                      const string&   descr,
                      FileFinder*     file_finder,
                      Metric*         parent,
                      TypeOfMetric    type_of_metric,
                      uint32_t        id,
                      const string&   cubepl_expression,
                      const string&   cubepl_init_expression,
                      const string&   cubepl_aggr_plus_expression,
                      const string&   cubepl_aggr_minus_expression,
                      const string&   cubepl_aggr_aggr_expression,
                      bool            row_wise,
                      VizTypeOfMetric visibility )
{
    Metric* met = NULL;
    switch ( type_of_metric )
    {
        case CUBE_METRIC_POSTDERIVED:
        {
            std::string parent_dtype = datatype;
            if ( parent != NULL )
            {
                parent_dtype = parent->get_dtype();
                if ( !is_intrinsic_type( parent_dtype ) )
                {
                    cerr << " Derived metrics can only be created for parent metrics with intrinsic value data type." << endl;
                    return NULL;
                }
            }
            met = new PostDerivedMetric( display_name, unique_name,  parent_dtype, unit_of_measure, val, url,
                                         descr, file_finder, parent, id, cubepl_expression, cubepl_init_expression, row_wise, visibility );
            break;
        }
        case CUBE_METRIC_PREDERIVED_INCLUSIVE:
        {
            std::string parent_dtype = datatype;
            if ( parent != NULL )
            {
                parent_dtype = parent->get_dtype();
                if ( !is_intrinsic_type( parent_dtype ) )
                {
                    cerr << " Derived metrics can only be created for parent metrics with intrinsic value data type." << endl;
                    return NULL;
                }
            }

            if ( cubepl_aggr_plus_expression != "" || cubepl_aggr_minus_expression != "" || cubepl_aggr_aggr_expression != "" )
            {
                met = new CustomPreDerivedInclusiveMetric( display_name, unique_name,  parent_dtype, unit_of_measure, val, url,
                                                           descr, file_finder, parent, id, cubepl_expression, cubepl_init_expression, cubepl_aggr_plus_expression, cubepl_aggr_minus_expression, cubepl_aggr_aggr_expression, row_wise, visibility  );
            }
            else
            {
                met = new PreDerivedInclusiveMetric( display_name, unique_name,  parent_dtype, unit_of_measure, val, url,
                                                     descr, file_finder, parent, id, cubepl_expression, cubepl_init_expression, cubepl_aggr_plus_expression, cubepl_aggr_minus_expression, cubepl_aggr_aggr_expression, row_wise, visibility  );
            }
            break;
        }
        case CUBE_METRIC_PREDERIVED_EXCLUSIVE:
        {
            std::string parent_dtype = datatype;
            if ( parent != NULL )
            {
                parent_dtype = parent->get_dtype();
                if ( !is_intrinsic_type( parent_dtype ) )
                {
                    cerr << " Derived metrics can only be created for parent metrics with intrinsic value data type." << endl;
                    return NULL;
                }
            }
            if ( cubepl_aggr_plus_expression != "" || cubepl_aggr_aggr_expression != "" )
            {
                met = new CustomPreDerivedExclusiveMetric( display_name, unique_name, parent_dtype, unit_of_measure, val, url,
                                                           descr, file_finder, parent, id, cubepl_expression, cubepl_init_expression, cubepl_aggr_plus_expression, cubepl_aggr_aggr_expression, row_wise, visibility  );
            }
            else
            {
                met = new PreDerivedExclusiveMetric( display_name, unique_name, parent_dtype, unit_of_measure, val, url,
                                                     descr, file_finder, parent, id, cubepl_expression, cubepl_init_expression, cubepl_aggr_plus_expression, cubepl_aggr_aggr_expression, row_wise, visibility  );
            }
            break;
        }
        case CUBE_METRIC_EXCLUSIVE:
            if ( is_double( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<double>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                              descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint64( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<uint64_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                                descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int64( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<int64_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint32( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<uint32_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                                descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int32( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<int32_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint16( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<uint16_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                                descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int16( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<int16_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint8( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<uint8_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int8( datatype ) )
            {
                met = new ExclusiveBuildInTypeMetric<int8_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                              descr, file_finder, parent, id, visibility  );
            }
            else                                                                                // use general implementation
            {
                met = new ExclusiveMetric( display_name, unique_name, datatype, unit_of_measure, val, url,
                                           descr, file_finder, parent, id, visibility  );
            }
            if ( !met->asExclusiveMetric() )
            {
                cerr << " Metric " << unique_name << " with type " << datatype << " cannot be declared as an exclusive metric" << endl;
                delete met;
                met = NULL;
            }
            break;
        case CUBE_METRIC_INCLUSIVE:
        default:
            if ( is_double( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<double>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                              descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint64( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<uint64_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                                descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int64( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<int64_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint32( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<uint32_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                                descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int32( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<int32_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint16( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<uint16_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                                descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int16( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<int16_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_uint8( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<uint8_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                               descr, file_finder, parent, id, visibility  );
            }
            else
            if ( is_int8( datatype ) )
            {
                met = new InclusiveBuildInTypeMetric<int8_t>( display_name, unique_name, datatype, unit_of_measure, val, url,
                                                              descr, file_finder, parent, id, visibility  );
            }
            else                                                                                // use general implementation
            {
                met = new InclusiveMetric( display_name, unique_name, datatype, unit_of_measure, val, url,
                                           descr, file_finder, parent, id, visibility  );
            }
            if ( !met->asInclusiveMetric() )
            {
                cerr << " Metric " << unique_name << " with type " << datatype << " cannot be declared as an inclusive metric" << endl;
                delete met;
                met = NULL;
            }
            break;
    }

    return met;
}

void
cube::Metric::pack( Connection& connection ) const
{
    // First pack base class
    SerializableVertex::pack( connection );

    connection << disp_name;
    connection << uniq_name;
    connection << dtype;
    connection << uom;
    connection << val;
    connection << url;
    connection << descr;
    connection << int64_t( ( parent ) ? parent->get_id() : int64_t( -1 ) );
    connection << expression;
    connection << expression_init;
    connection << expression_aggr_plus;
    connection << expression_aggr_minus;
    connection << int8_t( this->is_rowwise ? 1 : 0 );
    connection << int8_t( this->is_ghost );
}

cube::Metric::Metric( Connection&      connection,
                      const CubeProxy& cubeProxy )
    : SerializableVertex( connection )
{
    connection >> disp_name;
    connection >> uniq_name;
    connection >> dtype;
    connection >> uom;
    connection >> val;
    connection >> url;
    connection >> descr;
    int64_t parent_id = connection.get<int64_t>();
    assert( parent_id == -1 || parent_id < ( int64_t )( cubeProxy.getMetrics().size() ) );
    set_parent( parent_id == -1 ? NULL : cubeProxy.getMetrics()[ parent_id ] );
    connection >> expression;
    connection >> expression_init;
    connection >> expression_aggr_plus;
    connection >> expression_aggr_minus;
    is_rowwise = connection.get<uint8_t>();
    is_ghost   = static_cast<VizTypeOfMetric>( connection.get<uint8_t>() );
}

Metric*
cube::Metric::create( Connection&      connection,
                      const CubeProxy& cubeProxy )
{
    Serializable* object = MySerializablesFactory::getInstance().create( connection, cubeProxy );
    assert( object );
    Metric* metric = dynamic_cast<Metric*>( object );
    assert( metric );
    return metric;
}

std::ostream&
cube::Metric::output( std::ostream& stream ) const
{
    SerializableVertex::output( stream );
    stream << "  disp_name : \"" << disp_name << "\"\n"
           << "  uniq_name : \"" << uniq_name << "\"\n"
           << "  dtype : \"" << dtype << "\"\n"
           << "  uom : \"" << uom << "\"\n"
           << "  val : \"" << val << "\"\n"
           << "  url : \"" << url << "\"\n"
           << "  descr : \"" << descr << "\"\n";
    if ( parent )
    {
        stream << "  parent : " << parent->get_id() << "\n";
    }
    else
    {
        stream << "  parent : NULL\n";
    }

    stream << "  expression : \"" << expression << "\"\n"
           << "  init_expression : \"" << expression_init << "\"\n"
           << "  aggr_plus_expression : \"" << expression_aggr_plus << "\"\n"
           << "  aggr_minus_expression : \"" << expression_aggr_minus << "\"\n"
           << "  is_rowwise : " << ( this->is_rowwise ? "true" : "false" ) << "\n"
           << "  is_ghost : " << ( this->is_ghost == CUBE_METRIC_GHOST ? "CUBE_METRIC_GHOST" : "CUBE_METRIC_NORMAL" ) << "\n"
           << "  active : " << ( this->active ? "true" : "false" ) << "\n"
           << "  calltree_local_ids : [ ";

    string prefix( "" );
    for ( std::vector<signed int>::const_iterator cnode = calltree_local_ids.begin(); cnode != calltree_local_ids.end(); ++cnode )
    {
        stream << prefix << *cnode;
        prefix = ", ";
    }
    stream << "  ]\n";
    return stream;
}

void
cube::Metric::isUsedByOthers( bool value )
{
    used_by_others = value;
}
