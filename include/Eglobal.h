#ifndef EGLOBAL_H
#define EGLOBAL_H
#include<stdlib.h>
#include<Eread.h>
#include<Ewrite.h>
#define NO_MAX_POR_ELM 8
#define MAX_ID 10

/*...*/
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
/*...................................................................*/

/*...*/
typedef struct NODESET{
  int num;
  char gid[MAX_ID];
  int ngid;
}NODESET;

/*...*/
typedef struct Pnode{
  int *nincid;
  int *incid;
  int maxgrade;
}Pnode;

void propnode_malloc_geral(int*,int*,int*,long,long,int,int*);
void mynodegrade(int*,long,long,int,int*,int*);
void myelmnicid (int*,int*,int*,long,long,int,int);
/*...................................................................*/

/*...*/
void maketri(int *,int *, int *,int *,int *);
double getarea(int*,double *);
void  subvetor(double*,double*,double*,int);
void  prodvetorial(double*,double*,double*);
double dot(double*,double*,int);
double modF(int,double*);
/*...................................................................*/

/*variaveis globais*/
long nnode,nelem,nbody,nnodeset,ngeomset;
int maxno;
int dim;
NODE *node;
ELEMT *elemt;
NODESET *nodeset;
Pnode pnode;
#endif /* EGLOBAL_H */
