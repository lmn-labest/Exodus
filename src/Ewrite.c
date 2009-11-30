#define NAME "Ewrite.c"
#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void write_mef(char *s){
/*===*/
  FILE *fileout[3];
  char s1[200],s2[200],s3[200];
/*===================================================================*/
/**/
/*=== Manipulação do nome do arquivo*/  
  strcpy(s1,s);
  strcpy(s2,s);
  strcpy(s3,s);
  strcat(s1,".dat");
  strcat(s2,"_coor.dat");
  strcat(s3,"_elem.dat");
/*===================================================================*/
/**/  
/*=== arquivo principal*/ 
  fileout[0]=fopen(s1,"w");
  if (fileout==NULL){ 
    printf("Erro na abertura do arquivo:%s\n",s1);
    exit(1);
  } 
  printf("\nsucesso na abertura do arquivo:%s\n",s1);
/*...................................................................*/
/**/
/*...*/  
  write_head_mef(fileout[0],s2,s3);
  fclose(fileout[0]);
/*===================================================================*/
/**/
/*=== arquivo da coordenadas*/  
  fileout[1]=fopen(s2,"w");
  if (fileout==NULL){ 
    printf("Erro na abertura do arquivo:%s\n",s2);
    exit(1);
  } 
  printf("\nsucesso na abertura do arquivo:%s\n",s2);
  write_mef_coor(fileout[1]);
  fclose(fileout[1]);
/*===================================================================*/
/**/
/*=== arquivo das connectividades*/  
  fileout[2]=fopen(s3,"w");
  if (fileout==NULL){ 
    printf("Erro na abertura do arquivo:%s\n",s3);
    exit(1);
  } 
  printf("\nsucesso na abertura do arquivo:%s\n",s3);
  write_mef_cell(fileout[2]);
  fclose(fileout[2]);
/*===================================================================*/
}
/*********************************************************************/
/**/
/*********************************************************************
 * write_head_mef : escreve o arquivo principal no formato           *
 *                  do mef_par                                       *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 * file_coor - nome do arquivo de coordenadas                        *
 * file_elem - nome do arquivo de elementos                          *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_head_mef(FILE *f,char *file_coor,char *file_elem)
{
/*===*/  
  long i;
/*===================================================================*/  
/**/
/*===*/  
/*...*/
  fprintf(f,"mesh\n"
            "nnode %ld numel %ld ndf numat %ld maxno therm dim %d\n"
	    ,nnode,nelem,nbody,dim);
/*...................................................................*/
/**/
/*...*/  
  fprintf(f,"materials\n");
    for(i=0;i<nbody;i++)
      fprintf(f,"%ld\n",i+1);
  fprintf(f,"end materials\n");
/*...................................................................*/
/**/
/*...*/  
  fprintf(f,"insert %s\n",file_coor);
  fprintf(f,"insert %s\n",file_elem);
/*...................................................................*/
/**/
  fprintf(f,"end mesh\n");
/*===================================================================*/  
}
/*********************************************************************/
/**/
/*********************************************************************
 * write_head_coor : escreve o arquivo dos elementos                 *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_mef_coor(FILE *f)
{
/*===*/  
  long i;
/*===================================================================*/
/**/
/*===*/
  fprintf(stderr,"Escrevendo coordenadas...");
  fprintf(f,"coordinates\n");
/*...*/  
  switch(dim){
/*--- 1 dimencao*/
    case 1:
      printf("Não implementado\n");
      break;
/*-------------------------------------------------------------------*/
/**/
/*---2 dimencoes*/      
    case 2:
      for (i = 0 ; i < nnode ; i++)
        fprintf(f,"%ld %lf %lf",i+1, node[i].x , node[i].y);
      break;
/*-------------------------------------------------------------------*/
/**/
/*---3 dimencoes*/      
    case 3:
      for (i = 0 ; i < nnode ; i++)
        fprintf(f,"%ld %15.8lf %15.8lf %15.8lf\n", i+1 , node[i].x ,
	         node[i].y, node[i].z);
      break;
/*-------------------------------------------------------------------*/
/**/
/*---*/      
    default:
      printf("Numero de dimesoes invalido.\n"
	     "funcao write_mef_coor(FILE *f) arquivo = NAME\n");
      exit(0);
      break;
/*-------------------------------------------------------------------*/      
  }
/*...................................................................*/
  fprintf(stderr,"\ncoordenadas escritas.");
  fprintf(f,"end coordinates\nreturn\n");

/*===================================================================*/
}
/*********************************************************************/  
/**/
/*********************************************************************
 * write_head_cell: escreve o arquivo dos elementos                  *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_mef_cell(FILE *f){
/*===*/  
  short tp;
  long i;
/*=====================================================================*/
/**/  
/*===*/
  tp=elemt[0].type;
  fprintf(stderr,"\nEscrevendo Elementos...");
/*...*/
  switch(tp){
/*---*/    
    case 4:
      fprintf(stderr,"\nTetraedros");
      fprintf(f,"tetra4\n");
      for( i = 0 ; i < nelem ; i ++)
	fprintf(f,"%10ld %10ld %10ld %10ld %10ld %3d\n",i+1
	         ,elemt[i].node[0],elemt[i].node[1],elemt[i].node[3]
		 ,elemt[i].node[2],elemt[i].body);
      fprintf(f,"end tetra4");
      fprintf(stderr,"\nElementos escritos.\n");
      break;
/*-------------------------------------------------------------------*/  
/**/      
/*---*/    
    case 5:
      fprintf(stderr,"\nHexaedros");
      fprintf(f,"hexa8\n");
      for( i = 0 ; i < nelem ; i ++){
	fprintf(f,"%ld %ld %ld %ld " 
	          "%ld %ld %ld %ld %ld " 
	          "%d\n",i+1
	         ,elemt[i].node[0],elemt[i].node[4],elemt[i].node[7]
		 ,elemt[i].node[3],elemt[i].node[1],elemt[i].node[5]
		 ,elemt[i].node[6],elemt[i].node[2]
		 ,elemt[i].body);
	}
      fprintf(f,"end hexa8");
      fprintf(stderr,"\nElementos escritos.\n");
      break;
/*-------------------------------------------------------------------*/ 
/**/
/*---*/
    default:
      printf("Numero de elemento invalido.\n"
	     "funcao write_mef_cell(FILE *f) arquivo = NAME\n");
      exit(0);
      break;
/*-------------------------------------------------------------------*/
    }
  fprintf(f,"\nreturn\n");
/*...................................................................*/  
/*===================================================================*/  
}
/*********************************************************************/

