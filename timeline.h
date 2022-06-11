#ifndef TIMELINE_H
#define TIMELINE_H

typedef struct timeline {
  int inited1;
  int inited2;
  char *prev_project;
  double prev_time;
  double prev_from;
  char *csv;
} timeline;

int timeline_add(timeline *t, double time, char *project);
int timeline_finish(timeline *t);

#endif
