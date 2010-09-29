#ifndef EGLOBAL_H
#define EGLOBAL_H
#include<Eread.h>
#include<Ewrite.h>
#define NO_MAX_POR_ELM 8
#define MAX_ID 10
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
  int num;
  char gid[MAX_ID];
  int ngid;
}NODESET;

/*variaveis globais*/
long nnode,nelem,nbody,nnodeset,ngeomset;
int dim;
NODE *node;
ELEMT *elemt;
NODESET *nodeset;
#endif /* EGLOBAL_H */
