## -*- mode: autoconf -*-

##
## This file is part of the Score-P software (http://www.score-p.org)
##
## Copyright (c) 2009-2012,
## RWTH Aachen University, Germany
##
## Copyright (c) 2009-2012,
## Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
##
## Copyright (c) 2009-2012,
## Technische Universitaet Dresden, Germany
##
## Copyright (c) 2009-2012,
## University of Oregon, Eugene, USA
##
## Copyright (c) 2009-2013,
## Forschungszentrum Juelich GmbH, Germany
##
## Copyright (c) 2009-2012, 2014
## German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
##
## Copyright (c) 2009-2012,
## Technische Universitaet Muenchen, Germany
##
## This software may be modified and distributed under the terms of
## a BSD-style license.  See the COPYING file in the package base
## directory for details.
##

## file build-config/m4/ac_scorep_compiler_checks.m4

dnl ------------------------------------------------------------------

AC_DEFUN([AC_SCOREP_COMPILER_CHECKS],[
AC_LANG_PUSH([C])
AX_COMPILER_VENDOR
AC_LANG_POP([C])

# Disable default OpenMP support for the Cray compilers
AS_IF([test "x${ax_cv_c_compiler_vendor}" = xcray],
    [CC="${CC} -hnoomp -O2"
     CXX="${CXX} -hnoomp -O2"
     F77="${F77} -hnoomp -O2"
     FC="${FC} -hnoomp -O2"])dnl

AS_CASE([${ax_cv_c_compiler_vendor}],
    [intel],    [AFS_AM_CONDITIONAL([SCOREP_COMPILER_INTEL],   [test 1 -eq 1], [false])],
    [sun],      [AFS_AM_CONDITIONAL([SCOREP_COMPILER_SUN],     [test 1 -eq 1], [false])],
    [ibm],      [AFS_AM_CONDITIONAL([SCOREP_COMPILER_IBM],     [test 1 -eq 1], [false])],
    [portland], [AFS_AM_CONDITIONAL([SCOREP_COMPILER_PGI],     [test 1 -eq 1], [false])],
    [gnu],      [AFS_AM_CONDITIONAL([SCOREP_COMPILER_GNU],     [test 1 -eq 1], [false])],
    [cray],     [AFS_AM_CONDITIONAL([SCOREP_COMPILER_CRAY],    [test 1 -eq 1], [false])],
    [fujitsu],  [AFS_AM_CONDITIONAL([SCOREP_COMPILER_FUJITSU], [test 1 -eq 1], [false])],
    [open64],   [AFS_AM_CONDITIONAL([SCOREP_COMPILER_OPEN64],  [test 1 -eq 1], [false])],
    [clang],    [AFS_AM_CONDITIONAL([SCOREP_COMPILER_CLANG],   [test 1 -eq 1], [false])],
    [unknown],  [AC_MSG_WARN([Could not determine compiler vendor. AC_PACKAGE_NAME might not function properly.])],
    [AC_MSG_WARN([Compiler vendor '${ax_cv_c_compiler_vendor}' unsupported. AC_PACKAGE_NAME might not function properly.])])dnl

afs_compiler_intel=0
afs_compiler_sun=0
afs_compiler_ibm=0
afs_compiler_portland=0
afs_compiler_gnu=0
afs_compiler_cray=0
afs_compiler_fujitsu=0
afs_compiler_open64=0
afs_compipler_clang=0
AS_CASE([${ax_cv_c_compiler_vendor}],
    [intel],    [afs_compiler_intel=1],
    [sun],      [afs_compiler_sun=1],
    [ibm],      [afs_compiler_ibm=1],
    [portland], [afs_compiler_portland=1],
    [gnu],      [afs_compiler_gnu=1],
    [cray],     [afs_compiler_cray=1],
    [fujitsu],  [afs_compiler_fujitsu=1],
    [open64],   [afs_compiler_open64=1],
    [clang],    [afs_compiler_clang=1],
    [])

AC_SUBST([SCOREP_COMPILER_INTEL],   [${afs_compiler_intel}])dnl
AC_SUBST([SCOREP_COMPILER_SUN],     [${afs_compiler_sun}])dnl
AC_SUBST([SCOREP_COMPILER_IBM],     [${afs_compiler_ibm}])dnl
AC_SUBST([SCOREP_COMPILER_PGI],     [${afs_compiler_portland}])dnl
AC_SUBST([SCOREP_COMPILER_GNU],     [${afs_compiler_gnu}])dnl
AC_SUBST([SCOREP_COMPILER_CRAY],    [${afs_compiler_cray}])dnl
AC_SUBST([SCOREP_COMPILER_FUJITSU], [${afs_compiler_fujitsu}])dnl
AC_SUBST([SCOREP_COMPILER_OPEN64],  [${afs_compiler_open64}])dnl
AC_SUBST([SCOREP_COMPILER_CLANG],   [${afs_compiler_clang}])dnl
])dnl

