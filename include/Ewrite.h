#ifndef EWRITE_H
#define EWRITE_H
#include <stdio.h>
void write_mef      (char *s);
void write_head_mef (FILE *f,char *file_coor,char *file_elem);
void write_mef_coor (FILE *f);
void write_mef_cell (FILE *f);
#endif /* EWRITE_H */

