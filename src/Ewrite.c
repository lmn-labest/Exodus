#define NAME "Ewrite.c"
#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define pi 3.14

static void write_double(double,bool,FILE*);
static void force_big_endian(unsigned char *,bool,int);
static void write_int(int,bool,FILE*);
static void new_section(bool cod,FILE *f);
static void trocaFace(void);
static void trocaFaceMvf(void);

/* funcao de apoio*/
double distno(int,int);

/*********************************************************************
 * write_mvf : escreve o arquivo no formato mvf                      *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * ins       - nome do arquivo de saida                              *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_mvf(char *ins){
/*===*/
  FILE *fileout=NULL;
  char s[200];
/*===================================================================*/

/*=== Manipulação do nome do arquivo*/  
  strcpy(s,ins);
  strcat(s,".dat");
  
/*===================================================================*/

/*== renumera as faces*/
  trocaFaceMvf();
/*===================================================================*/
  
/*=== arquivo principal*/ 
  fileout=fopen(s,"w");
  if (fileout==NULL){ 
    fprintf(stderr,"Erro na abertura do arquivo: %s\n",s);
    exit(1);
  } 
  fprintf(stderr,"\nsucesso na abertura do arquivo: %s",s);
/*...................................................................*/
/**/
/*...*/
/*===================================================================*/
/**/
/*=== arquivo da coordenadas*/  
  fprintf(stderr,"\nsucesso na abertura do arquivo: %s",s);
  write_mvf_coor(fileout);
/*===================================================================*/
/**/
/*=== arquivo das connectividades*/  
  write_mvf_cell(fileout);
/*===================================================================*/
/**/
/*=== arquivo das restricoes*/  
  write_mvf_res(fileout);
/*===================================================================*/
}
/*********************************************************************/

/*********************************************************************
 * write_mef : escreve o arquivo no formato                          *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * ins       - nome do arquivo de saida                              *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
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

/*== renumera as faces*/
  trocaFace();
/*===================================================================*/
  
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
      case QUAD4:
      strcpy(type,"quad4bin");
      break;
      case TETRA4:
      strcpy(type,"tetra4bin");
      break;
      case HEXA8:
      strcpy(type,"hexa8bin");
      break;
      case QUAD8:
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
 * write_head_coor : escreve o arquivo dos coordenadas               *
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
/*********************************************************************
 * write_mvf_coor : escreve o arquivo dos coordenadas                *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_mvf_coor(FILE *f)
{
/*===*/  
  long i;
  double x;
  bool bin = false;
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
  fprintf(f,"endCoordinates\n");

/*===================================================================*/
}
/*********************************************************************/  

/*********************************************************************
 * write_mef_cell: escreve o arquivo dos elementos                   *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_mvf_cell(FILE *f){
/*===*/  
  short tp;
  long i;
  int no,j,nel;
  bool bin = false;
/*=====================================================================*/
/**/

