#define NAME "Ewrite.c"
#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static void write_double(double,bool,FILE*);
static void force_big_endian(unsigned char *,bool,int);
static void write_int(int,bool,FILE*);
static void new_section(bool cod,FILE *f);

/* funcao de apoio*/
double distno(int,int);

void write_mef(char *ins){
/*===*/
  FILE *fileout[3];
  char s[4][200];
  bool coorbin,elmbin;
/*===================================================================*/
/**/
/*=== arquivos binarias auxiliares*/  
  coorbin = false;
  elmbin  = false;
/*===================================================================*/
/**/
/*=== Manipulação do nome do arquivo*/  
  strcpy(s[0],ins);
  strcpy(s[1],ins);
  strcpy(s[2],ins);
  strcpy(s[3],ins);
  strcat(s[0],".dat");
  
  if(coorbin)
    strcat(s[1],"_coor_bin.dat");
  else  
    strcat(s[1],"_coor.dat");
  
  if(elmbin)
    strcat(s[2],"_elem_bin.dat");
  else
    strcat(s[2],"_elem.dat");
  
  strcat(s[3],"_restricion.dat");
/*===================================================================*/
/**/  
/*=== arquivo principal*/ 
  fileout[0]=fopen(s[0],"w");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo: %s\n",s[0]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo: %s",s[0]);
/*...................................................................*/
/**/
/*...*/
  write_head_mef(fileout[0],s[1],s[2],s[3],coorbin,elmbin);
  fclose(fileout[0]);
/*===================================================================*/
/**/
/*=== arquivo da coordenadas*/  
  fileout[1]=fopen(s[1],"wb");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo: %s\n",s[1]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo: %s",s[1]);
  write_mef_coor(fileout[1],coorbin);
  fclose(fileout[1]);
/*===================================================================*/
/**/
/*=== arquivo das connectividades*/  
  fileout[2]=fopen(s[2],"wb");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo: %s\n",s[2]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo: %s",s[2]);
  write_mef_cell(fileout[2],elmbin);
  fclose(fileout[2]);
