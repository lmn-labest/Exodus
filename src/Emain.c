#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char **argv)
{
/*===*/
  int exoid;
/*===================================================================*/
/**/
/*=== parametros de entrada*/
  if( argc < 3){
    printf("numero insuficiente de parametros\n");
    exit(0);
  }
/*===================================================================*/  
/**/
  else if(argc == 3){
/*...*/    
    fprintf(stderr,"Read file %s\n",argv[1]);
    read_exo(argv[1]);
/*...................................................................*/
    ngeom           = 2;
    geom            = malloc(ngeom*sizeof(GEOM));
    geom[0].id      = 1;
    geom[0].type    = 1;
    geom[0].ndf     = 1;
    geom[0].f       = malloc(geom[0].ndf*sizeof(double));
    geom[0].idm     = malloc(geom[0].ndf*sizeof(int));
    geom[0].f[0]    = 100.0;                                  
    geom[0].idm[0]  = 1;

    geom[1].id      = 4;
    geom[1].type    = 2;
    geom[1].ndf     = 1;
    geom[1].f       = malloc(geom[1].ndf*sizeof(double));
    geom[1].f[0]    = 50.0;
    
    nodeset.tc = 1;
/**/
    fprintf(stderr,"Write file %s",argv[2]);
    write_mef(argv[2]);
/*...................................................................*/   
  }
  printf("\nnodes = %ld numel = %ld\n",nnode,nelem);
/*===================================================================*/
 
return 0;  
} 
