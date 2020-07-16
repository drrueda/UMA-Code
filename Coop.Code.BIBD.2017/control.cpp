#include "random.h"
#include <windows.h>
#include <stdio.h>   /* para printf, FILE, fopen, fclose */
#include <stdlib.h>  /* para exit */
#include <string.h> /* para strdup, memcpy */
#include <limits.h>
#include <float.h>
#include <time.h>
using namespace std;


typedef struct para{
          short int v,b,k,r,lmd;
          short int cycles,ncand,selec,pres,na,sinic,talg,pf,pc,posi,xId,nSolMM;
          float TEval,EvalAlgo;
          char lp[100],tm[15],Migra,Reemp;
        };
typedef struct Algos{
         short int id; 
         char nom[25];
         int ls,gdo,seed,model,nejec; 
       };                             
                                      
typedef struct solution{
         short int gen[100][100];
         short int fit,tabu,model,alg;
         float time,maxEval,minEval;
         };
int xxsw=1,t=7,ttt=0,q=1;
void lee_param(int argc, char* argv[],para &p)
{
  char xxx[5];
  strcpy(p.lp,argv[1]);
  p.v=atoi(argv[1]);
  strcat(p.lp," ");
  strcat(p.lp,argv[2]);
  p.b=atoi(argv[2]);
  strcat(p.lp," ");
  strcat(p.lp,argv[3]);
  p.r=atoi(argv[3]);
  strcat(p.lp," ");
  strcat(p.lp,argv[4]);
  p.k=atoi(argv[4]);
  strcat(p.lp," ");
  strcat(p.lp,argv[5]);
  p.lmd=atoi(argv[5]);
  
  p.cycles=atoi(argv[6]); 
  p.TEval=atof(argv[7]); 
  p.ncand=atoi(argv[8]);
  p.pres=atoi(argv[9]);
  p.na=atoi(argv[10]);
  p.selec=atoi(argv[11]);
  p.sinic=atoi(argv[12]);
  strcpy(xxx,argv[13]);
  p.Migra=xxx[0];
  strcpy(xxx,argv[14]);
  p.Reemp=xxx[0];
  p.pf=2; // For the breaking of symmetries keep the many rows fixed
  p.pc=2; // For the breaking of symmetries keep the many col fixed
}
   

   
int algoritmos(Algos *ma,Algos &inicio)
{
  int cn=0;
  FILE *ff;
  ff=fopen("algos.txt","rt");
  if(!ff)
   {
      printf ("\nError Opening Algorithm Pool File::: algos.txt \n");
      system("pause");
      exit(1);
   }
  fscanf(ff,"%d",&inicio.id);
  if(!feof(ff))
   { 
     fscanf(ff,"%s %d %d",&inicio.nom,&inicio.ls,&inicio.model);
     inicio.seed=0;
   }
  while(!feof(ff))
   {                              // nom = Nombre del Algoritmo a Utilizar
     fscanf(ff,"%d",&ma[cn].id);  
     if(!feof(ff))
      {                      // Nombre de Algoritmo    Tipo de Tecnica   Tipo modeloModelo  
        fscanf(ff,"%s %d %d",&ma[cn].nom,&ma[cn].ls,&ma[cn].model);
        ma[cn].seed=0;
        ma[cn].gdo=ma[cn].nejec=0;
        ma[cn].id=cn;
        cn++;
      }
   }
  fclose(ff);
  return cn;
}
      
