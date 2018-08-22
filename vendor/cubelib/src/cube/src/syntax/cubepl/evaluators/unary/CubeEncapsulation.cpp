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



#include "config.h"

#include "CubeEncapsulation.h"
using namespace cube;
using namespace std;

EncapsulationEvaluation::EncapsulationEvaluation() : UnaryEvaluation()
{
};

EncapsulationEvaluation::EncapsulationEvaluation( GeneralEvaluation* _arg ) : UnaryEvaluation( _arg )
{
};


EncapsulationEvaluation::~EncapsulationEvaluation()
{
}

double
EncapsulationEvaluation::eval() const
{
    return arguments[ 0 ]->eval();
}



double
EncapsulationEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   ) const
{
    return arguments[ 0 ]->eval(     _cnode, _cf,
                                     _sf, _tf
                                     );
}



double
EncapsulationEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf ) const
{
    return arguments[ 0 ]->eval(     _cnode, _cf
                                     );
}

double
EncapsulationEvaluation::eval( double arg1, double arg2 ) const
{
    return arguments[ 0 ]->eval( arg1, arg2  );
}




double*
EncapsulationEvaluation::eval_row( Cnode*             _cnode,
                                   CalculationFlavour _cf ) const
{
    return arguments[ 0 ]->eval_row( _cnode, _cf );
}