/*===*/
  fprintf(stderr,"\nEscrevendo Elementos...");
  fprintf(f,"cells\n");
  for( i = 0 ; i < nelem ; i ++){
    tp=elemt[i].type;
/*...*/
    switch(tp){
/*--- tria3*/    
      case TRIA3:
          nel = elemt[i].num;
          write_int(nel,bin,f);
/*... mat*/          
          no = (int)elemt[i].body;
          write_int(no,bin,f);
/*... tipo geometrico*/
          write_int(2,bin,f);
/*... */                 
          write_int(3,bin,f);
          write_int(3,bin,f);
/*...*/
          no = (int )elemt[i].node[0];
          write_int(no,bin,f);
          no = (int)elemt[i].node[1];
          write_int(no,bin,f);
          no = (int)elemt[i].node[2];
          write_int(no,bin,f);
          no = (int)elemt[i].body;
          write_int(no,bin,f);
          new_section(bin,f);
      break;
/*-------------------------------------------------------------------*/  

/*--- quad4*/    
      case QUAD4:
          nel = elemt[i].num;
          write_int(nel,bin,f);
/*... mat*/          
          no = (int)elemt[i].body;
          write_int(no,bin,f);
/*... tipo geometrico*/
          write_int(3,bin,f);
/*... */                 
          write_int(4,bin,f);
          write_int(4,bin,f);
/*...*/
          no = (int )elemt[i].node[0];
          write_int(no,bin,f);
          no = (int)elemt[i].node[1];
          write_int(no,bin,f);
          no = (int)elemt[i].node[2];
          write_int(no,bin,f);
          no = (int)elemt[i].node[3];
          write_int(no,bin,f);
          new_section(bin,f);
      break;
/*-------------------------------------------------------------------*/  

/*--- tetra4*/    
      case TETRA4:
          nel = elemt[i].num;
          write_int(nel,bin,f);
/*... mat*/          
          no = (int)elemt[i].body;
          write_int(no,bin,f);
/*... tipo geometrico*/
          write_int(4,bin,f);
/*... */                 
          write_int(4,bin,f);
          write_int(4,bin,f);
/*...*/
          no = (int )elemt[i].node[0];
          write_int(no,bin,f);
          no = (int)elemt[i].node[1];
          write_int(no,bin,f);
          no = (int)elemt[i].node[3];
          write_int(no,bin,f);
          no = (int)elemt[i].node[2];
          write_int(no,bin,f);
          new_section(bin,f);
      break;
/*-------------------------------------------------------------------*/  

/*--- hexa8*/
      case HEXA8:
          nel = elemt[i].num;
          write_int(nel,bin,f);
/*... mat*/          
          no = (int)elemt[i].body;
          write_int(no,bin,f);
/*... tipo geometrico*/
          write_int(5,bin,f);
/*... */                 
          write_int(8,bin,f);
          write_int(6,bin,f);
/*...*/
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
          new_section(bin,f);
      break;
/*-------------------------------------------------------------------*/ 

/*...*/
      default:
        printf("Numero de elemento invalido.\n");
        exit(0);
        break;
/*-------------------------------------------------------------------*/
    }
/*-------------------------------------------------------------------*/
  }
/*-------------------------------------------------------------------*/
  fprintf(f,"endCells\n");
  fprintf(stderr,"\nElementos escritos.");
/*...................................................................*/
/*===================================================================*/
}
/*********************************************************************/

/*********************************************************************
 * write_mvf_res: escreve as restricoes                              *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_mvf_res(FILE *f)
{
/*===*/
  int i,j,side,nFace;
  int el,nel;
/*===================================================================*/  

/*...*/
  
  fprintf(stderr,"\nfaceRd1...");
  fprintf(f,"faceRd1\n");
  for(i=0;i<nsideset;i++){
    if(sideset[i].ngid){
      el    = sideset[i].num;
      nel   =  elemt[el-1].num; 
      fprintf(f,"%10d %2d",nel,7);
      for(j=0;j<6;j++){
        side = sideset[i].side[j];
        if(side == 1 || side == 2 || side == 3  || side == 4 )
          fprintf(f,"%3d ",1);
        else
          fprintf(f,"%3d ",side);
      }
      fprintf(f,"%3d ",0);
      fprintf(f,"\n");
    } 
  }
  fprintf(f,"endFaceRd1\n"); 
  fprintf(stderr,"\nfaceRd1 escrito.");
  
  fprintf(stderr,"\nfaceSd1...");
  fprintf(f,"faceSd1\n");
  for(i=0;i<nsideset;i++){
    if(sideset[i].ngid){
      el    = sideset[i].num;
      nel   =  elemt[el-1].num; 
      fprintf(f,"%10d %2d ",nel,7);
      for(j=0;j<6;j++){
        side = sideset[i].side[j];
        if(side == 1)
          fprintf(f,"%16.6lf ",100.0);
        else if(side == 2)
          fprintf(f,"%16.6lf ",1000.0);
        else if(side == 3)
          fprintf(f,"%16.6lf ",1000.0);
        else if(side == 4)
          fprintf(f,"%16.6lf ",100.0);
        else
          fprintf(f,"%16.6lf ",0.0);
      }
      fprintf(f,"%16.6lf ",0.0);
      fprintf(f,"\n");
    } 
  }
  fprintf(f,"endFaceSd1\n"); 
  fprintf(stderr,"\nfaceR1 escrito.");
}