float elapsed_time(clock_t t_start,clock_t t_end)
{ /*  Evaluate CPU seconds */
    float xx;

    xx = (float)(t_end - t_start)/CLOCKS_PER_SEC;
    return(xx);
}
void orden(solution *cand,para p)
{
  solution aux;
  int i,j,pos;
  for(i=0;i<p.ncand;i++)
  {
    aux=cand[i];
    pos=i;
    for(j=i+1;j<p.ncand;j++)
      if(cand[j].fit<aux.fit)
       {
         pos=j;
         aux=cand[j];
       }
    cand[pos]=cand[i];
    cand[i]=aux;
  }

}
void c_simultaneos(Algos *Tpool,Algos *Spool,para p)
{
  int *per=RandPerm(p.talg),kk=-1;
  for(int i=0;i<p.na;i++)
   {
   	 if(kk>=p.talg-1)
   	   kk=-1;
     kk++;
     Spool[i]=Tpool[per[kk]];
   }
  free(per);
}
void mejorA(Algos *pool,para p)
{
  Algos aux;
  int i,j,pos;
  for(i=0;i<p.talg;i++)
  {
    aux=pool[i];
    pos=i;
    for(j=i+1;j<p.talg;j++)
      if(pool[j].gdo>aux.gdo)
       {
         pos=j;
         aux=pool[j];
       }
    pool[pos]=pool[i];
    pool[i]=aux;
  }
}




void MajorDiverse(solution *candidatos,para p,solution *CandMay)
{
  typedef void (WINAPI *hFunc)(solution*,para);
  hFunc DtoB;
  HINSTANCE pLib;       
  pLib = LoadLibrary("GestorUtil.dll");
  if(!pLib)
    {
      printf("\nFailed to Take Pointer to the DLL\n");
      FreeLibrary(pLib);
      system("pause");
   }
  
  int i,j,k=0,pmay=-1;
  int xsum=0,xsin=0,cd;
  float xmay=-1;
  
  solution bCand,tmp[30];
  //  Convert the pool of candidates to the Binary model
  DtoB = (void (__stdcall *) (solution*,para)) GetProcAddress(pLib,"DecModelToBinModel");
  for(int xi=0;xi<p.ncand;xi++)
  {
  	bCand=candidatos[xi];
    if(candidatos[xi].model==8||candidatos[xi].model==9)
      DtoB(&bCand,p);
   	tmp[xi]=bCand;
  }
  FreeLibrary(pLib);
  for(i=0;i<p.ncand;i++)
    {
   	  xsum=0;
   	  k=0;
      for(j=0;j<p.ncand;j++)
        if(i!=j)
         {
       	   cd=0;
       	   for(int xfi=0;xfi<p.v;xfi++)
            for(int xco=0;xco<p.b;xco++)
             if(tmp[i].gen[xfi][xco]!=tmp[j].gen[xfi][xco]) // We calculate the Distance between all the Candidates
   	           cd++;
  	       xsum+=cd;
  	       k++;
         }
      if(xsum/k>xmay) // We're going to stay with the candidate who has the highest average distances
      {
      	xmay=xsum/k;
      	(*CandMay)=tmp[i];
      	pmay=i;
      } 
    } 
}
void candidato(solution *pool,solution *xcand,para p) // Assignment of the candidate that is delivered to the algorithm that goes into operation
{
  int x;
  switch(p.Migra)
  {
      case 'R': // Assign Randomly
              if(q)
                  (*xcand)=pool[0]; // Sometimes the best
              else
                (*xcand)=pool[URand(p.ncand)];
              q=!q;
              break;
       default: x=URand(p.ncand); // For Default:  Assign Randomly
               (*xcand)=pool[x];
            
              
  }
}



