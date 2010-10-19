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
  dim       = edim;
  nnode     = enode;
  nelem     = eelem;
  nbody     = nelemblock;
  ngeomset  = enodeset;
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
  int *connect=NULL;
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
    free(connect);
  } 
/*===================================================================*/
/*===*/
  maxno = num_nod_per_el;
  connect = (int *) malloc(nelem*maxno*sizeof(int));
  for(i=0;i<nelem;i++){
    px = maxno*i;
    for(j=0;j<maxno;j++)
      connect[px+j] = elemt[i].node[j];
  }  
  propnode_malloc_geral(pnode.nincid,pnode.incid,connect  
                       ,nnode,nelem,maxno,&(pnode.maxgrade));
  free(connect);
/*===================================================================*/  
/*===*/
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
  if(!strcmp(s,"QUAD"))
     i = 3;
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
  char *repnodeset=NULL;
  int *ids=NULL;
  int *id=NULL;
  int *node_list=NULL;
  int *node_set_list=NULL;
  int num_total,numdf,numset;
  float fdum;
  char *cdum;
/*===================================================================*/  
  
/*===*/  

  ids = (int *) calloc(ngeomset,sizeof(int));
  error = ex_get_node_set_ids (exoid, ids);

#if _DEBUG
  for(i=0;i<ngeomset;i++){
    fprintf(stderr,"%d ids=%d\n",i+1,ids[i]);
  }
#endif
/*...................................................................*/  
  
/*numero de nos com restricao*/
      error = ex_inquire (exoid, EX_INQ_NS_NODE_LEN, &num_total, &fdum,
      cdum);
      node_list     = (int*) calloc (num_total,sizeof(int));
      id            = (int *) calloc(num_total,sizeof(int));
      k = 0;
      for (i=0;i<ngeomset;i++){
        error     = ex_get_node_set_param (exoid,ids[i],&numset,&numdf);
        node_set_list = (int*) calloc(numset,sizeof(int));
        error      = ex_get_node_set (exoid, ids[i], node_set_list);
	for( j = 0; j < numset; j++){ 
	  node_list[j+k]  = node_set_list[j];
	  id[j+k]         = ids[i];
	}  
        k += numset;
        free(node_set_list); 
      }
#if _DEBUG
  for(i=0;i<num_total;i++){
    fprintf(stderr,"%d node_list=%d id=%d\n",i+1,node_list[i],id[i]);
  }
#endif
  insertionsort(node_list,id,num_total);
  repnodeset     = (char*) calloc (num_total,sizeof(char));
  contnodeset(node_list,repnodeset,&nnodeset,num_total);
#if _DEBUG
  printf("nnodeset=%d\n",nnodeset);
  for(i=0;i<num_total;i++){
    fprintf(stderr,"%d node_list=%d id=%d rep=%d\n",i+1,node_list[i],id[i],repnodeset[i]);
  }
#endif
  
  nodeset = (NODESET *) calloc(nnodeset,sizeof(NODESET));
  k =-1;
  for(i=0;i<num_total;i++){
    if(!repnodeset[i]){
      k++;
      nodeset[k].num          = node_list[i];
      nodeset[k].gid[id[i]-1] = 1;
      nodeset[k].ngid         = 1;
    }  
    else{  
      nodeset[k].gid[id[i]-1] = 1;
      nodeset[k].ngid +=1;
    }  
  }
#if _DEBUG
  for(i=0;i<nnodeset;i++){
    fprintf(stderr,"%d node=%d id=%d id=%d id=%d\n"
    ,i+1,nodeset[i].num
    ,nodeset[i].gid[0],nodeset[i].gid[1],nodeset[i].gid[2]);
  }
#endif

 
/*===================================================================*/  
}
/*********************************************************************/

void contnodeset(int *v,char*nv,long *n,int num)
{
  int i,k;
  int aux;
  
  k   = 0;
  aux = v[0];
  for(i=1;i<num;i++){
    if( aux == v[i]){
      nv[i]= 1 ;
      k++;
    }  
    else{
     nv[i] = 0; 
     aux = v[i];
   }  
  }
/*  printf("%d\n",k);*/
  *n = num - k;
}
void insertionsort(int *v,int *b,int n)
{
 int i, j, chave,chave1;
 
 for(j=1; j<n; j++) 
 {
   chave  = v[j];
   chave1  = b[j];
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
