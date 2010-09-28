#ifndef EWRITE_H
#define EWRITE_H
#include <stdio.h>
#include<math.h>
void write_mef       (char *);
void write_head_mef  (FILE *,char*,char*,char*);
void write_mef_coor  (FILE *);
void write_mef_cell  (FILE *);
void write_restricion(FILE *);
void insertionSort(int *,int *, int );
void removenode(int *,int *, int );
#endif /* EWRITE_H */