int improve(solution *candidatos,solution &xcand,para p,int xpos,solution *tmp)
{
    int xsi,si=1,xi,j,tcol=p.b;
    solution bCand,dCand,xIndiv;
    typedef void (WINAPI *hFunc)(solution*,para);
    typedef float (WINAPI *hFunc2)(solution*,para,float*);
    hFunc DtoB;
    hFunc2 DG;
    HINSTANCE pLib;       
    pLib = LoadLibrary("GestorUtil.dll");
    if(!pLib)
     {
      printf("\nFailed to Take Pointer to the DLL\n");
      FreeLibrary(pLib);
      system("pause");
     }
    //  Convertir el pool de candidatos al modelo Binario
    DtoB = (void (__stdcall *) (solution*,para)) GetProcAddress(pLib,"DecModelToBinModel");
    DG= (float (__stdcall *) (solution*,para,float*)) GetProcAddress(pLib,"diverse_gene");
    for(xi=0;xi<p.ncand;xi++)
     {
     	bCand=candidatos[xi];
        if(candidatos[xi].model==8||candidatos[xi].model==9)
       	   DtoB(&bCand,p);
    	tmp[xi]=bCand;
     }
    bCand=xcand;
    if(xcand.model==8||xcand.model==9)
      DtoB(&bCand,p);
    // Verify if there is any solution equal to the contribution
    si=0;
    for(xi=0;xi<p.ncand;xi++)
     if(xcand.fit==tmp[xi].fit)
       {
         j=0;
         for(int xfi=0;xfi<p.v;xfi++)
          {
            int xsum=0;
            for(int xco=0;xco<p.b;xco++)
             if(xcand.gen[xfi][xco]==candidatos[xi].gen[xfi][xco])
                xsum++;
            if(xsum==p.b)
              j++;
          }
         if(j==p.v)
            si++;
       }
           // Here we apply the Migration policy
    		// diverse-gene
        if(!si)  // If there is a Contribution (We verify the policy of Entropia)
          {
  	        xsi=1;
  	        switch(p.Reemp)
  	         {
                case 'D': // We replace if it diversifies the Population
                         xsi=0;
  	                     if(candidatos[0].fit>xcand.fit)
  	                       xsi=1;
                         else
                           {
		                      float xxsum=0.0,xd,sin;
       	                      for(xi=0;xi<p.ncand-1;xi++)
   	                            for(int xfi=0;xfi<p.v;xfi++)
                                  for(int xco=0;xco<p.b;xco++)
                                    if(tmp[xi].gen[xfi][xco]!=bCand.gen[xfi][xco])
   	                                  xxsum++;
                              xxsum=xxsum/(p.ncand-1);
   	                          xd=DG(tmp,p,&sin);
   	                          if(xd<(xxsum+sin)) // There is a contribution that diversifies the population
   	                              xsi=2;
    	                    }
                          if(xsi)
                           {
             	             candidatos[p.ncand-1]=xcand;
   	                         orden(candidatos,p);
   	                       }
                         break;
               default: candidatos[p.ncand-1]=xcand;
            }
        }
     FreeLibrary(pLib);
     return si; // 0 = There is a contribution ::::: >=1  Not There is a contribution
    
}

void change_algo(Algos *tabla,Algos *Spool,int pos,para *p)
{
   int *per=RandPerm(p->talg),sw=1,xPos;
   Algos may[15];
   switch(p->selec)
    {
 
      case 4: //  Selection for Ring
	          if(p->xId==(p->talg-1))  
                p->xId=0;
              else
                p->xId++;
              Spool[pos]=tabla[p->xId];
              break;
      case 5: //  Selection for BROAD-CAST
	          if(p->xId==(p->talg-1))  
                p->xId=0;
              else
                p->xId++;
              Spool[pos]=tabla[p->xId];
              break;
      default: // Selection of the Replacement Algorithm in random form
              sw=URand(p->talg);
              Spool[pos]=tabla[sw];
    }
  free(per);
}



