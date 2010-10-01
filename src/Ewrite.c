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
  int i,j,k,nset;
  int no;
  double x,y;
  double fy,fx,mf;
/*===================================================================*/  
/*...*/
/* teste do calculoda area cubo
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[2] || nodeset[i].gid[3]){
        fprintf(stderr,"\nno = %d\n",no);
	mf = modF(no);
        fprintf(stderr,"f = %lf\n",mf);
      }	
  }
  exit(0);
*/  
  fprintf(stderr,"\nEscrevendo restricion..");
  fprintf(stderr,"\nconstraintemp...");
  fprintf(f,"constraintemp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[2] || nodeset[i].gid[3])
        fprintf(f,"%10d %s\n",no,"1");
  }
  fprintf(f,"end constraintemp\n");
/*...*/  
  fprintf(stderr,"\nnodalsources...");
  fprintf(f,"nodalsources\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[2])
        fprintf(f,"%10d %20.8e\n",no,50.0);
      else if(nodeset[i].gid[3])
        fprintf(f,"%10d %20.8e\n",no,200.0);
  }
  fprintf(f,"end nodalsources\n");
/**/  
  fprintf(stderr,"\nintialtemp...");
  fprintf(f,"initialtemp\n");
  for(i=0;i<nnode;i++){
    fprintf(f,"%10d %20.8e\n",i+1,50.0);
  }  
  fprintf(f,"end initialtemp\n");
/*...................................................................*/
  fprintf(stderr,"\nconstraindisp...");
  fprintf(f,"constraindisp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if((nodeset[i].gid[0] || nodeset[i].gid[1]) 
        && !nodeset[i].gid[5])
        fprintf(f,"%10d %s\n",no,"0 0 1");
      if((nodeset[i].gid[2]) && !nodeset[i].gid[5])
        fprintf(f,"%10d %s\n",no,"1 1 0");
      if(nodeset[i].gid[5] )
        fprintf(f,"%10d %s\n",no,"1 1 1");
  }
  fprintf(f,"end constraindisp\n");
/*...................................................................*/
  fprintf(stderr,"\nnodalforces...");
  fprintf(f,"nodalforces\n");
   for(i=0;i<nnodeset;i++){
      if( nodeset[i].gid[4]){
        no = nodeset[i].num;
        x  = node[no].x;
        y = node[no].y;
	mf = modF(no);
//      fprintf(stderr,"\n%d %lf\n",no,mf);
  	fx = (x/sqrt(x*x + y*y)) * mf/(10000*10000);
  	fy = (y/sqrt(x*x + y*y)) * mf/(10000*10000);
//fx = (x/sqrt(x*x + y*y)) ;
//fy = (y/sqrt(x*x + y*y)) ;
        fprintf(f,"%10d %20.8e %20.8e %20.8e\n",no,fx,fy,0.0);
      }
      if( nodeset[i].gid[3] && !nodeset[i].gid[4]){
        no = nodeset[i].num;
        x  = node[no].x;
        y = node[no].y;
	mf = modF(no);
//      fprintf(stderr,"\n%d %lf\n",no,mf);
  	fx = (x/sqrt(x*x + y*y)) * mf/(10000*10000);
  	fy = (y/sqrt(x*x + y*y)) * mf/(10000*10000);
//fx = (x/sqrt(x*x + y*y)) ;
//fy = (y/sqrt(x*x + y*y)) ;
        fprintf(f,"%10d %20.8e %20.8e %20.8e\n",no,fx,fy,0.0);
      }	
  }
  fprintf(f,"end nodalforces\n");
  fprintf(f,"return\n");
  fprintf(stderr,"\nescrito restricion..");