/*********************************************************************
 * write_mef_cell: escreve o arquivo dos elementos                   *
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
  int no,j,nel;
  int typeElm[NTYPEELM],nElm[6];
  char nameElm[NTYPEELM][10] ={"tria3","quad4","tetra4"
                              ,"hexa8","tria6","quad8"}; 
/*=====================================================================*/
/**/
  typeElm[0] = TRIA3;
  typeElm[1] = QUAD4;
  typeElm[2] = TETRA4;
  typeElm[3] = HEXA8;
  typeElm[4] = TRIA6;
  typeElm[5] = QUAD8;
  nElm[0]    = 0;
  nElm[1]    = 0;
  nElm[2]    = 0;
  nElm[3]    = 0;
  nElm[4]    = 0;
  nElm[5]    = 0;
/*...*/
  for(j = 0; j < NTYPEELM;j++)
    for( i = 0 ; i < nelem ; i ++){
      tp=elemt[i].type;
      switch(tp){
/*---*/    
          case TRIA3:
          nElm[0]++;
          break;
/*-------------------------------------------------------------------*/  
/*---*/    
          case QUAD4:
          nElm[1]++;
          break;
/*-------------------------------------------------------------------*/  
/*---*/    
          case TETRA4:
          nElm[2]++;
          break;
/*-------------------------------------------------------------------*/  
/*---*/
          case HEXA8:
          nElm[3]++;
          break;
/*-------------------------------------------------------------------*/ 
/*---*/    
          case TRIA6:
          nElm[4]++;
          break;
/*-------------------------------------------------------------------*/  
/*---*/    
          case QUAD8:
          nElm[5]++;
          break;
/*-------------------------------------------------------------------*/  
/*---*/
          default:
          printf("Numero de elemento invalido.\n");
          exit(0);
          break;
/*-------------------------------------------------------------------*/
        }
      
    }
/*.....................................................................*/

/*===*/
  fprintf(stderr,"\nEscrevendo Elementos...");
  for(j = 0; j < NTYPEELM;j++){
    if(nElm[j]){
      fprintf(stderr,"\n%s",nameElm[j]);
      if(!bin)
        fprintf(f,"%s\n",nameElm[j]);
    }
    for( i = 0 ; i < nelem ; i ++){
      tp=elemt[i].type;
      if( tp == typeElm[j]){  
/*...*/
        switch(tp){
/*--- tria3*/    
          case TRIA3:
          nel = elemt[i].num;
          write_int(nel,bin,f);
          no = (int )elemt[i].node[0];
          write_int(no,bin,f);
          no = (int)elemt[i].node[1];
          write_int(no,bin,f);
          no = (int)elemt[i].node[2];
          write_int(no,bin,f);
          no = (int)elemt[i].body;
          write_int(no,bin,f);
          new_section(bin,f);
          break;
/*-------------------------------------------------------------------*/  

/*--- quad4*/    
          case QUAD4:
          nel = elemt[i].num;
          write_int(nel,bin,f);
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
          break;
/*-------------------------------------------------------------------*/  

/*--- tetra4*/    
          case TETRA4:
          nel = elemt[i].num;
          write_int(nel,bin,f);
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
          break;
/*-------------------------------------------------------------------*/  

/*--- hexa8*/
          case HEXA8:
          nel = elemt[i].num;
          write_int(nel,bin,f);
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
          break;
/*-------------------------------------------------------------------*/ 

/*... tria6*/
          case TRIA6:
          nel = elemt[i].num;
          write_int(nel,bin,f);
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
          break;
/*-------------------------------------------------------------------*/ 

/*-... quad8*/    
          case QUAD8:
          nel = elemt[i].num;
          write_int(nel,bin,f);
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
          break;
/*-------------------------------------------------------------------*/  

/*...*/
          default:
          printf("Numero de elemento invalido.\n");
          exit(0);
          break;
/*-------------------------------------------------------------------*/
        }
/*-------------------------------------------------------------------*/
      }
/*-------------------------------------------------------------------*/
    } 
/*-------------------------------------------------------------------*/
    if(nElm[j]){
      fprintf(stderr,"\nend %s",nameElm[j]);
      if(!bin)
        fprintf(f,"end %s\n",nameElm[j]);
/*-------------------------------------------------------------------*/
    }
/*-------------------------------------------------------------------*/
  }
  fprintf(stderr,"\nElementos escritos.");
  if(!bin)
    fprintf(f,"return\n");