void NSBPtoSBP(solution &ind,para p)
{
   int fi,col,c0;
   if(p.pf<=0)
     return;
   for(fi=0;fi<p.pf;fi++)
     for(col=0;col<p.b;col++)
       ind.gen[fi][col]=0;
   // We place r ones in the first cells of row 1 (One) of the matrix       
   for(col=0;col<p.r;col++)
    ind.gen[0][col]=1;
   if(p.pf==2)
     {
       // We put [Lamda] ones in the first cells of row 2 (Two) of the matrix        
       for(col=0;col<p.lmd;col++)
         ind.gen[1][col]=1; 
       // We put | [Lamda] -r | ones in the last cells of row 2 (Two) of the matrix
       fi=abs(p.r-p.lmd);
       c0=0;
       for(col=p.r;c0<fi;col++)
        {
           ind.gen[1][col]=1;
           c0++;
        }
     }
   if(p.pc>0)
    {
      // We place k ones in the first k cells of Column 1 (One) of the matrix         
      for(fi=p.pf;fi<p.k;fi++)
       {
         if(ind.gen[fi][0]==0)
          {
            ind.gen[fi][0]=1;
            for(col=1;col<p.b;col++)
             if(ind.gen[fi][col]==1)
              {
                ind.gen[fi][col]=0;
                break;
              }
          }
        }
       // We place | k-v | zeros in the Last cells of Column 1 (One) of the matrix       
       for(fi=p.k;fi<p.v;fi++)
        {
          if(ind.gen[fi][0]==1)
           {
             ind.gen[fi][0]=0;
             for(col=1;col<p.b;col++)
              if(ind.gen[fi][col]==0)
               {
                 ind.gen[fi][col]=1;
                 break;
               }
           }
        }
       if(p.pc==2)
         {
            c0=0;
            for(fi=0;fi<p.pf;fi++)
              c0+=ind.gen[fi][1];
            c0=abs(p.k-c0);
            for(fi=p.v-c0;fi<p.v;fi++)
             {
               if(ind.gen[fi][1]==0)
                {
                  ind.gen[fi][1]=1;
                  for(col=2;col<p.b;col++)
                   if(ind.gen[fi][col]==1)
                    {
                      ind.gen[fi][col]=0;
                      break;
                    }
                }
             }
            // We place | k-v | zeros in the Last cells of Column 1 (One) of the matrix
            for(fi=p.pf;fi<(p.v-c0);fi++)
             {
               if(ind.gen[fi][1]==1)
                {
                  ind.gen[fi][1]=0;
                  for(col=2;col<p.b;col++)
                   if(ind.gen[fi][col]==0)
                     {
                        ind.gen[fi][col]=1;
                        break;
                     }
                }
             }
         }
     }
   ind.model=2;
}
void NSBDectoSBDec(solution &ind,para p)
{
  int per[120],col,j;
  for(j=0;j<p.b;j++)
    per[j]=j;
  for(j=0;j<p.r;j++)
    per[ind.gen[0][j]]=-1;
  for(col=0;col<p.lmd;col++)
    ind.gen[1][col]=ind.gen[0][col];
  j=p.lmd;
  for(col=0;j<p.r;col++)
   if(per[col]!=-1)
    {
       ind.gen[1][j]=per[col];
       j++;
    }
  ind.model=9;
}

