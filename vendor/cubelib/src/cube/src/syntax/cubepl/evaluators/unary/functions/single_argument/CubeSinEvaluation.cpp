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

#include "CubeSinEvaluation.h"
using namespace cube;
using namespace std;


SinEvaluation::SinEvaluation() : UnaryEvaluation()
{
};


SinEvaluation::~SinEvaluation()
{
}

double
SinEvaluation::eval() const
{
    return sin( arguments[ 0 ]->eval() );
}


double
SinEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   ) const
{
    return sin( arguments[ 0 ]->eval( _cnode, _cf,
                                      _sf, _tf
                                      ) );
}


double
SinEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf ) const
{
    return sin( arguments[ 0 ]->eval( _cnode, _cf
                                      ) );
}

double
SinEvaluation::eval( double arg1, double arg2 ) const
{
    return sin( arguments[ 0 ]->eval( arg1, arg2 ) );
}


double*
SinEvaluation::eval_row( Cnode*             _cnode,
                         CalculationFlavour _cf ) const
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }

    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = sin( result[ i ] );
    }
    return result;
}