/*...................................................................*/
/*===================================================================*/
}
/*********************************************************************/

/*********************************************************************
 * write_gid      : saida para o gid                                 *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 * OBS: utiliza a lib gidPost                                        *
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
 * GAMBIARRA: para escrever as restricoes                            *
 *------------------------------------------------------------------ *
 * parametros de entrada :                                           *
 * ----------------------------------------------------------------- *
 * f         - arquivo de saida                                      *
 *------------------------------------------------------------------ *
 * parametros de saida :                                             *
 * ----------------------------------------------------------------- *
 *********************************************************************/  
void write_restricion(FILE *f)
{
/*===*/
  int i,j,k,nset;
  int no,el,side;
  int no1,no2,nel;
  double dl;
  double x,y;
  double force[3],mf;
/*===================================================================*/  

/*...*/
  
  fprintf(stderr,"\nfaceR1...");
  fprintf(f,"faceR1\n");
  for(i=0;i<nsideset;i++){
    if(sideset[i].ngid){
      el  = sideset[i].num;
//      nel =  elemt[el-1].num;  
      fprintf(f,"%10d ",nel);
      for(j=0;j<elemt[nel-1].nen+1;j++){
        side = sideset[i].side[j];
        fprintf(f,"%3d ",side);
      }
      fprintf(f,"\n");
    } 
  }
  fprintf(f,"end faceR1\n"); 
   
  fprintf(f,"nodalsources\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[0])
        fprintf(f,"%10d %s\n",no, "25");
      if(nodeset[i].gid[1])
        fprintf(f,"%10d %s\n",no, "100");
  }
  
  fprintf(f,"end nodalsources\n");
  fprintf(f,"return\n");
  return;

  fprintf(stderr,"\nconstraintemp...");
  fprintf(f,"constraintemp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[0])
        fprintf(f,"%10d %s\n",no, "1");
      if(nodeset[i].gid[1])
        fprintf(f,"%10d %s\n",no, "1");
  }
  
  fprintf(f,"end constraintemp\n");    
  fprintf(f,"nodalsources\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[0])
        fprintf(f,"%10d %s\n",no, "25");
      if(nodeset[i].gid[1])
        fprintf(f,"%10d %s\n",no, "100");
  }
  
  fprintf(f,"end nodalsources\n");
  fprintf(f,"return\n");
  return;
  
/*
  fprintf(stderr,"\nconstraintemp...");
  fprintf(f,"constraintemp\n");
  for(i=0;i<nnodeset;i++){
    no = nodeset[i].num;
      if(nodeset[i].gid[0])
        fprintf(f,"%10d %d\n",no,1);
      if(nodeset[i].gid[1])
        fprintf(f,"%10d %d\n",no,1);
  }
  fprintf(f,"end constraintemp\n");    

  fprintf(stderr,"\nnodalsources...");
  fprintf(f,"nodalsources\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[0])
        fprintf(f,"%10d %15.5lf\n",no,   2.0);
      if(nodeset[i].gid[1])
        fprintf(f,"%10d %15.5lf\n",no,   1.0);
  }
  fprintf(f,"end nodalsources\n");    
  return;*/

  fprintf(stderr,"\nconstraindisp...");
  fprintf(f,"constraindisp\n");
  for(i=0;i<nnodeset;i++){
      no = nodeset[i].num;
      if(nodeset[i].gid[2] && (!nodeset[i].gid[5]&&!nodeset[i].gid[6]))
        fprintf(f,"%10d %s\n",no, "0 1 0");
      if(nodeset[i].gid[9] && !nodeset[i].gid[5])
        fprintf(f,"%10d %s\n",no, "0 0 1");
      if(nodeset[i].gid[5])
        fprintf(f,"%10d %s\n",no, "0 1 1");
      if(nodeset[i].gid[3] && !nodeset[i].gid[6])
        fprintf(f,"%10d %s\n",no, "1 0 0");
      if(nodeset[i].gid[6])
        fprintf(f,"%10d %s\n",no, "1 1 0");
  }
  fprintf(f,"end constraindisp\n");    


  
  fprintf(stderr,"\nintialtemp...");
  fprintf(f,"initialtemp\n");
  for(i=0;i<nnode;i++){
      fprintf(f,"%10d %15.5lf\n",i+1,25.0);
  }
  fprintf(f,"end initialtemp\n");
  fprintf(f,"return\n");

  fprintf(stderr,"\nescrito restricion..");
  
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

/*subtracao de vetores*/
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
        sprintf(str, "%16d ", val);
        fprintf(f, str);

    }
}

