#include<Eglobal.h>
void propnode_malloc_geral(int *nincid,int *incid,int *el 
                          ,long nnode,long numel,int nen
                          ,int*maxgrade){
  long i;
  nincid = (int*) malloc(nnode*sizeof(int));
  mynodegrade(el,nnode,numel,nen,nincid,maxgrade);
/*  fprintf(stderr,"%d\n",*maxgrade);*/
  incid = (int*) malloc(nnode*(*maxgrade)*sizeof(int));
  myelmnicid (el,incid,nincid,nnode,numel,nen,*maxgrade);
  
/*  for(i=0;i<nnode;i++)*/
/*    fprintf(stderr,"no=%d maxgrade=%d\n",i+1,nincid[i]);*/
  
  pnode.nincid = nincid;
  pnode.incid  = incid;
}




/*********************************************************************/
/*                                                                   */  
/* NODEGRADE: Função do arquivo Colormesh do George. Determina o num */  
/* ro maximo de apariçoes dos nos na malha.                          */  
/* ----------------------------------------------------------------- */  
/* Parametros de entrada :                                           */  
/* ----------------------------------------------------------------- */  
/* ix0    - conectivida da malha(sem material)                       */  
/* nnode  - numero de nos                                            */  
/* numel  - numero de elementos                                      */  
/* nen    - numero de nos por elementos                              */  
/* ----------------------------------------------------------------- */  
/* Parametros de saida :                                             */  
/* ----------------------------------------------------------------- */  
/* nincid   - numero de incidencias por nos                          */  
/* maxgrade - numero de incidencias maximo na malha                  */  
/*********************************************************************/
void mynodegrade(int *el,long nnode,long numel,int nen,int *nincid
                ,int *maxgrade){
/*===*/   
/*auxiliar*/
  long i,k;
  int j;
  int node,grade;
/*===================================================================*/   

/*===*/   
/*...*/
  *maxgrade = 0;
   grade    = 0;
/*... zera o vetor*/
  for(i=0;i<nnode;i++)
    nincid[i] = 0;
/*...................................................................*/

/*...*/
  for(i=0;i<numel;i++){
    for(j=0;j<nen;j++){
                 k  = i*nen + j;
              node  = el[k] - 1;
      nincid[node] += 1;
             grade  = nincid[node];
      if( grade > *maxgrade)
        *maxgrade = grade;
    }
  }
/*===================================================================*/   
}
/*********************************************************************/

/*********************************************************************/
/* ELMINID  : Função do arquivo Colormesh do George. Determina o nume*/ 
/* ro maximo de apariçoes dos nos na malha.                          */ 
/* ----------------------------------------------------------------- */ 
/* Parametros de entrada :                                           */ 
/* ----------------------------------------------------------------- */ 
/* ix0    - conectivida da malha(sem material)                       */ 
/* nnode  - numero de nos                                            */ 
/* numel  - numero de elementos                                      */ 
/* nen    - numero de nos por elementos                              */ 
/* men    - numero de nos por elementos                              */ 
/* nincid   - numero de incidencias por nos                          */ 
/* maxgrade - numero de incidencias maximo na malha                  */ 
/* ----------------------------------------------------------------- */ 
/* Parametros de saida :                                             */ 
/* ----------------------------------------------------------------- */ 
/* incid    - incidencia dos lementos por no                         */ 
/*********************************************************************/
void myelmnicid(int *el,int *incid,int *nincid,long nnode,long numel
               ,int nen,int maxgrade){
/*===*/   
/*... auxiliar*/
  int  node,ipos;
  long i;
  long k;
  int  j;
/*===================================================================*/   

/*===*/   
/*... zera o vetor*/
  for(i=0;i<nnode;i++){
    nincid[i] = 0;
  }
  for(i=0;i<nnode*maxgrade;i++){
    incid[i] = 0;
  }
/*...................................................................*/

/*...*/
  for(i=0;i<numel;i++){
    for(j=0;j<nen;j++){
                 k  = i*nen + j;
              node  = el[k] - 1;
      nincid[node] += 1;
             ipos   = nincid[node]-1;
	         k  = node*maxgrade + ipos;
	   incid[k] = i+1; 
    }
  }
/*...................................................................*/
/*===================================================================*/   
}
/*********************************************************************/
