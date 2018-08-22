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
 * \file CubePreDerivedExclusiveMetric.h
 * \brief Defines a metric, which calculates its value using user defined expression in CubePL"
 */
#ifndef CUBE_DERIVED_EXCLUSIVE_METRIC_H
#define CUBE_DERIVED_EXCLUSIVE_METRIC_H


#include "CubeMetric.h"
#include "CubeValues.h"
#include "CubeGeneralEvaluation.h"

/*
 *----------------------------------------------------------------------------
 *
 * class Metric
 *
 *******************************----------------------------------------------------------------------------
 */

namespace cube
{
/*  class Cnode;
   class Thread;*/

/**
 * Metric has a treelike structure and is a kind of Vertex.
 */

class PreDerivedExclusiveMetric : public Metric
{
public:

    PreDerivedExclusiveMetric( const std::string& disp_name,
                               const std::string& uniq_name,
                               const std::string& dtype,
                               const std::string& uom,
                               const std::string& val,
                               const std::string& url,
                               const std::string& descr,
                               FileFinder*        ffinder,
                               Metric*            parent,
                               uint32_t           id = 0,
                               const std::string& _expression = "",
                               const std::string& _expression_init = "",
                               const std::string& _expression_aggr_plus = "",
                               const std::string& _expression_aggr_aggr = "",
                               const bool         threadwise = true,
                               VizTypeOfMetric    is_ghost = CUBE_METRIC_NORMAL
                               )
        : Metric( disp_name, uniq_name, dtype, uom, val, url, descr, ffinder, parent, id, _expression, _expression_init, _expression_aggr_plus, "", _expression_aggr_aggr,  threadwise,  is_ghost )
    {
        adv_sev_mat = NULL;
        initialized = false; // with creation initialisation is done
        cache_selection( ncid, ntid, cube::CUBE_METRIC_EXCLUSIVE );
    }

    /// @brief
    ///     Create a metric object via Cube connection
    /// @param connection
    ///     Active Cube connection.
    /// @param cubeProxy
    ///     Cube proxy for internal cross-referencing.
    ///
    static Serializable*
    create( Connection&      connection,
            const CubeProxy& cubeProxy );

    virtual
    ~PreDerivedExclusiveMetric();

    /// @copydoc Serialization::get_serialization_key
    virtual std::string
    get_serialization_key() const;

    /// @copydoc Serialization::get_serialization_key
    static std::string
    get_static_serialization_key();

    virtual row_of_objects_t*
    create_calltree_id_maps( IDdeliverer*      _id,
                             Cnode*            cnode,
                             row_of_objects_t* row )
    {
        return row;
    };

    virtual TypeOfMetric
    get_type_of_metric() const
    {
        return CUBE_METRIC_PREDERIVED_EXCLUSIVE;
    };
    virtual std::string
    get_metric_kind() const
    {
        return "PREDERIVED_EXCLUSIVE";
    };

    virtual
    void
    initialize( void );


    using cube::Metric::get_sev_adv;


    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cnf );

    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cfv,
                 Sysres*            sys,
                 CalculationFlavour sf );

    virtual double*
    get_sevs( Cnode*             cnode,
              CalculationFlavour cfv
              );
    virtual Value**
    get_sevs_adv( Cnode*             cnode,
                  CalculationFlavour cfv
                  );

    virtual char*
    get_sevs_raw( Cnode*             cnode,
                  CalculationFlavour cfv
                  );


    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cnf );

    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv,
             Sysres*            sys,
             CalculationFlavour sf );

    virtual IndexFormat
    get_index_format()
    {
        return CUBE_INDEX_FORMAT_SPARSE;
    };

    virtual
    bool
    isBuildIn()
    {
        return true;
    };

    virtual
    bool
    isDerived()
    {
        return true;
    };

    // /< Empty hook
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
    PreDerivedExclusiveMetric( Connection&      connection,
                               const CubeProxy& cubeProxy );
};
}
#endif
