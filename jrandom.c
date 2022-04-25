/*
 * Copyright (c) 2022 2023
 *     John McCue <jmccue@jmcunx.com>
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/* Prints Random Numbers */

#define PROGNAME "jrandom"

#ifndef _MSDOS
#include <sys/param.h>
#endif

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#ifdef OpenBSD
#include <err.h>
#endif

#ifndef FILE_URANDOM
#define FILE_URANDOM "/dev/urandom"
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#define NUM unsigned long long
#define SCKARG 80
#define SWITCH_CHAR '-'

#define ARG_HELP          'h'  /* Show Help                          */
#define ARG_ITERATIONS    'I'  /* Iterations                         */
#define ARG_MAX_VALUE     'M'  /* Maximum Value                      */
#define ARG_MIN_VALUE     'L'  /* Low Value                          */
#define ARG_MOST          'm'  /* process at most                    */
#define ARG_SLEEP         's'  /* sleep seconds                      */
#define ARG_VERBOSE       'v'  /* Verbose                            */
#define ARG_VERSION       'V'  /* Show Version Information           */

#define LIT_INFO_01  "\tBuild: %s %s\n"
#define LIT_PROG     "Print one or more Random Number(s)"

#define MSG_ERR_E000    "Try '%s %c%c' for more information\n"
#define MSG_ERR_E004    "E004: value %s invalid value for %c%c\n"
#define MSG_ERR_E004L   "E004: value %llu invalid value for %c%c\n"
#define MSG_ERR_E006    "E006: '%s' is an invalid value for %c%c\n"
#define MSG_ERR_E056    "E056: Open Error on File '%s' : %s\n"
#define MSG_ERR_E074    "E074: 'Too many Arguments specified for %c%c\n"
#define MSG_ERR_E086    "E086: Value %s for Arg %c%c too big, must be less than %d digits\n"
#define MSG_ERR_E087    "E087: Value %s for Arg %c%c too large, must be less than %llu\n"
#define MSG_ERR_E088    "E088: Minimum Value %llu must be less than Maximum Value %llu\n"
#define MSG_ERR_E090    "E090: unveil() %d: %s\n"
#define MSG_ERR_E091    "E091: pledge() %d: %s\n"
#define MSG_INF_I098    "I098: Random Numbers Written  : %-llu\n"
#define MSG_INF_I099    "I099: Sleeping                : %-u second(s)\n"
#define MSG_INF_I100    "I100: Pause after printing    : %-u Random Numbers\n"
#define MSG_INF_I101    "I101: Minimum Value to Print  : %-llu\n"
#define MSG_INF_I102    "I102: Maximum Value to Print  : %-llu\n"
#define MSG_INF_I103    "I103: Number of Items to Print: %-u\n"
#define MSG_INF_I104    "I104: Pause Seconds           : %-u\n"
#define MSG_INF_I105    "I105: Verbose Level           : %-d\n"
#define MSG_INF_I035    "I035: Lines Written           :  %9ld\n"

#define USG_MSG_USAGE_1          "usage:\t%s [OPTIONS]\n"
#define USG_MSG_OPTIONS          "Options\n"
#define USG_MSG_ARG_VERBOSE_8    "\t%c%c\t\t: verbose level, each time specified level increases\n"
#define USG_MSG_ARG_HELP         "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_VERSION      "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_ARG_SLEEP        "\t%c%c s\t\t: Sleep 's' seconds between iterations.\n"
#define USG_MSG_ARG_MIN_VALUE    "\t%c%c n\t\t: Minumum Value\n"
#define USG_MSG_ARG_MAX_VALUE    "\t%c%c n\t\t: Maximum Value\n"
#define USG_MSG_ARG_ITERATIONS_1 "\t%c%c n\t\t: Pause after processing 'n' Objects\n"
#define USG_MSG_ARG_MOST         "\t%c%c n\t\t: Print no more than 'n' entries\n"

struct s_args
{
  int verbose;
  unsigned int iterations;
  unsigned int pause_secs;
  unsigned int print_most;
  NUM val_low;
  NUM val_high;
} ;

/*
 * is_numr() -- determines if all characters are numeric
 */
int is_numr(char *s)

{
  if (s == (char *) NULL)
    return(FALSE);

  for ( ; (*s) != (char) 0; s++)
    {
      if ( ! isdigit((int)(*s)) )
	return(FALSE);
    }

  return(TRUE);

} /* is_numr() */

/*
 * show_info()
 */
