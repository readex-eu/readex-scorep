/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
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


#ifndef __DEFINED_VARIABLE_EVALUATION_H
#define __DEFINED_VARIABLE_EVALUATION_H 0

#include "CubeStringEvaluation.h"
#include "CubePLMemoryManager.h"
namespace cube
{
class DefinedVariableEvaluation : public StringEvaluation
{
protected:
    CubePLMemoryManager* memory;
    std::string          variable_name;
public:
    DefinedVariableEvaluation( std::string          _variable,
                               CubePLMemoryManager* _manager  ) : StringEvaluation(), memory( _manager ),  variable_name( _variable )
    {
    };

    virtual
    ~DefinedVariableEvaluation();

    virtual
    double
    eval() const
    {
        return ( memory->defined( variable_name ) ) ? 1. : 0.;
    }


    virtual
    double
    eval( Cnode*, CalculationFlavour, Sysres*, CalculationFlavour   ) const
    {
        return eval();
    };

    virtual
    double
    eval( Cnode*, CalculationFlavour ) const
    {
        return eval();
    };


    virtual
    std::string
    strEval() const
    {
        return ( memory->defined( variable_name ) ) ? "1" : "0";
    }




    virtual
    double*
    eval_row( Cnode*             _cnode,
              CalculationFlavour _cf ) const
    {
        return NULL;
    }

    virtual
    void
    print() const
    {
        std::cout << "defined(" << variable_name << ")";
    };

    virtual
    double
    eval( double arg1, double arg2 ) const
    {
        return eval();
    };
};
};

#endif
