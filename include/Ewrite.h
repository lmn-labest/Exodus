#ifndef EWRITE_H
#define EWRITE_H
#include <stdio.h>
#include<math.h>
#include<stdbool.h>
void write_mef       (char *);
void write_head_mef  (FILE *,char*,char*,char*,bool,bool);
void write_mef_coor  (FILE *,bool);
void write_mef_cell  (FILE *,bool);
void write_restricion(FILE *);
void tetraf(int);
#endif /* EWRITE_H */