/*===================================================================*/
/**/
/*=== arquivo das restricoes*/  
  fileout[3]=fopen(s[3],"w");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo: %s\n",s[3]);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo: %s",s[3]);
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
void write_head_mef(FILE *f,char *file1,char *file2,char *file3
                   ,bool coorbin,bool elmbin)
{
/*===*/  
  long i;
  short tp;
  char type[20];
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
  if(coorbin)
    fprintf(f,"coordinatesbin %s\n",file1);
  else
    fprintf(f,"insert %s\n",file1);
  
  if(elmbin){
    tp=elemt[0].type;
    switch (tp){
      case 3:
      strcpy(type,"quad4bin");
      break;
      case 4:
      strcpy(type,"tetra4bin");
      break;
      case 5:
      strcpy(type,"hexa8bin");
      break;
      case 23:
      strcpy(type,"quad8bin");
      break;
    }
    fprintf(f,"%s %ld %s\n",type,nelem,file2);
  }  
  else
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
void write_mef_coor(FILE *f,bool bin)
{
/*===*/  
  long i;
  double x;
/*===================================================================*/
/**/
/*===*/
  fprintf(stderr,"\nEscrevendo coordenadas...");
  if(!bin)
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
      for (i = 0 ; i < nnode ; i++){
        write_int(i+1,bin,f);
	write_double(node[i].x,bin,f);
	write_double(node[i].y,bin,f);
	new_section(bin,f);
      }	
      break;
/*-------------------------------------------------------------------*/
/**/
/*---3 dimencoes*/      
    case 3:
      for (i = 0 ; i < nnode ; i++){
        write_int(i+1,bin,f);
	write_double(node[i].x,bin,f);
	write_double(node[i].y,bin,f);
	write_double(node[i].z,bin,f);
	new_section(bin,f);
      }	
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
  if(!bin)
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
void write_mef_cell(FILE *f,bool bin){
/*===*/  
  short tp;
  long i;
  int no;
/*=====================================================================*/
/**/  
/*===*/
  tp=elemt[0].type;
  fprintf(stderr,"\nEscrevendo Elementos...");
/*...*/
  switch(tp){
/*---*/    
    case 2:
      fprintf(stderr,"\nTria3");
      if(!bin)
        fprintf(f,"tria3\n");
      for( i = 0 ; i < nelem ; i ++){
        write_int(i+1,bin,f);
	no = (int )elemt[i].node[0];
        write_int(no,bin,f);
	no = (int)elemt[i].node[1];
        write_int(no,bin,f);
	no = (int)elemt[i].node[2];
        write_int(no,bin,f);
	no = (int)elemt[i].body;
        write_int(no,bin,f);
	new_section(bin,f);
      }	
      if(!bin)
        fprintf(f,"end tria3");
      fprintf(stderr,"\nElementos escritos.");
      break;
/*-------------------------------------------------------------------*/  
/*---*/    
    case 3:
      fprintf(stderr,"\nQuad4");
      if(!bin)
        fprintf(f,"quad4\n");
      for( i = 0 ; i < nelem ; i ++){
        write_int(i+1,bin,f);
	no = (int )elemt[i].node[0];
        write_int(no,bin,f);
	no = (int)elemt[i].node[1];
        write_int(no,bin,f);
	no = (int)elemt[i].node[2];
        write_int(no,bin,f);
	no = (int)elemt[i].node[3];
        write_int(no,bin,f);
	no = (int)elemt[i].body;
        write_int(no,bin,f);
	new_section(bin,f);
      }	
      if(!bin)
        fprintf(f,"end quad4");
      fprintf(stderr,"\nElementos escritos.");
      break;
/*-------------------------------------------------------------------*/  
/*---*/    
    case 4:
      fprintf(stderr,"\nTetra4");
      if(!bin)
        fprintf(f,"tetra4\n");
      for( i = 0 ; i < nelem ; i ++){
        write_int(i+1,bin,f);
	no = (int )elemt[i].node[0];
        write_int(no,bin,f);
	no = (int)elemt[i].node[1];
        write_int(no,bin,f);
	no = (int)elemt[i].node[3];
        write_int(no,bin,f);
	no = (int)elemt[i].node[2];
        write_int(no,bin,f);
	no = (int)elemt[i].body;
        write_int(no,bin,f);
	new_section(bin,f);
      }		 
      if(!bin)		 
        fprintf(f,"end tetra4");
      fprintf(stderr,"\nElementos escritos.");
      break;
/*-------------------------------------------------------------------*/  
/**/      
/*---*/    
    case 5:
      fprintf(stderr,"\nHexa8");
      if(!bin)
        fprintf(f,"hexa8\n");
      for( i = 0 ; i < nelem ; i ++){
        write_int(i+1,bin,f);
	no = (int )elemt[i].node[0];
        write_int(no,bin,f);
	no = (int)elemt[i].node[4];
        write_int(no,bin,f);
	no = (int)elemt[i].node[7];
        write_int(no,bin,f);
	no = (int)elemt[i].node[3];
        write_int(no,bin,f);
	no = (int)elemt[i].node[1];
        write_int(no,bin,f);
	no = (int)elemt[i].node[5];
        write_int(no,bin,f);
	no = (int)elemt[i].node[6];
        write_int(no,bin,f);
	no = (int)elemt[i].node[2];
        write_int(no,bin,f);
	no = (int)elemt[i].body;
        write_int(no,bin,f);
	new_section(bin,f);
      }
      if(!bin)
        fprintf(f,"end hexa8");
      fprintf(stderr,"\nElementos escritos.");
      break;
/*-------------------------------------------------------------------*/ 
/**/
    case 22:
      fprintf(stderr,"\nTria6");
      if(!bin)
        fprintf(f,"tria6\n");
      for( i = 0 ; i < nelem ; i ++){
        write_int(i+1,bin,f);
	no = (int)elemt[i].node[0];
        write_int(no,bin,f);
	no = (int)elemt[i].node[1];
        write_int(no,bin,f);
	no = (int)elemt[i].node[2];
        write_int(no,bin,f);
	no = (int)elemt[i].node[3];
        write_int(no,bin,f);
	no = (int)elemt[i].node[4];
        write_int(no,bin,f);
	no = (int)elemt[i].node[5];
        write_int(no,bin,f);
	no = (int)elemt[i].body;
        write_int(no,bin,f);
	new_section(bin,f);
      }
      if(!bin)
        fprintf(f,"end tria6");
      fprintf(stderr,"\nElementos escritos.");
      break;
/*---*/    
    case 23:
      fprintf(stderr,"\nQuad8");
      if(!bin)
        fprintf(f,"quad8\n");
      for( i = 0 ; i < nelem ; i ++){
        write_int(i+1,bin,f);
	no = (int)elemt[i].node[0];
        write_int(no,bin,f);
	no = (int)elemt[i].node[1];
        write_int(no,bin,f);
	no = (int)elemt[i].node[2];
        write_int(no,bin,f);
	no = (int)elemt[i].node[3];
        write_int(no,bin,f);
	no = (int)elemt[i].node[4];
        write_int(no,bin,f);
	no = (int)elemt[i].node[5];
        write_int(no,bin,f);
	no = (int)elemt[i].node[6];
        write_int(no,bin,f);
	no = (int)elemt[i].node[7];
        write_int(no,bin,f);
	no = (int)elemt[i].body;
        write_int(no,bin,f);
	new_section(bin,f);
      }
      if(!bin)
        fprintf(f,"end quad8");
      fprintf(stderr,"\nElementos escritos.");
      break;
/*-------------------------------------------------------------------*/  
/*---*/
    default:
      printf("Numero de elemento invalido.\n"
	     "funcao write_mef_cell(FILE *f) arquivo = NAME\n");
      exit(0);
      break;
/*-------------------------------------------------------------------*/
    }
  if(!bin)
    fprintf(f,"\nreturn\n");
/*...................................................................*/  
/*===================================================================*/  
}


/*********************************************************************
 * write_gid      : saida para o gid                                 *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
 void write_gid(char *prefixo){
   int GiD_PostAscii;
   int ngid;
   int id;
   int elmi[20];
   char s[200];
   int i;

   strcpy(s,prefixo);
   strcat(s,".msh");
   GiD_PostAscii = 0;
   GiD_OpenPostMeshFile(s,GiD_PostAscii);
   
   if(dim == 2)
     ngid = GiD_2D;
   
   else if(dim == 3)
     ngid = GiD_3D;
     
   if(elemt[0].type == 4)
     GiD_BeginMesh("Cubit mesh", ngid, GiD_Tetrahedra, 4);
   else{
     printf("Somente elementos tetraedricos\n");
     exit(EXIT_FAILURE);
   }  
     
  /* coordinates */

   GiD_BeginCoordinates();
   for ( i = 0; i < nnode; i++ ) {
    id = i + 1;
    GiD_WriteCoordinates( id, node[i].x, node[i].y, node[i].z );
   }
   GiD_EndCoordinates();
   
/* elements */
  GiD_BeginElements();
  for ( i = 0; i < nelem; i++ ) {
/*tetraedros*/    
    if(elemt[i].type == 4){
      id = i + 1;
      elmi[0] = elemt[i].node[0];
      elmi[1] = elemt[i].node[1];
      elmi[2] = elemt[i].node[2];
      elmi[3] = elemt[i].node[3];
      elmi[4] = elemt[i].body;
      GiD_WriteElementMat(id, elmi);
    }  
  }
  
  GiD_EndElements();
  GiD_EndMesh();
  GiD_ClosePostMeshFile();
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
  int no1,no2,nel;
  double dl;
  double x,y;
  double force[3],mf;
/*===================================================================*/  
/*...*/
/*      
  fprintf(stderr,"\nEscrevendo restricion..");
  fprintf(stderr,"\nconstraintemp...");
  fprintf(f,"constraintemp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
        if(nodeset[i].gid[2] || nodeset[i].gid[3])
        fprintf(f,"%10d %s\n",no,"1");
  }
  fprintf(f,"end constraintemp\n");
*/      
/*...*/  
/*     
  fprintf(stderr,"\nnodalsources...");
  fprintf(f,"nodalsources\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
        if(nodeset[i].gid[2])
//      if(nodeset[i].gid[0])
          fprintf(f,"%10d %20.8e\n",no,50.0);
        else if(nodeset[i].gid[3])
          fprintf(f,"%10d %20.8e\n",no,200.0);
  }
  fprintf(f,"end nodalsources\n");
*/    
/**/
/*        
  fprintf(stderr,"\nintialtemp...");
  fprintf(f,"initialtemp\n");
  for(i=0;i<nnode;i++){
    fprintf(f,"%10d %20.8e\n",i+1,50.0);
  }  
  fprintf(f,"end initialtemp\n");
*/      
/*...................................................................*/

/*... incendio*/
/*  fprintf(stderr,"\nconstraindisp...");
  fprintf(f,"constraintemp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[1] && !nodeset[i].gid[2])
        fprintf(f,"%10d %s\n",no,"1");
  }
  fprintf(f,"end constraintemp\n");    
*/  
/*...................................................................*/
  
/*  fprintf(stderr,"\nnodalsources...");
  fprintf(f,"nodalsources\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[1] && !nodeset[i].gid[2])
        fprintf(f,"%10d %s\n",no,"20.0");
      if(nodeset[i].gid[0]){*/
/*calcula aproximadamente a area de influencia de um no atraves
 * da distancia de um aresta qualquer de um elemento a qual akelo no
 * pertence( para carga distribuida na face)*/      
/*      nel = pnode.incid[(no-1)*pnode.maxgrade];
        no1 = elemt[nel-1].node[0];
        no2 = elemt[nel-1].node[1];
        dl=distno(no2,no1);
        fprintf(f,"%10d %lf\n",no,10.0);
      }	
  }
  fprintf(f,"end nodalsources\n");    
*/
        
  fprintf(stderr,"\nintialtemp...");
  fprintf(f,"initialtemp\n");
  for(i=0;i<nnode;i++){
    fprintf(f,"%10d %20.8e\n",i+1,20.0);
  }  
  fprintf(f,"end initialtemp\n");
        
  fprintf(stderr,"\nconstraintemp...");
  fprintf(f,"constraintemp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[0] || nodeset[i].gid[1])
        fprintf(f,"%10d %s\n",no,"1");
  }
  fprintf(f,"end constraintemp\n");    

//  fprintf(stderr,"\nnodalsources...");
//  fprintf(f,"nodalsources\n");
//  for(i=0;i<nnodeset;i++){
//       no = nodeset[i].num;
//       if(nodeset[i].gid[0])
//        fprintf(f,"%10d %s\n",no,"1200.0");
//  }	
//  fprintf(f,"end nodalsources\n");  
  fprintf(f,"return\n");
//  fprintf(stderr,"\nescrito restricion..");//
/*  
  fprintf(stderr,"\nconstraindisp...");
  fprintf(f,"constraindisp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[0])
        fprintf(f,"%10d %s\n",no,"1");
  }
  fprintf(f,"end constraindisp\n");  
  fprintf(stderr,"\nnodalloads...");
  fprintf(f,"nodalloads\n");
   for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if( nodeset[i].gid[0])
        fprintf(f,"%10d %s\n",no,"1");
  }
  fprintf(f,"end nodalloads\n");  
  fprintf(f,"return\n");
  fprintf(stderr,"\nescrito restricion..");
*/
/*===================================================================*/  
}
double modF(int no,double *f)
{
#define F  0.5
  int *face=NULL;
  int i,j,k,kk,l;
  int grade;
  int nel;
  int nfaces;
  int shno,idno;
  int T1[3],T2[3],T3[3],T4[3];
  double A[4],Aface[4],n[3],fn[4][3],modf;

  face = (int*) calloc(8*4,sizeof(int));
  kk = 0;
  fprintf(stderr,"\nno=%d\n",no);
  grade=pnode.nincid[no-1];
/*  fprintf(stderr,"grade=%d\n",grade);*/
/* loop no elementos q este no compartilha*/
  for(i=0;i<grade;i++){
    nel = pnode.incid[(no-1)*pnode.maxgrade+i];
    fprintf(stderr,"nel=%d\n",nel);
    for(j=0;j<maxno;j++){
      shno = elemt[nel-1].node[j];
      fprintf(stderr,"nel=%d no=%d\n",nel,shno);
/* verifica se esse no esta na superficie desejada*/
      for(k=0;k<nnodeset;k++){
        idno = nodeset[k].num;
	if( shno == idno ){
	  if( shno == idno ){
	    if(nodeset[k].gid[0]){
  	      fprintf(stderr,"idno=%d kk=%d\n",idno,kk);
              face[kk] = idno;
	      kk++;
	    }  
	  }  
	}  
      }	  
    }
  }
  
  nfaces = kk/4;
  printf("\n");
  for(i=0;i<nfaces*4;i++)
    printf("%d face=%d\n",i+1,face[i]);
/**/
/*calculo da area das faces*/  
  for(i=0;i<nfaces;i++){
    maketri(&face[i*4],T1,T2,T3,T4);
/*  fprintf(stderr,"\n");*/
/*  fprintf(stderr,"T1=(%d %d %d)\n",T1[0],T1[1],T1[2]);*/
/*  fprintf(stderr,"T2=(%d %d %d)\n",T2[0],T2[1],T2[2]);*/
/*  fprintf(stderr,"T3=(%d %d %d)\n",T3[0],T3[1],T3[2]);*/
/*  fprintf(stderr,"T4=(%d %d %d)\n",T4[0],T4[1],T4[2]);*/
    A[0]=getarea(T1,n);
    A[1]=getarea(T2,n);
    A[2]=getarea(T3,n);
    A[3]=getarea(T4,n);
/*vetor normal a face*/
    fn[i][0] = n[0];
    fn[i][1] = n[1];
    fn[i][2] = n[2];
/*....................................................*/    
    Aface[i] = A[0] + A[1] + A[2] + A[3];
    Aface[i] = Aface[i]/2.0;
//  fprintf(stderr,"no = %d A[%d]=%lf "
//   "normal =(%lf,%lf,%lf) mod = %lf\n"
//   ,no,i+1,Aface[i],fn[i][0],fn[i][1],fn[i][2]
//   ,sqrt(dot(fn[i],fn[i],3)));
    
  }
  
/*Forca equivalente nodal*/  
  f[0] =0.0;
  f[1] =0.0;
  f[2] =0.0;
  for(i=0;i<nfaces;i++){
    for(j=0;j<3;j++)
        f[j] += fn[i][j]*F*Aface[i]/4.0;
  }
/*produto escalar*/
  modf = dot(f,f,3);
//fprintf(stderr,"dot=%lf\n",D);
  modf = sqrt(modf); 
/**/    
  free(face);
  return modf;
}

void maketri(int *face,int *T1, int *T2,int *T3,int *T4){

  T1[0] = T2[0] = T3[0] = T4[0] = face[0];
  T1[1] = T3[1] = T4[1]         = face[1];
  T2[2] = T3[2]                 = face[2];
  T1[2] = T4[2]                 = face[3];
  T2[1]                         = face[3];

}

double distno(int no1,int no2){

  double x21,y21,z21;
  x21 = node[no2-1].x - node[no1-1].x;
  y21 = node[no2-1].y - node[no1-1].y;
  z21 = node[no2-1].z - node[no1-1].z;
  return sqrt( x21*x21 + y21*y21 + z21*z21 );  

}

double getarea(int*T,double *n){
  
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
/*vetor normal a face*/
  n[0]=VxV[0]/D;
  n[1]=VxV[1]/D;
  n[2]=VxV[2]/D;
/*Area*/  
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

/* ********************************************************************
 *  Function: force_big_endian
 *
 *  Purpose:
 *      Determines if the machine is little-endian.  If so, then
 *      , for binary data, it will force the data to be big-endian.
 *
 *  Note:       This assumes that all inputs are 4 bytes long.
 *
 *  Programmer: Hank Childs
 *  Creation:   September 3, 2004
 * 
 * *******************************************************************/

static void force_big_endian(unsigned char *bytes,bool cod,int nbytes)
{
    static int doneTest = 0;
    static int shouldSwap = 0;
    if (!doneTest)
    {
        int tmp1 = 1;
        unsigned char *tmp2 = (unsigned char *) &tmp1;
        if (*tmp2 != 0)
            shouldSwap = 1;
        doneTest = 1;
    }

    if (shouldSwap & cod)
    {
	if(nbytes==8){
//          fprintf(stderr,"8 bytes\n");
          unsigned char tmp = bytes[0];
          bytes[0] = bytes[7];
          bytes[7] = tmp;
          tmp = bytes[1];
          bytes[1] = bytes[6];
          bytes[6] = tmp;
          tmp = bytes[2];
          bytes[2] = bytes[5];
          bytes[5] = tmp;
          tmp = bytes[3];
          bytes[3] = bytes[4];
          bytes[4] = tmp;
	}
        else if(nbytes==4){	
/*	  fprintf(stderr,"4 bytes\n");*/
          unsigned char tmp = bytes[0];
          bytes[0] = bytes[3];
          bytes[3] = tmp;
          tmp = bytes[1];
          bytes[1] = bytes[2];
          bytes[2] = tmp;
       }
    }
}

static void write_double(double val,bool cod,FILE *f)
{
    if (cod)
    {
        force_big_endian((unsigned char *) &val,cod,8);
        fwrite(&val, sizeof(double), 1, f);
    }
    else
    {
        char str[128];
        sprintf(str, "%20.12e ", val);
        fprintf(f, str);
    }
}
/* *******************************************************************
 *  Function: write_int
 *
 *  Purpose:
 *      Writes an integer to the currently open file.  This routine
 *      takes care of ASCII vs binary issues.
 *
 *  Programmer: Hank Childs
 *  Creation:   September 3, 2004
 * 
 * *******************************************************************/

static void write_int(int val,bool cod,FILE *f)
{
    if (cod)
    {
        force_big_endian((unsigned char *) &val,cod,4);
        fwrite(&val, sizeof(int), 1, f);
    }
    else
    {
        char str[128];
        sprintf(str, "%d ", val);
        fprintf(f, str);

    }
}

static void new_section(bool cod,FILE *f){
 
  if(cod);
  else
    fprintf(f,"\n");

}

