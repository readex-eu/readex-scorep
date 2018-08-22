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
 * \file CubeCustomPreDerivedExclusiveMetric.h
 * \brief Defines a metric, which calculates its value using user defined expression in CubePL"
 */
#ifndef CUBE_CUSTOM_DERIVED_EXCLUSIVE_METRIC_H
#define CUBE_CUSTOM_DERIVED_EXCLUSIVE_METRIC_H

// #include "CubeVertex.h"
// #include "CubeVertex.h"
// #include "CubeCnode.h"
// #include "CubeThread.h"
#include "CubePreDerivedExclusiveMetric.h"
// #include "CubeAdvMatrix.h"
// #include "CubeAdvancedMatrix.h"
// #include "CubeMemoryMatrix.h"
// #include "CubeIDdeliverer.h"
// #include "CubeDeepSearchEnumerator.h"
// #include "CubeValues.h"

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

class CustomPreDerivedExclusiveMetric : public PreDerivedExclusiveMetric
{
public:

    CustomPreDerivedExclusiveMetric( const std::string& disp_name,
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
        : PreDerivedExclusiveMetric( disp_name, uniq_name, dtype, uom, val, url, descr, ffinder, parent, id, _expression, _expression_init, _expression_aggr_plus, _expression_aggr_aggr, threadwise,  is_ghost )
    {
        adv_sev_mat = NULL;
        initialized = false; // with creation initialisation is done
        cache_selection( ncid, ntid, CUBE_METRIC_EXCLUSIVE );
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
    ~CustomPreDerivedExclusiveMetric();

    /// @copydoc Serialization::get_serialization_key
    ///
    virtual std::string
    get_serialization_key() const;

    virtual
    bool
    isCustom()
    {
        return true;
    };

    /// @copydoc Serialization::get_serialization_key
    static std::string
    get_static_serialization_key();

    using cube::PreDerivedExclusiveMetric::get_sev_adv;


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
    CustomPreDerivedExclusiveMetric( Connection&      connection,
                                     const CubeProxy& cubeProxy );
};
}
#endif
