#ifndef EREAD_H
#define EREAD_H
#include <exodusII.h>
void read_exo(char *file_name);
void read_head_exo(int exoid);
void read_coor_exo(int exoid);
void read_elem_exo(int exoid);
void read_carg_node_exo(int exoid);
void read_carg_side_exo(int exoid);
void error_exo(int error , char *field);
void contnodeset(int *v,char*nv,long *n,int num);
void insertionsort2Vector(int *v,int *b,int n);
void insertionsort3Vector(int *v,int *b1,int *b2,int n);

#endif /* EREAD_H */


