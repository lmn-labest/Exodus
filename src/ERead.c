#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>
void read_exo(char *file_name)
{
/*===*/
  int CPU_ws,IO_ws;
  int exoid,error;
  float version;  
/*===================================================================*/
/**/
  CPU_ws = sizeof(double);
  IO_ws = 0;
/*===Abrindo arquivo*/  
  exoid = ex_open (file_name,EX_READ,&CPU_ws,&IO_ws
                  ,&version);
/*...*/
  if(exoid < 0){
    fprintf(stderr,"Erro na abertura do arquivo %s.\n",file_name);
    exit(0);
  }  
  else
    fprintf(stderr,"Sucesso na abertura do arquivo %s.\n",file_name);
/*===================================================================*/
/**/
/*===versão do arquivo*/
  printf("Exodus version  %f\n",version);
/*===================================================================*/
/**/
/*===lendo cabecalho do arquivo */
  fprintf(stderr,"Lendo cabecalho...");
  read_head_exo(exoid);
  fprintf(stderr,"cabecalho lido\n");
/*===================================================================*/
/**/
/*===lendo coordenadas*/
  fprintf(stderr,"Lendo coordenadas...");
  read_coor_exo(exoid);
  fprintf(stderr,"coordenadas lidas.\n");
/*===================================================================*/
/**/
/*===lendo elementos*/
  fprintf(stderr,"Lendo elementos...");
  read_elem_exo(exoid);
  fprintf(stderr,"elementos lidos.\n");
/*===================================================================*/
/**/
/*===fecahndo arquivo*/
  error = ex_close(exoid);
  error_exo (error,"Fechar");
}
/*********************************************************************/
/**/
/*********************************************************************/
void read_head_exo(int exoid)
{  
/*===*/  
  char title[MAX_LINE_LENGTH+1];
  int edim,enode,eelem,nelemblock,enodeset,dum;
  int error = 0;
/*===================================================================*/  
/**/
/*===*/
  error = ex_get_init(exoid,title,&edim,&enode,&eelem,&nelemblock
                     ,&enodeset,&dum);
  error_exo(error,"cabecalho");
/*===================================================================*/
/**/ 
/*===*/
  dim   = edim;
  nnode = enode;
  nelem = eelem;
  nbody = nelemblock;
/*===================================================================*/  
}
/*********************************************************************/
/**/
/*********************************************************************/
void read_coor_exo(int exoid){
/*===*/  
  double *x,*y,*z;
  int i,error = 0;
/*===================================================================*/
/**/
/*===*/  
  x = (double *) calloc(nnode,sizeof(double));
  y = (double *) calloc(nnode,sizeof(double));
  z = (double *) calloc(nnode,sizeof(double));
  error = ex_get_coord(exoid,x,y,z);
  error_exo(error,"Coordenadas");
/*===================================================================*/
/**/
/*===*/
   node = (NODE *) calloc(nnode,sizeof(NODE));
  for(i=0;i<nnode;i++){
    node[i].x = x[i];    
    node[i].y = y[i];    
    node[i].z = z[i];
  }  
/*===================================================================*/
/**/
/*===*/  
  free(x);
  free(y);
  free(z);
/*===================================================================*/  
}
void read_elem_exo(exoid){
/*===*/
  char element_type[MAX_STR_LENGTH + 1];
  int id,num_el_in_blk,num_nod_per_el,num_attr,px;
  int *connect;
  int i,j,ii,jj=0,error =0 ;
  float *attrib;
/*===================================================================*/
/**/
    elemt = (ELEMT *) calloc(nelem,sizeof(ELEMT));
/*===*/ 
  for(ii=1;ii<nbody+1;ii++){
    id = ii;
    error = ex_get_elem_block (exoid,id,element_type,&num_el_in_blk
	                      ,&num_nod_per_el,&num_attr);
    error_exo(error,"Elementos");
    connect = (int *) calloc(num_nod_per_el*num_el_in_blk,sizeof(int));
    error = ex_get_elem_conn (exoid, id, connect);
    error_exo(error,"Incidencias");
    for(i=0;i<num_el_in_blk;i++){
      elemt[jj].type = type_elm(element_type);
      elemt[jj].nen  = num_nod_per_el;
      elemt[jj].body = id;
      px = num_nod_per_el*i;
        for(j=0;j<num_nod_per_el;j++)
          elemt[jj].node[j] = connect[j+px];
      jj++;
    }
  } 
/*===================================================================*/
/**/
/*===*/
   free(connect);
/*===================================================================*/   
}
/*********************************************************************/
/**/
/*********************************************************************/
void error_exo(int error , char *field){
  if( error < 0){
    fprintf(stderr,"Erro na leitura do %s %d\n",field,error);
    exit(0);
  }  
  else if( error > 0)
    fprintf(stderr,"Warning na leitura do %s %d\n",field,error);
}
/*********************************************************************/ 
int type_elm(char *s)
{
  short i;
  if(!strcmp(s,"TETRA"))
     i = 4;
  else if(!strcmp(s,"HEX8"))
     i = 5;
  return i;
}
/*********************************************************************/



  




