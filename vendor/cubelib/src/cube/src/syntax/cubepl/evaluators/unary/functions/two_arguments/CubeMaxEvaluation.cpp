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
#include <algorithm>

#include "CubeMaxEvaluation.h"
using namespace cube;
using namespace std;


MaxEvaluation::MaxEvaluation() : UnaryEvaluation()
{
};


MaxEvaluation::~MaxEvaluation()
{
}

double
MaxEvaluation::eval() const
{
    return std::max( arguments[ 0 ]->eval(), arguments[ 1 ]->eval() );
}


double
MaxEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   ) const
{
    return std::max( arguments[ 0 ]->eval(    _cnode, _cf,
                                              _sf, _tf
                                              ),
                     arguments[ 1 ]->eval( _cnode, _cf,
                                           _sf, _tf
                                           )
                     );
}

double
MaxEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf ) const
{
    return std::max( arguments[ 0 ]->eval(    _cnode, _cf
                                              ),
                     arguments[ 1 ]->eval(     _cnode, _cf
                                               )
                     );
}

double
MaxEvaluation::eval( double arg1, double arg2 ) const
{
    return std::max( arguments[ 0 ]->eval( arg1, arg2 ),
                     arguments[ 1 ]->eval( arg1, arg2 )
                     );
}

double*
MaxEvaluation::eval_row( Cnode*             _cnode,
                         CalculationFlavour _cf ) const
{
    double* result  = arguments[ 0 ]->eval_row( _cnode, _cf );
    double* _result = arguments[ 1 ]->eval_row( _cnode, _cf );
    if ( result == NULL && _result == NULL )
    {
        return NULL;
    }
    if ( result == NULL )
    {
        result = _result;
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] = std::max( result[ i ],  0.  );
        }
    }
    else
    if ( _result == NULL )
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] = std::max( result[ i ], 0.  );
        }
    }
    else
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] = std::max( result[ i ],  _result[ i ] );
        }
        delete[] _result;
    }
    return result;
}