void show_info(FILE *fp, int rmode)
{

  switch (rmode)
    {
    case 2:
      fprintf(fp,"%s\n", PROGNAME);
#ifdef OSTYPE
      fprintf(fp,"\t%s\n",OSTYPE);
#endif
      fprintf(fp, LIT_INFO_01, __DATE__, __TIME__);
      break;
    default:
      fprintf(fp, USG_MSG_USAGE_1, PROGNAME);
      fprintf(fp, "\t%s\n", LIT_PROG);
      fprintf(fp, USG_MSG_OPTIONS);
      fprintf(fp, USG_MSG_ARG_HELP,         SWITCH_CHAR, ARG_HELP);
      fprintf(fp, USG_MSG_ARG_ITERATIONS_1, SWITCH_CHAR, ARG_ITERATIONS);
      fprintf(fp, USG_MSG_ARG_MIN_VALUE,    SWITCH_CHAR, ARG_MIN_VALUE);
      fprintf(fp, USG_MSG_ARG_MAX_VALUE,    SWITCH_CHAR, ARG_MAX_VALUE);
      fprintf(fp, USG_MSG_ARG_MOST,         SWITCH_CHAR, ARG_MOST);
      fprintf(fp, USG_MSG_ARG_SLEEP,        SWITCH_CHAR, ARG_SLEEP);
      fprintf(fp, USG_MSG_ARG_VERSION,      SWITCH_CHAR, ARG_VERSION);
      fprintf(fp, USG_MSG_ARG_VERBOSE_8,    SWITCH_CHAR, ARG_VERBOSE);
      break;
    }

  exit(EXIT_FAILURE);

} /* show_info() */

/*
 * get_ull() -- unsigned long long sometimes has conversion
 *              issues, so logic exists to get around that issue.
 */
