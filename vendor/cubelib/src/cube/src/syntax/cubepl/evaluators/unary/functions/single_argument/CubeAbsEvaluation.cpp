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

#include "CubeAbsEvaluation.h"
using namespace cube;
using namespace std;


AbsEvaluation::AbsEvaluation() : UnaryEvaluation()
{
};


AbsEvaluation::~AbsEvaluation()
{
}

double
AbsEvaluation::eval() const
{
    return fabs( arguments[ 0 ]->eval() );
}




double
AbsEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   ) const
{
    return fabs( arguments[ 0 ]->eval(        _cnode, _cf,
                                              _sf, _tf
                                              ) );
}


double
AbsEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf ) const
{
    return fabs( arguments[ 0 ]->eval(        _cnode, _cf
                                              ) );
}

double
AbsEvaluation::eval( double arg1, double arg2 ) const
{
    return fabs( arguments[ 0 ]->eval( arg1, arg2  ) );
}



double*
AbsEvaluation::eval_row( Cnode*             _cnode,
                         CalculationFlavour _cf ) const
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = fabs( result[ i ] );
    }
    return result;
}
