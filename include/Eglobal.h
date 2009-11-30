#ifndef EGLOBAL_H
#define EGLOBAL_H
#include<Eread.h>
#include<Ewrite.h>
#define NO_MAX_POR_ELM 8
typedef struct NODE{
  double x;
  double y;
  double z;
}NODE;
typedef struct ELEMT{
  int nen;
  long node[NO_MAX_POR_ELM];
  int type;
  int body;
}ELEMT;

/*variaveis globais*/
long nnode,nelem,nbody;
int dim;
NODE *node;
ELEMT *elemt; 
#endif /* EGLOBAL_H */
