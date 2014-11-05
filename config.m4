dnl $Id$
dnl config.m4 for extension crontab

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(crontab, for crontab support,
dnl Make sure that the comment is aligned:
dnl [  --with-crontab             Include crontab support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(crontab, whether to enable crontab support,
[  --enable-crontab           Enable crontab support])

if test "$PHP_CRONTAB" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-crontab -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/crontab.h"  # you most likely want to change this
  dnl if test -r $PHP_CRONTAB/$SEARCH_FOR; then # path given as parameter
  dnl   CRONTAB_DIR=$PHP_CRONTAB
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for crontab files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CRONTAB_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CRONTAB_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the crontab distribution])
  dnl fi

  dnl # --with-crontab -> add include path
  dnl PHP_ADD_INCLUDE($CRONTAB_DIR/include)

  dnl # --with-crontab -> check for lib and symbol presence
  dnl LIBNAME=crontab # you may want to change this
  dnl LIBSYMBOL=crontab # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CRONTAB_DIR/lib, CRONTAB_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CRONTABLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong crontab lib version or lib not found])
  dnl ],[
  dnl   -L$CRONTAB_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(CRONTAB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(crontab, crontab.c, $ext_shared)
fi
