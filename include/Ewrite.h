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


void write_mvf     (char *s);
void write_mvf_coor(FILE *file);
void write_mvf_cell(FILE *file);
void write_mvf_res(FILE *file);

/*escrita para o gid*/
#include<gidpost.h>
void write_gid       (char *);

#endif /* EWRITE_H */