void NSBDtoSBD(solution &ind,para p)
{
   int fi,col,c0;
   if(p.pc<=0)
     return;
   for(fi=0;fi<p.v;fi++)
     for(col=0;col<p.pc;col++)
       ind.gen[fi][col]=0;
    // We put k ones in the first cells of column 1 (One) of the matrix         
   for(fi=0;fi<p.k;fi++)
    ind.gen[fi][0]=1;
   for(fi=0;fi<p.pf;fi++)
     ind.gen[fi][1]=1;
  
   if(p.pc==2)
    {
     // We place k ones in the Last [k-p-> rows] cells of Column 2 (Two) of the matrix  
     c0=0; 
     for(fi=0;fi<p.pf;fi++)
      c0+=ind.gen[fi][1];
     c0=abs(p.k-c0);
     for(fi=p.v-c0;fi<p.v;fi++)
       ind.gen[fi][1]=1;
    }
   if(p.pf>0)
    {
      for(col=p.pc;col<p.r;col++)
       {
         if(ind.gen[0][col]==0)
          {
            ind.gen[0][col]=1;
            for(fi=1;fi<p.v;fi++)
             if(ind.gen[fi][col]==1)
              {
                ind.gen[fi][col]=0;
                break;
              }
          }
        }
       for(col=p.r;col<p.b;col++)
        {
          if(ind.gen[0][col]==1)
           {
             ind.gen[0][col]=0;
             for(fi=1;fi<p.v;fi++)
              if(ind.gen[fi][col]==0)
               {
                 ind.gen[fi][col]=1;
                 break;
               }
           }
        }
       if(p.pf==2)
         {
            c0=0;
            for(col=0;col<p.pc;col++)
              c0+=ind.gen[1][col];
            c0=abs(p.r-c0);
            for(col=p.b-c0;col<p.b;col++)
             {
               if(ind.gen[1][col]==0)
                {
                  ind.gen[1][col]=1;
                  for(fi=p.pf;fi<p.v;fi++)
                   if(ind.gen[fi][col]==1)
                    {
                      ind.gen[fi][col]=0;
                      break;
                    }
                }
             }
            for(col=p.pc;col<(p.b-c0);col++)
             {
               if(ind.gen[1][col]==1)
                {
                  ind.gen[1][col]=0;
                  for(fi=p.pf;fi<p.v;fi++)
                   if(ind.gen[fi][col]==0)
                     {
                        ind.gen[fi][col]=1;
                        break;
                     }
                }
             }
         }
     }
    ind.model=4;
}

void getsol(para p,solution *xcand,Algos xAlgor)
{
   FILE *f;
   int bas,j,col,sw,tf,tcol=p.b;
   typedef int (WINAPI *hFunc)(solution,para);
   hFunc getFit;
   HINSTANCE pLib;       
   pLib = LoadLibrary("GestorUtil.dll");
   if(!pLib)
     {
      printf("\nFailed to Take Pointer to the DLL\n");
      FreeLibrary(pLib);
      system("pause");
     }
   f=fopen("mejor.txt","r");
   if(!f)
     {
       printf ("\nError No Algorithm Solution\n");
       system("pause");
       exit(1);
     } // We will take the solution provided from the File mejor.txt
   fscanf(f,"%f",&(xcand->time)); 
   fscanf(f,"%f",&(xcand->maxEval)); 
   fscanf(f,"%f",&(xcand->minEval)); 
   fscanf(f,"%d",&bas); // Fitness
   sw=tf=0;
   (xcand->fit)=bas;
   if(xAlgor.model==8||xAlgor.model==9)
     tcol=p.r;
   for(j=0;j<p.v&&!sw;j++)
    {
     for(col=0;col<tcol&&!sw;col++)
      {
        fscanf(f,"%d",&bas);
        if(bas!=-1)
          xcand->gen[j][col]=bas;
        else
          sw=1;
      }
     if(!sw)
       tf++;
    }
   (xcand->fit)=(int)FLT_MAX;
   getFit= (int (__stdcall *) (solution,para)) GetProcAddress(pLib,"evaluar");
   if(tf>=p.v)
     {
       (xcand->model)=xAlgor.model;
       (xcand->fit)=getFit(*xcand,p);
       (xcand->alg)=xAlgor.ls;
     }
   fclose(f);
}
void WriteOne(solution &indiv,para p)
{
     FILE *f;
     char log[]="temp.txt";
     int tcol=p.b;
     f=fopen(log,"w");
     if(!f)
       {
         printf ("\n!!!!!Error When creating algorithm cooperation solution!!!!!!\n");
         system("pause");
         exit(0);
      }
     fprintf(f,"%d\n",p.na);
     if(indiv.model==8||indiv.model==9)
       tcol=p.r;
     for(int xj=0;xj<p.v;xj++)
        for(int col=0;col<tcol;col++)
          fprintf(f," %d",indiv.gen[xj][col]);
     fclose(f);
}