NUM get_ull(char *argval, char arg, NUM current_val)
{
  double val = (double) 0;
  char cmax[2049];
  int msize = 0;

  memset(cmax, 0x00, 2049);

  if (current_val != (NUM) 0)
    {
      fprintf(stderr, MSG_ERR_E074, SWITCH_CHAR, arg);
      fprintf(stderr, MSG_ERR_E000, PROGNAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  if (is_numr(argval) != TRUE)
    {
      fprintf(stderr, MSG_ERR_E006, argval,   SWITCH_CHAR, arg);
      fprintf(stderr, MSG_ERR_E000, PROGNAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  val = atof(argval);
  snprintf(cmax, 2048, "%llu",ULLONG_MAX);
  msize = strlen(cmax) - 1;

  if (val < (double) 0)
    {
      fprintf(stderr, MSG_ERR_E004, argval, SWITCH_CHAR, arg);
      fprintf(stderr, MSG_ERR_E000,  PROGNAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
  if (strlen(argval) > msize)
    {
      fprintf(stderr, MSG_ERR_E086, argval, SWITCH_CHAR, arg, (msize +1));
      fprintf(stderr, MSG_ERR_E000,  PROGNAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
  if (val >= (double) (ULLONG_MAX - 1))
    {
      fprintf(stderr, MSG_ERR_E087, argval, SWITCH_CHAR, arg, (ULLONG_MAX - 1));
      fprintf(stderr, MSG_ERR_E000,  PROGNAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  return((NUM) val);

} /* get_ull() */

/*
 * process_arg() -- process arguments
 */
void process_arg(int argc, char **argv, struct s_args *args)

{
  char ckarg[SCKARG];
  int opt;
  NUM secs       = (NUM) 0;
  NUM min        = (NUM) 0;
  NUM max        = (NUM) 0;
  NUM iterations = (NUM) 0;
  NUM most       = (NUM) 0;

  args->verbose     = 0;
  args->iterations  = (unsigned int) 500;
  args->pause_secs  = (unsigned int) 1;
  args->print_most  = (unsigned int) 1;
  args->val_low     = (NUM) 0;
  args->val_high    = (NUM) 0;

  snprintf(ckarg, SCKARG, "%c%c%c%c:%c:%c:%c:%c:",
	   ARG_VERBOSE,    ARG_VERSION,   ARG_HELP,
	   ARG_SLEEP,      ARG_MIN_VALUE, ARG_MAX_VALUE,
	   ARG_ITERATIONS, ARG_MOST);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	case ARG_HELP:
	  show_info(stderr, 1);
	  break;
	case ARG_VERSION:
	  show_info(stderr, 2);
	  break;
	case ARG_VERBOSE:
	  args->verbose++;
	  break;
	case ARG_MIN_VALUE:
	  min = get_ull(optarg, ARG_MIN_VALUE, min);
	  break;
	case ARG_MAX_VALUE:
	  max = get_ull(optarg, ARG_MAX_VALUE, max);
	  break;
	case ARG_MOST:
	  most = get_ull(optarg, ARG_MOST, most);
	  break;
	case ARG_ITERATIONS:
	  iterations = get_ull(optarg, ARG_ITERATIONS, iterations);
	  break;
	case ARG_SLEEP:
	  secs = get_ull(optarg, ARG_SLEEP, secs);
	  break;
	default:
	  fprintf(stderr, MSG_ERR_E000, PROGNAME, SWITCH_CHAR, ARG_HELP);
	  exit(EXIT_FAILURE);
	  break;
	}
    }

  /* validate and save */
  if (secs >= UINT_MAX)
    {
      fprintf(stderr, MSG_ERR_E004L, secs,     SWITCH_CHAR, ARG_SLEEP);
      fprintf(stderr, MSG_ERR_E000,  PROGNAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
  if (iterations >= UINT_MAX)
    {
      fprintf(stderr, MSG_ERR_E004L, iterations, SWITCH_CHAR, ARG_ITERATIONS);
      fprintf(stderr, MSG_ERR_E000,  PROGNAME,   SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
  if (most >= UINT_MAX)
    {
      fprintf(stderr, MSG_ERR_E004L, most,       SWITCH_CHAR, ARG_MOST);
      fprintf(stderr, MSG_ERR_E000,  PROGNAME,   SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }
  if ((max != (NUM) 0) || (min != (NUM) 0))
    {
      if (min >= max)
	{
	  fprintf(stderr, MSG_ERR_E088, min, max);
	  fprintf(stderr, MSG_ERR_E000, PROGNAME,   SWITCH_CHAR, ARG_HELP);
	  exit(EXIT_FAILURE);
	}
    }
  if (secs > 0U)
    args->pause_secs  = (unsigned int) secs;
  if (iterations > 0U)
    args->iterations = (unsigned int) iterations;
  if (most > 0U)
    args->print_most = (unsigned int) most;
  if (min > 0LL)
    args->val_low = (NUM) min;
  if (max > 0LL)
    args->val_high = (NUM) max;

} /* END process_arg() */

/*
 * main
 */
int main(int argc, char **argv)
{
  int fd = 0;
  unsigned int most = 0U;
  unsigned int iterations = 0U;
  NUM writes    = (NUM) 0;
  NUM s = (NUM) 0;
  struct s_args args;

#ifdef OpenBSD
  if (unveil(FILE_URANDOM,"r") == -1)
    err(1, MSG_ERR_E090, 1, strerror(errno));
  if(pledge("stdio rpath",NULL) == -1)
    err(1, MSG_ERR_E091, 1, strerror(errno));
#endif

  process_arg(argc, argv, &args);

  fd = open(FILE_URANDOM, O_RDONLY);
  if (fd < 0)
    {
      fprintf(stderr, MSG_ERR_E056, FILE_URANDOM, strerror(errno));
      exit(EXIT_FAILURE);
    }

#ifdef OpenBSD
  if(pledge("stdio",NULL) == -1)
    err(1, MSG_ERR_E091, 2, strerror(errno));
#endif

  if (args.verbose > 1)
    {
      fprintf(stderr, MSG_INF_I100, args.iterations);
      fprintf(stderr, MSG_INF_I101, args.val_low);
      fprintf(stderr, MSG_INF_I102, args.val_high);
      fprintf(stderr, MSG_INF_I103, args.print_most);
      fprintf(stderr, MSG_INF_I104, args.pause_secs);
      fprintf(stderr, MSG_INF_I105, args.verbose);
    }

  while (most < args.print_most)
    {
      most++;
      iterations++;
      if ((args.pause_secs > 0U) && (iterations > args.iterations))
	{
	  if (args.verbose > 2)
	    fprintf(stderr, MSG_INF_I099, args.pause_secs);
	  iterations = 1U;
	  sleep(args.pause_secs);
	}
      read(fd, &s, sizeof(NUM));
      if (args.val_low != args.val_high)
	s = s % (args.val_high - args.val_low + (NUM) 1) + args.val_low;
      fprintf(stdout, "%llu\n", s); fflush(stdout);
      writes++;
    }

  if (args.verbose > 0)
    {
      fprintf(stderr, MSG_INF_I098, writes);
    }

  close(fd);

} /* main() */