/*===================================================================*/  
}
double modF(int no)
{
#define F 100000.0
  double f;
  int *face=NULL;
  int i,j,k,kk,l;
  int grade;
  int nel;
  int nfaces;
  int shno,idno;
  int T1[3],T2[3],T3[3],T4[3];
  double A[4],Aface[4];

  face = (int*) calloc(8*4,sizeof(int));
  kk = 0;
//  fprintf(stderr,"\nno=%d\n",no);
  grade=pnode.nincid[no-1];
//  fprintf(stderr,"grade=%d\n",grade);
/* loop no elementos q este no compartilha*/
  for(i=0;i<grade;i++){
    nel = pnode.incid[(no-1)*pnode.maxgrade+i];
  //  fprintf(stderr,"nel=%d\n",nel);
    for(j=0;j<maxno;j++){
      shno = elemt[nel-1].node[j];
    //  fprintf(stderr,"nel=%d no=%d\n",nel,shno);
/* verifica se esse no esta na superficie desejada*/
      for(k=0;k<nnodeset;k++){
        idno = nodeset[k].num;
	if( shno == idno ){
	  if( shno == idno ){
	    if(nodeset[k].gid[3]){
//	      fprintf(stderr,"idno=%d kk=%d\n",idno,kk);
              face[kk] = idno;
	      kk++;
	    }  
	  }  
	}  
      }	  
    }
  }
  
  nfaces = kk/4;
//printf("\n");
//for(i=0;i<nfaces*4;i++)
//  printf("%d face=%d\n",i+1,face[i]);
/**/
/*calculo da area das faces*/  
  for(i=0;i<nfaces;i++){
    maketri(&face[i*4],T1,T2,T3,T4);
//fprintf(stderr,"\n");
//fprintf(stderr,"T1=(%d %d %d)\n",T1[0],T1[1],T1[2]);
//fprintf(stderr,"T2=(%d %d %d)\n",T2[0],T2[1],T2[2]);
//fprintf(stderr,"T3=(%d %d %d)\n",T3[0],T3[1],T3[2]);
//fprintf(stderr,"T4=(%d %d %d)\n",T4[0],T4[1],T4[2]);
    A[0]=getarea(T1);
    A[1]=getarea(T2);
    A[2]=getarea(T3);
    A[3]=getarea(T4);
    Aface[i] = A[0] + A[1] + A[2] + A[3];
    Aface[i] = Aface[i]/2;
//    fprintf(stderr,"A[%d]=%lf\n",i+1,Aface[i]);
  }
/*Forca equivalente nodal*/  
  f =0.0;
  for(i=0;i<nfaces;i++){
    f += F*Aface[i];
  }
/**/    
  free(face);
  return f;
}

void maketri(int *face,int *T1, int *T2,int *T3,int *T4){

  T1[0] = T2[0] = T3[0] = T4[0] = face[0];
  T1[1] = T3[1] = T4[1]         = face[1];
  T2[2] = T3[2]                 = face[2];
  T1[2] = T4[2]                 = face[3];
  T2[1]                         = face[3];

}

double getarea(int*T){
  
  double aux1[3],aux2[3];
  double V1[3],V2[3],VxV[3];
  double D,A;
  int no;
/*T(a,b,c)*/  
/*V1= (b-a)*/
/*a*/
  no      = T[0] - 1;

  aux1[0] = node[no].x;
  aux1[1] = node[no].y;
  aux1[2] = node[no].z;
//fprintf(stderr,"no %d aux1 %lf %lf %lf\n",no+1,aux1[0],aux1[1],aux1[2]);
/*b*/
  no      = T[1] - 1;
  aux2[0] = node[no].x;
  aux2[1] = node[no].y;
  aux2[2] = node[no].z;
//  fprintf(stderr,"no %d aux2 %lf %lf %lf\n",no+1,aux2[0],aux2[1],aux2[2]);
/*b-a*/  
  subvetor(V1,aux2,aux1,3);
//fprintf(stderr,"V1 %lf %lf %lf\n",V1[0],V1[1],V1[2]);
/*V1= (c-a)*/
/*c*/
  no      = T[2] - 1;
  aux2[0] = node[no].x;
  aux2[1] = node[no].y;
  aux2[2] = node[no].z;
//  fprintf(stderr,"no %d aux2 %lf %lf %lf\n",no+1,aux2[0],aux2[1],aux2[2]);
/*c-a*/  
  subvetor(V2,aux2,aux1,3);
//fprintf(stderr,"V2 %lf %lf %lf\n",V2[0],V2[1],V2[2]);
/*produto vetoria*/
  prodvetorial(VxV,V1,V2); 
//fprintf(stderr,"V1XV2 %lf %lf %lf\n",VxV[0],VxV[1],VxV[2]);
/*produto escalar*/
  D = dot(VxV,VxV,3);
//fprintf(stderr,"dot=%lf\n",D);
  D = sqrt(D); 
  A = (1/2.0)*D;
//fprintf(stderr,"A=%lf\n",A);
  return A;
}

/*sub tracao de vetores*/
void  subvetor(double* V1,double* aux2,double* aux1,int n){
  
  int i;
  for(i=0;i<n;i++)
    V1[i]=aux2[i]-aux1[i];

}
/*produto vetorial*/
void  prodvetorial(double *V,double*x1,double*x2){
  
  V[0] = x1[1]*x2[2] - x1[2]*x2[1];
  V[1] = x1[2]*x2[0] - x1[0]*x2[2];
  V[2] = x1[0]*x2[1] - x2[0]*x1[1];

}

/*produto escalar*/
double dot(double *x,double *y,int n){
  
  int i;
  double tm;
  
  tm = 0.0;
  for(i=0;i<n;i++){
    tm += x[i]*y[i];
  }

  return tm;
}
