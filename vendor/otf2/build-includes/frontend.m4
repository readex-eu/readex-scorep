## -*- mode: autoconf -*-

dnl
dnl This file is part of the Score-P software (http://www.score-p.org)
dnl
dnl Copyright (c) 2015, 2017,
dnl Technische Universitaet Dresden, Germany
dnl
dnl This software may be modified and distributed under the terms of
dnl a BSD-style license.  See the COPYING file in the package base
dnl directory for details.
dnl

AC_ARG_VAR([PYTHON],
           [The Python interpreter to be used for the Python bindings. Use PYTHON=: to disable Python support.])
AM_PATH_PYTHON([2.5], , [:])

rm -f python_bindings_supported
otf2_have_python_bindings_support=yes
otf2_python_bindings_reason=", using $PYTHON"

AS_IF([test "x${PYTHON}" = "x:"], [
    otf2_have_python_bindings_support=no
    otf2_python_bindings_reason=", missing python"
])

AS_IF([test "x${otf2_have_python_bindings_support}" = "xyes" && test "x${enable_shared}" != "xyes"], [
    otf2_have_python_bindings_support=no
    otf2_python_bindings_reason=", missing shared library support"
])

AS_IF([test "x${otf2_have_python_bindings_support}" = "xyes"], [
    AX_PYTHON_MODULE([six])
    AS_IF([test "x${HAVE_PYMOD_SIX}" = "xyes"], [
        AS_CASE(["${HAVE_PYMOD_VERSION_SIX}"],
        [[1.[456789].*|1.1?.*|2.*]], [
            :
        ],
        [*], [
            otf2_have_python_bindings_support=no
            otf2_python_bindings_reason=", version requirement of six failed: ${HAVE_PYMOD_VERSION_SIX} < 1.4.0"
        ])
    ], [
        otf2_have_python_bindings_support=no
        otf2_python_bindings_reason=", missing six module"
    ])
])

AS_IF([test "x${otf2_have_python_bindings_support}" = "xyes"], [
    # Note: builtins is from python-future a.k.a. future, not to be confused with __builtins__ or __future__
    AX_PYTHON_MODULE([builtins])
    AS_IF([test "x${HAVE_PYMOD_BUILTINS}" != "xyes"], [
        otf2_have_python_bindings_support=no
        otf2_python_bindings_reason=", missing builtins module"
    ])
])

AS_IF([test "x${otf2_have_python_bindings_support}" = "xyes"], [
    touch python_bindings_supported
])
AFS_AM_CONDITIONAL([HAVE_PYTHON_BINDINGS_SUPPORT],
                   [test "x${otf2_have_python_bindings_support}" = "xyes"],
                   [false])
AFS_SUMMARY([Python bindings support], [${otf2_have_python_bindings_support}${otf2_python_bindings_reason}])
