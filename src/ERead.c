#include<Eglobal.h>
#include<stdio.h>
#include<stdlib.h>

/*********************************************************************/
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
  fprintf(stderr,"Lendo carregamentos por no...\n");
  read_carg_node_exo(exoid);
  fprintf(stderr,"carregamentos.\n");
/*===================================================================*/

/*===lendo carregamentos*/
  fprintf(stderr,"Lendo carregamentos por face...\n");
  read_carg_side_exo(exoid);
  fprintf(stderr,"carregamentos.\n");
/*===================================================================*/
/**/
/*===fechando arquivo*/
  error = ex_close(exoid);
  error_exo (error,"Fechar");
}
/*********************************************************************/

/*********************************************************************/
void read_head_exo(int exoid)
{  
/*===*/  
  char title[MAX_LINE_LENGTH+1];
  int edim,enode,eelem,nelemblock,enodeset,esideset;
  int error = 0;
/*===================================================================*/  
/**/
/*===*/
  error = ex_get_init(exoid,title,&edim,&enode,&eelem,&nelemblock
                     ,&enodeset,&esideset);
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
		 "sideset = %d \n"
		 "****************\n"
		 ,edim,enode,eelem,nelemblock,enodeset,esideset);  
/*...................................................................*/  

/*...*/  
  dim           = edim;
  nnode         = enode;
  nelem         = eelem;
  nbody         = nelemblock;
  ngeomsetnode  = enodeset;
  ngeomsetside  = esideset;
/*...................................................................*/  
/*===================================================================*/  
}
/*********************************************************************/

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
/*********************************************************************/

/*********************************************************************/
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
  maxno = 0;
  for(ii=1;ii<nbody+1;ii++){
    id = ii;
    error = ex_get_elem_block (exoid,id,element_type,&num_el_in_blk
	                      ,&num_nod_per_el,&num_attr);
    error_exo(error,"Elementos");
    maxno = MAX(maxno,num_nod_per_el);
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
}
/*********************************************************************/

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
  short i=0;
  if(!strcmp(s,"TRI")||!strcmp(s,"TRIA3"))
     i = 2;
  if(!strcmp(s,"QUAD")||!strcmp(s,"QUAD4"))
     i = 3;
  else if(!strcmp(s,"TETRA")||!strcmp(s,"TETRA4"))
     i = 4;
  else if(!strcmp(s,"HEX")||!strcmp(s,"HEX8"))
     i = 5;
  else if(!strcmp(s,"TRI6"))
     i = 22;
  else if(!strcmp(s,"QUAD8"))
     i = 23;
  return i;
}
/*********************************************************************/

