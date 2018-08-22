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


#include <cmath>

#include "CubeTanEvaluation.h"
using namespace cube;
using namespace std;


TanEvaluation::TanEvaluation() : UnaryEvaluation()
{
};


TanEvaluation::~TanEvaluation()
{
}

double
TanEvaluation::eval() const
{
    return tan( arguments[ 0 ]->eval() );
}




double
TanEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   ) const
{
    return tan( arguments[ 0 ]->eval( _cnode, _cf,
                                      _sf, _tf
                                      ) );
}




double
TanEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf ) const
{
    return tan( arguments[ 0 ]->eval( _cnode, _cf
                                      ) );
}


double
TanEvaluation::eval( double arg1, double arg2 ) const
{
    return tan( arguments[ 0 ]->eval( arg1, arg2 ) );
}


double*
TanEvaluation::eval_row( Cnode*             _cnode,
                         CalculationFlavour _cf ) const
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }

    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = tan( result[ i ] );
    }
    return result;
}
