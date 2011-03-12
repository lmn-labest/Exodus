#ifndef EWRITE_H
#define EWRITE_H
#include <stdio.h>
#include<math.h>
#include<stdbool.h>
/*escrita para o mefpar*/
void write_mef       (char *);
void write_head_mef  (FILE *,char*,char*,char*,bool,bool);
void write_mef_coor  (FILE *,bool);
void write_mef_cell  (FILE *,bool);
void write_restricion(FILE *);
/*escrita para o gid*/
#include<gidpost.h>
void write_gid       (char *);

void tetraf(int);
#endif /* EWRITE_H */

