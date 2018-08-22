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


#ifndef __HALF_SMALLER_EVALUATION_H
#define __HALF_SMALLER_EVALUATION_H 0

#include "CubeBinaryEvaluation.h"

namespace cube
{
class HalfSmallerEvaluation : public BinaryEvaluation
{
public:
    HalfSmallerEvaluation();
    HalfSmallerEvaluation( GeneralEvaluation*,
                           GeneralEvaluation* );
    virtual
    ~HalfSmallerEvaluation();

    inline
    virtual
    double
    eval() const
    {
        return ( ( arguments[ 0 ]->eval() ) <= ( arguments[ 1 ]->eval() ) ) ? 1. : 0.;
    }

    inline
    virtual
    double
    eval( Cnode* _cnode, CalculationFlavour _cf, Sysres* _sf, CalculationFlavour _tf  ) const
    {
        return ( ( arguments[ 0 ]->eval( _cnode, _cf, _sf, _tf ) ) <= ( arguments[ 1 ]->eval( _cnode, _cf, _sf, _tf ) ) ) ? 1. : 0.;
    }

    inline
    virtual
    double
    eval( Cnode* _cnode, CalculationFlavour _cf ) const
    {
        return ( ( arguments[ 0 ]->eval( _cnode, _cf ) ) <= ( arguments[ 1 ]->eval( _cnode, _cf ) ) ) ? 1. : 0.;
    }


    inline
    virtual
    double*
    eval_row( Cnode*             _cnode,
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
                result[ i ] = ( result[ i ] >= 0. ) ? 1. : 0.;
            }
        }
        else
        if ( _result == NULL )
        {
            for ( size_t i = 0; i < row_size; i++ )
            {
                result[ i ] = ( result[ i ] <= 0. ) ? 1. : 0.;
            }
        }
        else
        {
            for ( size_t i = 0; i < row_size; i++ )
            {
                result[ i ] = ( result[ i ] <= _result[ i ] ) ? 1. : 0.;
            }
            delete _result;
        }
        return result;
    }

    inline
    virtual
    void
    print() const
    {
        arguments[ 0 ]->print();
        std::cout << "<=";
        arguments[ 1 ]->print();
    };

    inline
    virtual
    double
    eval( double arg1,
          double arg2 ) const
    {
        return ( ( arguments[ 0 ]->eval( arg1, arg2 ) ) <= ( arguments[ 1 ]->eval( arg1, arg2 ) ) ) ? 1. : 0.;
    }
};
};

#endif
