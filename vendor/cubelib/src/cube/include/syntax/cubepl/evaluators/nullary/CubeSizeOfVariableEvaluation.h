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


#ifndef __SIZEOF_VARIABLE_EVALUATION_H
#define __SIZEOF_VARIABLE_EVALUATION_H 0

#include "CubeStringEvaluation.h"
#include "CubePLMemoryManager.h"
namespace cube
{
class SizeOfVariableEvaluation : public StringEvaluation
{
protected:
    CubePLMemoryManager* memory;
    MemoryAdress         variable;
    std::string          variable_name;
    KindOfVariable       kind;
public:
    SizeOfVariableEvaluation( std::string          _variable,
                              CubePLMemoryManager* _manager  ) : StringEvaluation(), memory( _manager ),  variable( memory->register_variable( _variable ) ), variable_name( _variable )
    {
        kind = memory->kind_of_variable( variable_name );
    };

    virtual
    ~SizeOfVariableEvaluation();

    virtual
    double
    eval() const;

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
    double*
    eval_row( Cnode*             _cnode,
              CalculationFlavour _cf ) const
    {
        return NULL;
    }

    virtual
    std::string
    strEval() const;



    virtual
    void
    print() const
    {
        std::cout << "sizeof(" << variable_name << ")";
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
