#ifndef EREAD_H
#define EREAD_H
#include <exodusII.h>
void read_exo     (char *s);
void read_head_exo(int exoid);
void read_coor_exo(int exoid);
void read_elem_exo(int exoid);
void error_exo    (int error , char *field);
#endif /* EREAD_H */


