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

typedef struct NODESET{
  int *node;
  int *inode;
  int *nset;
  int  total;
  int *g;
  /* 1 - constraintemp */  
  /* 2 - ndalsources   */  
  /* 3 - constraindisp */  
  /* 4 - nodalforces   */
  char tc;
  char dc;
  char fr;
  char sr;
}NODESET;

/*variaveis globais*/
long nnode,nelem,nbody,nnodeset;
int dim;
NODE *node;
ELEMT *elemt;
NODESET nodeset;
#endif /* EGLOBAL_H */
