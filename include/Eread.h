#ifndef EREAD_H
#define EREAD_H
#include <exodusII.h>
void read_exo      (char *                 );
void read_head_exo (int                    );
void read_coor_exo (int                    );
void read_elem_exo (int                    );
void read_carg_exo (int                    );
void error_exo     (int  ,char *           );
void contnodeset   (int *,char *,long*,int );
void insertionsort (int *,int * ,int       );
#endif /* EREAD_H */