void WriteAll(solution *candidate,para p)
{
     FILE *f;
     char log[]="temp.txt";
     int tcol=p.b;
     f=fopen(log,"w");
     if(!f)
       {
         printf ("\n!!!!!Error When creating algorithm cooperation solution!!!!! \n");
         system("pause");
         exit(0);
      }
     fprintf(f,"%d\n",p.nSolMM);
     if(candidate[0].model==8||candidate[0].model==9)
       tcol=p.r;
     for(int xi=0;xi<p.nSolMM;xi++)
       {
         for(int xj=0;xj<p.v;xj++)
          for(int col=0;col<tcol;col++)
            fprintf(f," %d",candidate[xi].gen[xj][col]);
            fprintf(f,"\n");
         }
     fclose(f);
}

void WriteSol(solution &indiv,para p)
{
     FILE *f;
     char log[]="mejor.txt";
     int tcol=p.b;
     f=fopen(log,"w");
     if(!f)
       {
         printf ("\n!!!!!Error When creating algorithm cooperation solution!!!!!!!\n");
         system("pause");
         exit(0);
      }
     fprintf(f,"%d  %d   %d   %d",p.b,p.b,p.b,100000);
     if(indiv.model==8||indiv.model==9)
       tcol=p.r;
     for(int xj=0;xj<p.v;xj++)
        for(int col=0;col<tcol;col++)
          fprintf(f," %d",indiv.gen[xj][col]);
     fclose(f);
}
void impCand(solution cand,para p)
{
     int i,j;
     printf("\nCandidato Fit: %d\n",cand.fit);
     for(int i=0;i<p.v;i++){
      for(int j=0;j<p.b;j++)
        printf(" %d ",cand.gen[i][j]);
       printf("\n");}
       printf("\n");
     
 }
