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
/**/
    fprintf(stderr,"Write file %s",argv[2]);
    write_mef(argv[2]);
/*...................................................................*/   
  }
  printf("\nnodes = %ld numel = %ld\n",nnode,nelem);
/*===================================================================*/
 
return 0;  
} 
