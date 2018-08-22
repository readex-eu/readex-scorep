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


#ifndef __NEGATIVE_EVALUATION_H
#define __NEGATIVE_EVALUATION_H 0

#include "CubeUnaryEvaluation.h"

namespace cube
{
class NegativeEvaluation : public UnaryEvaluation
{
public:
    NegativeEvaluation();
    NegativeEvaluation( GeneralEvaluation* );

    virtual
    ~NegativeEvaluation();

    virtual
    double
    eval() const;


    virtual
    double eval( Cnode*, CalculationFlavour, Sysres*, CalculationFlavour   ) const;


    virtual
    double eval( Cnode*, CalculationFlavour ) const;



    virtual
    double*
    eval_row( Cnode*             _cnode,
              CalculationFlavour _cf ) const;

    virtual
    void
    print() const
    {
        std::cout << "-";
        arguments[ 0 ]->print();
    };

    virtual
    double
    eval( double arg1,
          double arg2 ) const;
};
};
#endif
