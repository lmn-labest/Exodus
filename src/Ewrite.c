#define NAME "Ewrite.c"
#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void write_mef(char *ins){
/*===*/
  FILE *fileout[3];
  char s[4][200];
/*===================================================================*/
/**/
/*=== Manipulação do nome do arquivo*/  
  strcpy(s[0],ins);
  strcpy(s[1],ins);
  strcpy(s[2],ins);
  strcpy(s[3],ins);
  strcat(s[0],".dat");
  strcat(s[1],"_coor.dat");
  strcat(s[2],"_elem.dat");
  strcat(s[3],"_restricion.dat");
/*===================================================================*/
/**/  
/*=== arquivo principal*/ 
  fileout[0]=fopen(s[0],"w");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo:%s\n",s[0]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo:%s",s[0]);
/*...................................................................*/
/**/
/*...*/
  write_head_mef(fileout[0],s[1],s[2],s[3]);
  fclose(fileout[0]);
/*===================================================================*/
/**/
/*=== arquivo da coordenadas*/  
  fileout[1]=fopen(s[1],"w");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo:%s\n",s[1]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo:%s",s[1]);
  write_mef_coor(fileout[1]);
  fclose(fileout[1]);
/*===================================================================*/
/**/
/*=== arquivo das connectividades*/  
  fileout[2]=fopen(s[2],"w");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo:%s\n",s[2]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo:%s",s[2]);
  write_mef_cell(fileout[2]);
  fclose(fileout[2]);
/*===================================================================*/
/**/
/*=== arquivo das restricoes*/  
  fileout[3]=fopen(s[3],"w");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo:%s\n",s[3]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo:%s",s[3]);
  write_restricion(fileout[3]);
  fclose(fileout[3]);
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
void write_head_mef(FILE *f,char *file1,char *file2,char *file3)
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
  fprintf(f,"insert %s\n",file1);
  fprintf(f,"insert %s\n",file2);
  fprintf(f,"insert %s\n",file3);
/*...................................................................*/
/**/
  fprintf(f,"end mesh\n");
  fprintf(f,"stop\n");
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
  fprintf(stderr,"\nEscrevendo coordenadas...");
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
/*gambiarra para o cilindro*/	
        fprintf(f,"%ld %15.8lf %15.8lf %15.8lf\n", i+1 , node[i].x/10000 ,
	         node[i].y/10000, node[i].z/10000);
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
	fprintf(f,"%10ld %10ld %10ld %10ld " 
	          "%10ld %10ld %10ld %10ld %10ld " 
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
/*********************************************************************
 * GAMBIARRA PARA O POCO:                                            *
 *                                                                   *
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
void write_restricion(FILE *f)
{
/*===*/
#define F 2781.581653
  int i,j,k,nset;
  int type,*id,*b;
  int *aux,*ids;
  double x,y;
  double fy,fx;
/*===================================================================*/  
/*...*/
  fprintf(stderr,"\nEscrevendo restricion..");
  insertionSort(nodeset.node,nodeset.id,nodeset.total);
/*  removenode(nodeset.node,nodeset.id,nodeset.total);*/
  fprintf(stderr,"\nconstraintemp...");
  fprintf(f,"constraintemp\n");
    for(i=0;i<nodeset.total;i++){
      if( nodeset.id[i] == 3 || nodeset.id[i] == 4)  
        fprintf(f,"%10d %2d\n",nodeset.node[i],1);
    }
  fprintf(f,"end constraintemp\n");
/*...*/  
  fprintf(stderr,"\nnodalsources...");
  fprintf(f,"nodalsources\n");
    for(i=0;i<nodeset.total;i++){
      if( nodeset.id[i] == 3)  
        fprintf(f,"%10d %lf\n",nodeset.node[i], 50.0);
      if( nodeset.id[i] == 4)  
        fprintf(f,"%10d %lf\n",nodeset.node[i],200.0);
    }
  fprintf(f,"end nodalsources\n");
  fprintf(stderr,"\nconstraindisp...");
  b = (int*) calloc(nodeset.total,sizeof(int));
  removenode(nodeset.node,b,nodeset.total);
  fprintf(f,"constraindisp\n");
    for(i=0;i<nodeset.total;i++){
      j = nodeset.node[i];
      if( nodeset.id[i] == 1 || nodeset.id[i] == 2){
	if(!b[i])
	  fprintf(f,"%10d %s\n",j,"0 0 1");
      }	  
      if( nodeset.id[i] == 3) { 
	if(!b[i])
         fprintf(f,"%10d %s\n",j,"1 1 0");
      }
      if(b[i] == -1 && nodeset.id[i] == 6 ){
        fprintf(f,"%10d %s\n",j,"1 1 1");
      }
      if( nodeset.id[i] == 5) {
          fprintf(f,"%10d %s\n",j,"0 0 1");
      }	
    }
  fprintf(f,"end constraindisp\n");
  fprintf(stderr,"\nnodalforces...");
  fprintf(f,"nodalforces\n");
    for(i=0;i<nodeset.total;i++){
      if( nodeset.id[i] == 5) {
	  j = nodeset.node[i];
	  x = node[j].x;
	  y = node[j].y; 
	  fx = (x/sqrt(x*x + y*y)) * 2 * F;
	  fy = (y/sqrt(x*x + y*y)) * 2 * F;
          fprintf(f,"%10d %20.8e %20.8e %20.8e\n",j,fx,fy,0.0);
      }	
      if( nodeset.id[i] == 4 && !b[i]  ) {
	  j = nodeset.node[i];
	  x = node[j].x;
	  y = node[j].y; 
	  fx = (x/sqrt(x*x + y*y)) * 4 * F;
	  fy = (y/sqrt(x*x + y*y)) * 4 * F;
          fprintf(f,"%10d %20.8e %20.8e %20.8e\n",j,fx,fy,0.0);
      }	
    } 
  fprintf(f,"end nodalforces\n");
  fprintf(stderr,"\nintialtemp...");
  fprintf(f,"initialtemp\n");
    for(j=1;j<=nnode;j++){
//    k =1;
//    for(i=0;i<nodeset.total;i++){
//     if(j==nodeset.node[i]){
//        if( nodeset.id[i] == 4) {
//	  k = 0;
//	  fprintf(f,"%10d %20.8e\n",j,200.0);
//	  break;
//	}
//      }
//    }
//    if(k)
        fprintf(f,"%10d %20.8e\n",j,50.0);
  }  
  fprintf(f,"end initialtemp\n");
/*...................................................................*/
  fprintf(f,"return\n");
  fprintf(stderr,"\nescrito restricion..");
/*===================================================================*/  
}

void insertionSort(int *v,int *b,int n)
{
 int i, j, chave,chave1;
 
 for(j=1; j<n; j++) 
 {
   chave  = v[j];
   chave1 = b[j];
   i = j-1;
   while(i >= 0 && v[i] > chave)
   {
     v[i+1] = v[i];
     b[i+1] = b[i];
     i--;
   }		
   v[i+1] = chave;
   b[i+1] = chave1;
 }  
}

void removenode(int *v,int *b, int n)
{
  int i,j;
  int aux1,aux2;

  for(i=0;i<n;i++){
    aux1=v[i];
    for(j=i+1;j<n;j++){
       aux2=v[j];
       if(aux2 == aux1){
	 b[j]   = -1;
         b[j-1] = -1;
       }	 
       else
	 break;
    }  
  }  
}
