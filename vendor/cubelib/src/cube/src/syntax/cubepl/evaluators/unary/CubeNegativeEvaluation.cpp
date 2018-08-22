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

#include "CubeNegativeEvaluation.h"
using namespace cube;
using namespace std;


NegativeEvaluation::NegativeEvaluation() : UnaryEvaluation()
{
}

NegativeEvaluation::NegativeEvaluation( GeneralEvaluation* _arg ) : UnaryEvaluation( _arg )
{
}


NegativeEvaluation::~NegativeEvaluation()
{
}

double
NegativeEvaluation::eval() const
{
    return -( arguments[ 0 ]->eval() );
}



double
NegativeEvaluation::eval( Cnode* _cnode, CalculationFlavour _cf, Sysres* _sys, CalculationFlavour _sf  ) const
{
    return -( arguments[ 0 ]->eval( _cnode, _cf, _sys, _sf ) );
}



double
NegativeEvaluation::eval( Cnode* _cnode, CalculationFlavour _cf ) const
{
    return -( arguments[ 0 ]->eval( _cnode, _cf ) );
}

double
NegativeEvaluation::eval( double arg1, double arg2 ) const
{
    return -( arguments[ 0 ]->eval( arg1, arg2 ) );
}



double*
NegativeEvaluation::eval_row( Cnode*             _cnode,
                              CalculationFlavour _cf ) const
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = -( result[ i ] );
    }
    return result;
}
