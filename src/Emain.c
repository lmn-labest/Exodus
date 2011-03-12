#include<Eglobal.h>
#include<stdio.h>
int main(int argc, char **argv)
{
/*===*/
  int exoid;
/*===================================================================*/
/**/
/*=== parametros de entrada*/
  if( argc < 4){
    printf("numero insuficiente de parametros\n");
    printf("erad: <input exodus file> <software"
            " output> <prefixo name output>\n"
	    "-g - gid file\n"
	    "-m - mefpar file\n");
    exit(EXIT_FAILURE);
  }
/*===================================================================*/  
/**/
  else if(argc == 4){
/*...*/    
    fprintf(stderr,"Read file %s\n",argv[1]);
    read_exo(argv[1]);
/*...................................................................*/
/**/
    if(!strcmp(argv[2],"-m")){ 
      fprintf(stderr,"Write file mefpar %s",argv[3]);
      write_mef(argv[3]);
    }  
    else if(!strcmp(argv[2],"-g")){ 
      fprintf(stderr,"Write file gid %s",argv[3]);
      write_gid(argv[3]);
    }  
/*...................................................................*/   
  }
  printf("\nnodes = %ld numel = %ld\n",nnode,nelem);
/*===================================================================*/
 
return 0;  
} 
