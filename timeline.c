#include "timeline.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// copied from https://github.com/jwerle/asprintf.c/blob/master/asprintf.c
static int vafmt(char **str, const char *fmt, va_list args) {
  int size = 0;
  va_list tmpa;

  // copy
  va_copy(tmpa, args);

  // apply variadic arguments to
  // sprintf with format to get size
  size = vsnprintf(NULL, 0, fmt, tmpa);

  // toss args
  va_end(tmpa);

  // return -1 to be compliant if
  // size is less than 0
  if (size < 0) {
    return -1;
  }

  // alloc with size plus 1 for `\0'
  *str = (char *)malloc(size + 1);

  // return -1 to be compliant
  // if pointer is `NULL'
  if (NULL == *str) {
    return -1;
  }

  // format string with original
  // variadic arguments and set new size
  size = vsprintf(*str, fmt, args);
  return size;
}

static int fmt(char **str, const char *fmt, ...) {
  int size = 0;
  va_list args;

  // init variadic argumens
  va_start(args, fmt);

  // format and get size
  size = vafmt(str, fmt, args);

  // toss args
  va_end(args);

  return size;
}

static int timeline_csv_append(timeline *t, double time) {
  char *row;

  int from = t->prev_from;
  int to = time;
  int err = fmt(&row, "%s,%d,%d\n", t->prev_project, from, to);
  if (err < 0) return err;

  if (t->inited2 != 1) {
    t->csv = row;
    t->inited2 = 1;
  } else {
    t->csv = realloc(t->csv, strlen(t->csv) + strlen(row) + 1);
    strcat(t->csv, row);
    free(row);
  }

  return 0;
}

int timeline_add(timeline *t, double time, char *project) {
  if (t->inited1 != 1) {
    t->prev_time = time;
    t->prev_from = time;
    t->prev_project = project;
    t->inited1 = 1;
    return 0;
  }

  double diff = time - t->prev_time;

  if (diff < 300) {
    if (strcmp(t->prev_project, project) != 0) {
      int err = timeline_csv_append(t, time);
      if (err < 0) return err;
      t->prev_from = time;
    }
  } else {
    int err = timeline_csv_append(t, t->prev_time);
    if (err < 0) return err;
    t->prev_from = time;
  }

  t->prev_time = time;
  t->prev_project = project;
  return 0;
}

int timeline_finish(timeline *t) {
  if (t->prev_project != NULL) {
    int err = timeline_csv_append(t, t->prev_time);
    if (err < 0) return err;
  }

  return 0;
}