/*********************************************************************/
void read_carg_node_exo(int exoid){
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

  ids = (int *) calloc(ngeomsetnode,sizeof(int));
  error = ex_get_node_set_ids (exoid, ids);

#if _DEBUG
  for(i=0;i<ngeomsetnode;i++){
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
  for (i=0;i<ngeomsetnode;i++){
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
  insertionsort2Vector(node_list,id,num_total);
  repnodeset     = (char*) calloc (num_total,sizeof(char));
  contnodeset(node_list,repnodeset,&nnodeset,num_total);
#if _DEBUG
  printf("nnodeset=%d\n",nnodeset);
  for(i=0;i<num_total;i++){
    fprintf(stderr,"%d node_list=%d id=%d rep=%d\n"
           ,i+1,node_list[i],id[i],repnodeset[i]);
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
      nodeset[k].ngid++;
    }  
  }
  free(repnodeset);
  free(ids);
  free(id);
  free(node_list);
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

/*********************************************************************/
void read_carg_side_exo(int exoid){
/*===*/  
  int i,j,k;
  int error = 0;
  char *repsideset=NULL;
  int *ids=NULL;
  int *id=NULL;
  int *side_list=NULL;
  int *elem_list=NULL;
  int *side_set_list=NULL;
  int *elem_set_list=NULL;
  int tp[4];
  int num_total,numdf,numset;
  float fdum;
  char *cdum;
/*===================================================================*/  
  
/*===*/  

  ids = (int *) calloc(ngeomsetside,sizeof(int));
  ERROR_MALLOC(ids,__LINE__,__func__,__FILE__);
  error = ex_get_side_set_ids (exoid, ids);

#if _DEBUG
  for(i=0;i<ngeomsetside;i++){
    fprintf(stderr,"%d ids=%d\n",i+1,ids[i]);
  }
#endif
/*...................................................................*/  
  
/*numero de nos com restricao*/
  error = ex_inquire (exoid, EX_INQ_SS_ELEM_LEN, &num_total, &fdum,
  cdum);
  side_list     = (int *) calloc(num_total,sizeof(int));
  ERROR_MALLOC(side_list,__LINE__,__func__,__FILE__);
  elem_list     = (int *) calloc(num_total,sizeof(int));
  ERROR_MALLOC(elem_list,__LINE__,__func__,__FILE__);
  id            = (int *) calloc(num_total,sizeof(int));
  ERROR_MALLOC(id,__LINE__,__func__,__FILE__);
  k = 0;
#if _DEBUG
  printf("num_total  %d %f\n",num_total);
#endif
  for (i=0;i<ngeomsetside;i++){
    error     = ex_get_side_set_param (exoid,ids[i],&numset,&numdf);
    elem_set_list = (int*) calloc(numset,sizeof(int));
    ERROR_MALLOC(elem_set_list,__LINE__,__func__,__FILE__);
    side_set_list = (int*) calloc(numset,sizeof(int));
    ERROR_MALLOC(side_set_list,__LINE__,__func__,__FILE__);
    error      = ex_get_side_set (exoid, ids[i]
                                 ,elem_set_list,side_set_list);
#if _DEBUG
    for(j=0;j<numset;j++){
      fprintf(stderr,"id %d elem %d side %d\n",i,elem_set_list[j],side_set_list[j]);
    }
#endif

	  for( j = 0; j < numset; j++){ 
	    side_list[j+k]  = side_set_list[j];
	    elem_list[j+k]  = elem_set_list[j];
	    id[j+k]         = ids[i];
	  }  

    k += numset;
    free(side_set_list); 
    free(elem_set_list); 
  }
#if _DEBUG
  for(i=0;i<num_total;i++){
    fprintf(stderr,"%d elm_list=%d side_list=%d id=%d\n"
          ,i+1,elem_list[i],side_list[i],id[i]);
  }
#endif
  
  insertionsort3Vector(elem_list,side_list,id,num_total);
  repsideset     = (char*) calloc (num_total,sizeof(char));
  ERROR_MALLOC(repsideset,__LINE__,__func__,__FILE__);
  contnodeset(elem_list,repsideset,&nsideset,num_total);
#if _DEBUG
  printf("nsideset=%d\n",nsideset);
  for(i=0;i<num_total;i++){
    fprintf(stderr,"%d elem_list=%d side_list = %d id=%d rep=%d\n"
           ,i+1,elem_list[i],side_list[i],id[i],repsideset[i]);
  }
#endif
  sideset = (SIDESET *) calloc(nsideset,sizeof(SIDESET));
  ERROR_MALLOC(sideset,__LINE__,__func__,__FILE__);
  for(i=0;i<num_total;i++)
    for(j=0;j<MAX_FACE;j++)
      sideset[i].side[j] = 0;

  k = -1;
  for(i=0;i<num_total;i++){
    if(!repsideset[i]){
      k++;
      sideset[k].num                  = elem_list[i];
      sideset[k].side[side_list[i]-1] = id[i];
      sideset[k].gid[id[i]-1]         = 1;
      sideset[k].ngid                 = 1;
    }
    else{  
      sideset[k].side[side_list[i]-1] = id[i];
      nodeset[k].gid[id[i]-1]         = 1;
      sideset[k].ngid++;
    }  
  }
  free(repsideset);
  free(ids);
  free(id);
  free(side_list);
  free(elem_list);
  
#if _DEBUG
  for(i=0;i<nsideset;i++){
    printf("%d elem %d ",i+1,sideset[i].num);
    for(j=0;j<MAX_FACE ;j++)
      printf("face = %d ",sideset[i].side[j]);
    printf("\n");
  }
#endif
//for(i=0;i<nsideset;i++){
//  tp[0]              = sideset[i].side[0];
//  tp[1]              = sideset[i].side[1];
//  tp[2]              = sideset[i].side[2];
//  tp[3]              = sideset[i].side[3];
//  sideset[i].side[0] =  tp[0];
//  sideset[i].side[1] =  tp[3];
//  sideset[i].side[2] =  tp[1];
//  sideset[i].side[3] =  tp[2];
//}
 
/*===================================================================*/  
}
/*********************************************************************/

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
/*********************************************************************/

/*********************************************************************/
void insertionsort2Vector(int *v,int *b,int n)
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
/*********************************************************************/

/*********************************************************************/
void insertionsort3Vector(int *v,int *b1,int *b2,int n)
{
 int i, j, chave,chave1,chave2;
 
 for(j=1; j<n; j++) 
 {
   chave   = v[j];
   chave1  = b1[j];
   chave2  = b2[j];
   i = j-1;
   while(i >= 0 && v[i] > chave)
   {
      v[i+1] = v[i];
     b1[i+1] = b1[i];
     b2[i+1] = b2[i];
     i--;
   }		
   v[i+1]  = chave;
   b1[i+1] = chave1;
   b2[i+1] = chave2;
 }  
}
/*********************************************************************/
