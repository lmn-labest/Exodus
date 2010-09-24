#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>
void read_exo(char *file_name)
{
/*===*/
  int CPU_ws,IO_ws;
  int exoid,error;
  float version;
/*...................................................................*/  
  int numset,numdf;
  int id;
  int *ids;
  int i,j;
  int *node_list;
/*...................................................................*/  
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
/*===lendo carregamentos*/
  fprintf(stderr,"Lendo carregamentos...\n");
  read_carg_exo(exoid);
  fprintf(stderr,"carregamentos.\n");
/*===================================================================*/
/**/
/*===fechando arquivo*/
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
/*...*/  
  fprintf(stderr,"\n****************\n"
                 "dim     = %d \n"
                 "nnode   = %d \n"
		 "nelm    = %d \n" 
		 "mat     = %d \n"
		 "nodeset = %d \n"
		 "****************\n"
		 ,edim,enode,eelem,nelemblock,enodeset);  
/*...................................................................*/  

/*...*/  
  dim      = edim;
  nnode    = enode;
  nelem    = eelem;
  nbody    = nelemblock;
  nnodeset = enodeset;
/*...................................................................*/  
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

/*********************************************************************/
void read_carg_exo(int exoid){
/*===*/  
  int i,j,k;
  int error = 0;
  int *ids=NULL;
  int *node_list=NULL;
  int numset,numdf;
/*===================================================================*/  
  
/*===*/  
  
/*... numero total de grupos de nos com restricoes*/
/*...*/  
//nodeset.g = (int *) malloc(nnodeset*sizeof(int));
/*.................................................*/  

  ids = (int *) calloc(nnodeset, sizeof(int));
  error = ex_get_node_set_ids (exoid, ids);

#if _DEBUG
  for(i=0;i<nnodeset;i++){
    fprintf(stderr,"%d ids=%d\n",i+1,ids[i]);
  }
#endif
/*...................................................................*/  
  
  nodeset.inode = (int *) calloc(nnodeset, sizeof(int));
  nodeset.nset  = (int *) calloc(nnodeset, sizeof(int));
  
  for(i=0;i<nnodeset;i++){
    error = ex_get_node_set_param (exoid,ids[i],&numset,&numdf);
    nodeset.nset[i]   = numset;
  }

  for(i=1;i<nnodeset;i++)
    nodeset.inode[i]  = nodeset.nset[i-1] + nodeset.inode[i-1];
  
  nodeset.total  = nodeset.inode[nnodeset-1] + nodeset.nset[nnodeset-1];
  nodeset.node   = (int *) calloc(nodeset.total, sizeof(int));
  nodeset.id     = (int *) calloc(nodeset.total, sizeof(int));
  
  for(i=0;i<nnodeset;i++){

#if _DEBUG
    fprintf(stderr,"grupo = %d pont = %d length = %d\n"
	          ,i,nodeset.inode[i]
	          ,nodeset.nset[i]);
#endif

    node_list = (int *) calloc(nodeset.nset[i], sizeof(int));
    error = ex_get_node_set (exoid, ids[i], node_list);
    for(j=0;j<nodeset.nset[i];j++){
      k = nodeset.inode[i];
      nodeset.node[k+j] = node_list[j];
      nodeset.id[k+j]   = ids[i];
#if _DEBUG
      fprintf(stderr,"%d no = %d\n",j+1,node_list[j]);
#endif

    }
    free(node_list);
  }
  free(ids);

#if _DEBUG
  for(i=0;i<nodeset.total;i++){
    fprintf(stderr,"%d no = %d id = %d\n"
	          ,i+1,nodeset.node[i],nodeset.id[i]);
  }  
#endif
 
/*===================================================================*/  
}
/*********************************************************************/


  