int main(int argc, char *argv[])
{
   FILE* f,*fs,*fl;
   float TEVAL=0.0,TNodos=0.0;
   para p;
   long int xId,xInic,xFin,Truns=0;
   int ta,CYCLES=0,xnIndiv,sinmejora=0;
   solution candidate[30],xcand,xindiv,indiv,tempo[30];
   char comand[200],major[]="mejor.txt",log[]="temp.txt",Out[]="trace.txt";
   HINSTANCE pLib;      
   
   pLib = LoadLibrary("GestorUtil.dll");
   if(!pLib)
     {
      printf("\nFailed to Take Pointer to the DLL\n");
      FreeLibrary(pLib);
      system("pause");
     }
   typedef void (WINAPI *hFunc)(para,solution*,Algos*);
   typedef void (WINAPI *hFunc2)(para,Algos,char *);
   typedef void (WINAPI *hFunc4)(solution *,para);
   hFunc Cpool;
   hFunc2 Go;
   hFunc4 RCol,RFila,DtoB,BtoD;
    Algos ma[10],inicio,pool[10];
    // For take time
    clock_t start,end;
    p.talg=algoritmos(ma,inicio);
    ta=p.talg;
    p.xId=-1;
    if(p.talg==0)
      {
        printf ("\nError THERE ARE NO Algorithms in the Pool\n");
        system("pause");
        exit(1);
      }
    lee_param(argc,argv,p);

    c_simultaneos(ma,pool,p);
    p.EvalAlgo=((p.TEval/p.cycles)/p.na);
    Cpool= (void (__stdcall *) (para,solution*,Algos*)) GetProcAddress(pLib,"c_pool");
    Go= (void (__stdcall *) (para,Algos,char *)) GetProcAddress(pLib,"comando");
    DtoB = (void (__stdcall *) (solution*,para)) GetProcAddress(pLib,"DecModelToBinModel");
    BtoD = (void (__stdcall *) (solution*,para)) GetProcAddress(pLib,"BinModelToDecModel");
    RCol= (void (__stdcall *) (solution*,para)) GetProcAddress(pLib,"ReparaCol");
    RFila= (void (__stdcall *) (solution*,para)) GetProcAddress(pLib,"ReparaFila");
    Cpool(p,candidate,&inicio);
    fs=fopen(Out,"wt");
    while(CYCLES<p.cycles&&TEVAL<=p.TEval)
     {  
        orden(candidate,p);
        candidato(candidate,&xindiv,p);
        for(int j=0;j<p.na;j++)
         {
            p.posi=j;
            switch(pool[j].ls)
             {
                 case 4: 
                         if(p.selec==5) 
                               indiv=xindiv;
                         else
                            candidato(candidate,&indiv,p);
                         if(indiv.model==3||indiv.model==4)
                           RFila(&indiv,p);
                         else
                           if(indiv.model==8||indiv.model==9)
                             DtoB(&indiv,p);
                         WriteOne(indiv,p);
                         break;
                 case 7: p.nSolMM=0;
                         for(int t=0;t<p.ncand;t++)
                          {
                            indiv=candidate[t];
                            if(indiv.fit>0)
                             {
                               if(indiv.model==3||indiv.model==4)
                                 RFila(&indiv,p);
                               else
                                 if(indiv.model==8||indiv.model==9)
                                   DtoB(&indiv,p);
                               tempo[p.nSolMM]=indiv;
                               p.nSolMM++;
                             }
                          }
                         WriteAll(tempo,p);
                         break;
               default: 
                         if(p.selec==5) 
                           indiv=xindiv;
                         else
                           candidato(candidate,&indiv,p);
                         if(indiv.model==3||indiv.model==4)
                           RFila(&indiv,p);
                         else
                          if(indiv.model==8||indiv.model==9)
                             DtoB(&indiv,p);
                         WriteOne(indiv,p);
             }
            ma[pool[j].id].seed++;
            pool[j].seed=ma[pool[j].id].seed;
            Go(p,pool[j],comand);
            printf("\n[%d] %s",j,comand);
            // Here we will take the time
            start = clock();
            //system("pause");
            system(comand); // Run Agent
            end = clock();           
            // End take the time
            TEVAL+=p.EvalAlgo; 
            getsol(p,&xcand,pool[j]);
            //printf("\nYa fue Tomar Solucion");
            TNodos+=(float)p.EvalAlgo;
            fprintf(fs,"%d %d %10.0f %10.0f %5d %15.2f\n",Truns,pool[j].ls,TNodos,TNodos-xcand.minEval,xcand.fit,elapsed_time(start,end));
            ma[pool[j].id].nejec++;
            if(!improve(candidate,xcand,p,j,tempo))
               ma[pool[j].id].gdo++;
            else
              sinmejora++;
            if(sinmejora>2&&candidate[0].fit>0)
              {
                WriteSol(candidate[0],p); // Update candidate pool
                Cpool(p,candidate,&inicio);
                sinmejora=0;
              }
            change_algo(ma,pool,j,&p);
            Truns++;
         }
        CYCLES++;
     }
     fclose(fs);
     f=fopen("runs.txt","wt");
     for(int xi=0;xi<p.ncand;xi++)
       {
         fprintf(f,"%d %d %d ",xi+1,candidate[xi].fit,candidate[xi].alg);
         Truns=p.b;
         if(candidate[xi].model==9||candidate[xi].model==8)
           Truns=p.r;
         for(int i=0;i<p.v;i++)
            for(int j=0;j<Truns;j++)
               fprintf(f," %d ",candidate[xi].gen[i][j]);
         fprintf(f,"\n");
       }
     fclose(f);
     f=fopen("aportes.txt","wt");
     for(int xi=0;xi<p.talg;xi++)
         fprintf(f,"%d %d %d %d\n",ma[xi].ls,ma[xi].gdo,ma[xi].model,ma[xi].nejec);
      fclose(f);
  // system("pause");
  FreeLibrary(pLib);
}