static void new_section(bool cod,FILE *f){
 
  if(cod);
  else
    fprintf(f,"\n");

}
/********************************************************************* 
 * chanceFace                                                        * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
static void trocaFace(void){

  int  temp[6],el,i,tp;
/*=== reorganicao das faces*/
   for(i=0;i<nsideset;i++){
    el                 = sideset[i].num;
    tp=elemt[el-1].type;
    switch (tp){
/*tetraedro*/
      case TETRA4:
        temp[0]            = sideset[i].side[0];
        temp[1]            = sideset[i].side[1];
        temp[2]            = sideset[i].side[2];
        temp[3]            = sideset[i].side[3];
        sideset[i].side[0] =  temp[0];
        sideset[i].side[1] =  temp[3];
        sideset[i].side[2] =  temp[1];
        sideset[i].side[3] =  temp[2];
      break;
    }
  }
/*===================================================================*/

}

/********************************************************************* 
 * chanceFace                                                        * 
 *-------------------------------------------------------------------* 
 * Parametros de entrada:                                            * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * Parametros de saida:                                              * 
 *-------------------------------------------------------------------* 
 *                                                                   * 
 *-------------------------------------------------------------------* 
 * OBS:                                                              * 
 *-------------------------------------------------------------------* 
 *********************************************************************/
static void trocaFaceMvf(void){

  int  temp[6],el,i,tp;
/*=== reorganicao das faces*/
   for(i=0;i<nsideset;i++){
    el                 = sideset[i].num;
    tp=elemt[el-1].type;
    switch (tp){
/*tetraedro*/
      case HEXA8: 
        temp[0]            = sideset[i].side[0];
        temp[1]            = sideset[i].side[1];
        temp[2]            = sideset[i].side[2];
        temp[3]            = sideset[i].side[3];
        temp[4]            = sideset[i].side[4];
        temp[5]            = sideset[i].side[5];
        sideset[i].side[0] =  temp[4];
        sideset[i].side[1] =  temp[5];
        sideset[i].side[2] =  temp[0];
        sideset[i].side[3] =  temp[1];
        sideset[i].side[4] =  temp[2];
        sideset[i].side[5] =  temp[3];
      break;
    }
  }
/*===================================================================*/

}


