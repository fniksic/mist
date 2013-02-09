// vim:sw=4:ts=4
/*
    This file is part of mist.

    mist is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    mist is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with mist; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Copyright 2002, Anthony Piron
*/
#include <errno.h>		/* for definition of errno */
#include <stdarg.h>		/* ANSI C header file */
#include <string.h>
#include <stdlib.h>
#include "error.h"

static void err_doit(int, const char *, va_list);

char *pname = NULL;		/* caller can set this from argv[0] */

/* Nonfatal error related to a system call.  Print a message and return. */

void
err_ret(const char *fmt, ...)
{
  va_list ap;
  
  va_start(ap, fmt);
  err_doit(1, fmt, ap);
  va_end(ap);
  return;
}

/* Fatal error related to a system call.  Print a message and terminate. */

void
err_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, fmt, ap);
  va_end(ap);
  exit(1);
}

/* Fatal error related to a system call.  Print a message, dump core, and
 * terminate. */

void
err_dump(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, fmt, ap);
  va_end(ap);
  abort();		/* dump core and terminate */
  exit(1);		/* shouldn't get here */
}

/* Nonfatal error unrelated to a system call.  Print a message and return. */

void
err_msg(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, fmt, ap);
  va_end(ap);
  return;
}

/* Fatal error unrelated to a system call.  Print a message and terminate. */

void
err_quit(const char *fmt, ...)
{
  va_list ap;
  
  va_start(ap, fmt);
  err_doit(0, fmt, ap);
  va_end(ap);
  exit(1);
}

/* Print a message and return to caller.  Caller specifies "errnoflag". */

static void
err_doit(int errnoflag, const char *fmt, va_list ap)
{
  int errno_save;
  char buf[MAXLINE];
  
  errno_save = errno;		/* value caller might want printed */
  vsprintf(buf, fmt, ap);
  if (errnoflag)
    sprintf(buf+strlen(buf), ": %s", strerror(errno_save));
  strcat(buf, "\n");
  fflush(stdout);		/* in case stdout and stderr are the same */
  fputs(buf, stderr);
  fflush(NULL);		/* flushes all stdio output streams */
  return;
}
