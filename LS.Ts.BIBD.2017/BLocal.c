/*
 * NOMBRE:    BLocal.c
 * AUTOR:     David Rodriguez
 * FECHA:     01-10-08
 * PROPOSITO: modulo para las operaciones de Busqueda Local.
 *
 */

#include <stdio.h>   	/* para printf */
#include <stdlib.h>  	/* para exit */
#include <windows.h>
#include <values.h>
//#include <dos.h>
//#include <time.h>
#include "es.h"
#include "esinitp.h"
#include "util.h"	/* TRUE, FALSE, MAXFLOAT */ 
#include "random.h"	/* para URand01 */
#include "esprob.h"
#include "esreemp.h"
#include "esindiv.h"
#include "espop.h"
#include "BLocal.h"


#ifdef __PARALELIZACION_GLOBAL__
#include "esthread.h"
#endif
clock_t st;
float cpu;
int kxSeed=0;
float convert_time(clock_t t_start,clock_t t_end)
{ 
    float xx;

    xx = (float)(t_end - t_start)/CLOCKS_PER_SEC;
    return(xx);
}
long puntos[]={5000000,
7500000,
10000000,
12500000,
15000000,
17500000,
20000000,
22500000,
25000000,
27500000,
30000000,
32500000,
35000000,
37500000,
40000000,
42500000,
45000000,
47500000,
50000000,
52500000,
55000000,
57500000,
60000000,
62500000,
65000000,
67500000,
70000000,
72500000,
75000000,
77500000,
80000000,
82500000,
85000000,
87500000,
90000000,
92500000,
95000000,
97500000,
100000000,
102500000,
105000000,
107500000,
110000000,
112500000,
115000000,
117500000,
120000000,
122500000,
125000000,
127500000,
130000000,
132500000,
135000000,
137500000,
140000000,
142500000,
145000000,
147500000,
150000000};

///////////////////////////////////////////////////////////////////////////////////////
// Calcula el Tiempo transcurrido en Segundos de CPU
///////////////////////////////////////////////////////////////////////////////////////
float elapsed_time(clock_t t_start,clock_t t_end)
{ /*  Evaluate CPU seconds */
    float xx;

    xx = (float)(t_end - t_start)/CLOCKS_PER_SEC;
    return(xx);
}
///////////////////////////////////////////////////////////////////////////////////////
// Leer Solucion Colaborativa
///////////////////////////////////////////////////////////////////////////////////////
void GetSolution(Individuo* i, ProblemInfo *p,char *xarch)
{
   FILE *fpP;
   int k,xpos;
   fpP=fopen(xarch,"rt");
   if(!fpP)
     {
         printf("\n!!!!!Error taking Collaborative Solution!!!!!!");
         system("pause");
         return;
     }
   fscanf(fpP,"%d",&xpos); //Lee 1 valores ::: De momento no tiene Importancia
   k=0;
   if(xpos==-1)
     {
       printf("\n!!!!!Error taking Collaborative Solution!!!!!!\n");
       system("pause");
       exit(0);
    }
   for(int fi=0;fi<p->v*p->b;fi++)
    {
       fscanf(fpP,"%d",&xpos);
       if(xpos==-1)
          break;
       else
          {
            i->genoma[fi]=xpos;
            k++;
          }
    }
  fclose(fpP);
  if(k<p->v*p->b)
     {
        printf("\n!!!!!!Fatal Error!!!!!");
        system("pause");
        exit(0);
     }
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//void coopera(solution &s,para p)
//{
//     int xpos,gen[MAX_VAL][MAX_VAL],kyi[MAX_VAL],tf=0,fi,col,tlmd,tk,siva,j;
//     xpos=200;
//     for(fi=0;fi<p.v;fi++)
//       for(col=0;col<p.b;col++)
//           gen[fi][col]=s.gen[fi][col];
//   j=1;
//   for(fi=1;fi<p.v;fi++)
//     {
//       for(col=0;col<p.b;col++)
//         kyi[col]=gen[fi][col];
//       siva=1;
//       for(tf=0;tf<j&&siva;tf++)
//        {
//           tlmd=0;
//           for(col=0;col<p.b;col++)
//             tlmd+=gen[tf][col]*kyi[col];
//           if(tlmd!=p.lmd)
//             siva=0;
//        }
//     if(siva)
//      {
//        for(col=0;col<p.b;col++)
//         gen[j][col]=kyi[col];
//        j++;
//      }
//   }  
//  for(fi=0;fi<j;fi++)
//    for(col=0;col<p.b;col++)
//      s.gen[fi][col]=gen[fi][col];
//  s.tl=j;
// // system("pause");    
//}
////////////////////////////////////////////////////
//
//void rand_row(solution &s,para p)
//{
//   int *vec,k,j;
//   vec=(int *)malloc(sizeof(int)*(p.b));
//   for(j=0;j<p.b;j++)
//    vec[j]=0;
//   j=0;
//   while(j<p.r)
//    {
//      k=URand(p.b);
//      if(vec[k]==0)
//       { 
//         vec[k]=1;
//         j++;
//       }
//    }
//   for(j=0;j<p.b;j++)
//     s.xi[j]=vec[j];
// free(vec);
//}
//int eval_fit(solution s, para p)
//{
//    int j,k,r,fitness=0;
//    int fila[200],col[200];
//    for (j=0; j<p.v; j++)
//       fila[j]=0;
//    for (j=0; j<p.b; j++)
//       col[j]=0;
//    for (j=0; j<p.v; j++)
//      for(k=0;k<p.b;k++)
//        {
//           fila[j]+=s.gen[j][k];
//           col[k]+=s.gen[j][k];
//        }
//    for (j=0; j<p.v; j++)
//      for(k=j+1;k<p.v;k++)
//        {
//           int xs=0;
//          for(r=0;r<p.b;r++)
//            xs+=s.gen[j][r]*s.gen[k][r];
//          fitness+=abs(xs-p.lmd);
//        }
//    for (j=0; j<p.v; j++)
//       fitness+=abs(fila[j]-p.r);
//    for (j=0; j<p.b; j++)
//      {
//        fitness+=abs(col[j]-p.k);
//      }
//    return fitness;
//}
//
//int producto(solution s,para p)
//{
//    int tf,col,siva=1,tk,tlmd;
//    for(tf=0;tf<s.tl&&siva;tf++)
//        {
//           tlmd=0;
//           for(col=0;col<p.b;col++)
//             tlmd+=s.gen[tf][col]*s.xi[col];
//           if(tlmd!=p.lmd)
//             siva=0;
//        }
//    return siva;
//}
//int nproducto(solution s,para p,int j,int *kyi)
//{
//    int tf,col,siva=0,tk,tlmd;
//    for(tf=0;tf<j;tf++)
//        {
//           tlmd=0;
//           for(col=0;col<p.b;col++)
//             tlmd+=s.gen[tf][col]*kyi[col];
//           if(tlmd==p.lmd)
//             siva++;
//        }
//    return siva;
//}
//int Lproducto(solution &s,para p,int j,int *kyi)
//{
//    int tf,col,tlmd,sum=0;
//    for(tf=0;tf<j;tf++)
//        {
//           tlmd=0;
//           for(col=0;col<p.b;col++)
//             tlmd+=s.gen[tf][col]*kyi[col];
//           if(tlmd<p.lmd)
//             sum++;
//           s.gen[tf][p.b]=tlmd;
//        }
//    return sum;
//}
//
//int Expand_X(solution &s, para p)
//{/* Branch and bound process */
//    int i,sw=1,sum=0;
//    // Verificar si tiene los Unos reglamentarios
//    for (i=0;i<p.b;i++)
//      sum+=s.xi[i];
//    if(sum<p.r)
//      return 1;
//    /* Filling column j */
//    for (i=0;i<p.b;i++) 
//      s.gen[s.tl][i]=s.xi[i]; 
//    s.tl++;
//    
//    /* BIBD found! */
//    if (s.tl==p.v){
//        sw=eval_fit(s,p);
//    }
//  return sw;
//}
//
//int find_row(solution &s, para p,solution &list){
//   int j,fi,col,kyi[MAX_VAL],vec[MAX_VAL],kki[MAX_VAL],xvan,tf,may=-1,mej[MAX_VAL],son=-1;
//   j=s.tl;
//   s.xi[0]=s.xi[1]=-1;
//   //Incializar y determinar cuantos unos quedan por acomodar
//   for(col=0;col<p.b;col++)
//    {
//      kyi[col]=0;
//      vec[col]=0;
//      for(int li=0;li<j;li++)
//        kyi[col]+=s.gen[li][col];
//      kyi[col]=p.k-kyi[col];
//    }
//   int bas=0;
//   //Pruebas aleatorizadas
//   for(col=0;col<p.b;col++)
//     kki[col]=vec[col];
//   xvan=bas;
//   mej[p.b]=(int)FLT_MAX;//100000;
//   for(int jj=0;jj<p.b;jj++)
//    {
//      for(col=0;col<p.b;col++)
//        vec[col]=kki[col];
//      bas=xvan;
//      vec[p.b]=-1;
//      col=jj;
//      int *id=RandPerm(p.b);
//      while(col!=-1&&bas<p.r)
//       {
//        if(vec[id[col]]==0&&kyi[id[col]]!=0)
//          {
//           vec[id[col]]=1;
//           //Probar cuantas filas estan fuera de lamda
//           int desv=Lproducto(s,p,j,vec);
//           tf=0;
//           for(int ji=0;ji<j;ji++)
//            if(s.gen[ji][p.b]>p.lmd)
//               tf++;
//           if(tf)
//             vec[id[col]]=0;
//           if(vec[id[col]])
//            {
//              bas++;
//              vec[p.b]=desv;
//            }
//          }
//         if(col==p.b-1)
//          col=-1;
//         col++;
//         if(col==jj)
//          col=-1;
//       }
//     free(id);
//     if(bas==p.r)
//      {
//        for(int xcol=0;xcol<p.b;xcol++)
//         s.xi[xcol]=vec[xcol];
//        if(producto(s,p))
//         {
//           for(int xcol=0;xcol<p.b;xcol++)
//             mej[xcol]=vec[xcol];
//           may=bas;
//           break;
//         }
//        if(vec[p.b]<mej[p.b])
//          {
//            for(int xcol=0;xcol<=p.b;xcol++)
//               mej[xcol]=vec[xcol];
//             may=bas;
//          }
//      }else
//     if(bas>may)
//      {
//       for(col=0;col<=p.b;col++)
//        mej[col]=vec[col];
//       may=bas;
//      }
//    }
//   bas=may;
//   fi=0;
//   while(bas>0&&bas<p.r&&fi<p.b+2)//&&(float)elapsed_time(tt,clock())>0.5)
//    {
//      int men=(int)FLT_MAX,xpos=0,xx=0;
//      Lproducto(s,p,j,mej);
//      fi++;
//      for(col=0;col<j;col++)
//        if(s.gen[col][p.b]<men)
//          {
//            men=s.gen[col][p.b];
//            xpos=col;
//          }
//      men=0;
//      while(men==0&&xx<p.b+2)
//      {
//        col=URand(p.b);
//        if(mej[col]==0&&kyi[col]!=0&&s.gen[xpos][col]!=0)
//         {
//          mej[col]=1;
//          bas++;
//          men=1;
//        }
//       else
//         xx++;
//      }
//    }
//   if(bas<p.r)
//     {
//       rand_row(s,p);
//       return -2;
//     }
//   for(int xcol=0;xcol<p.b;xcol++)
//       s.xi[xcol]=list.xi[xcol]=mej[xcol];
//}
//int igual(solution &s, para p)
//{
//  int tf,col,siva=0,tlmd;
//  
//  for(tf=0;tf<s.tabu&&!siva;tf++)
//    if(s.gen[tf][0]!=-1)
//     {
//       tlmd=0;
//       for(col=0;col<p.b;col++)
//          tlmd+=s.gen[tf][col]*s.xi[col];
//       if(tlmd>=p.r)     
//         siva=1;
//     }
//  return siva;
//}
//void add_tabu(solution &list, para p)
//{
//  for(int col=0;col<p.b;col++)
//    list.gen[list.tl][col]=list.xi[col];
//  if(list.tl>=MAXI-1)
//    list.tl=-1;
//  list.tl++;
//  
//}
//void suprimir(solution &s, para p,solution &list)
//{
//  if(s.tl<=0)
//   {
//     s.tl=0;
//     return;
//   }
//  int i,sum=0,col,ld,sw=-1;
//  for (i=0;i<p.b;i++)
//      sum+=s.xi[i];
//  if(sum==p.r)
//   {
//     // Finding row conflicting
//     for (i=0;i<s.tl&&sw==-1;i++)
//      {
//         ld=0;
//         for(col=0;col<p.b;col++)
//           ld+=s.gen[i][col]*s.xi[col];
//         if(ld!=p.lmd)
//           sw=i;
//      }
//    
//     if(sw==-1) // Hay una fila conflicto
//       sw=URand(s.tl);
//     for (i=0;i<p.b;i++)
//       list.xi[i]=s.gen[sw][i];
//     if(!igual(list,p))
//      {
//        add_tabu(list,p); // Agregar a la lista Tabu
//        for (i=0;i<p.b;i++)
//          list.xi[i]=s.xi[i];
//        if(!igual(list,p))
//           add_tabu(list,p); // Agregar a la lista Tabu
//        
//      }
//     //Suprimir la fila conflicto
//       for (i=sw;i<s.tl-1;i++)
//         for(col=0;col<p.b;col++)
//           s.gen[i][col]=s.gen[i+1][col];
//       s.tl--;
//     //}
//     
//   } 
//  else
//   {
//     printf("\n!!!! Errorr.... !!!! Numero de unos diferente de r ::: %d != %d",p.r,sum);
//     system("pause");
//     exit(0);
//   }
//}
//
//int iguales(solution &s, para p)
//{
//  int tf,col,siva=0,tlmd;
//  if(producto(s,p))
//    return 0;
//  for(tf=0;tf<s.tl&&!siva;tf++)
//   {
//     tlmd=0;
//     for(col=0;col<p.b;col++)
//         tlmd+=s.gen[tf][col]*s.xi[col];
//     if(tlmd>=p.r)     
//       siva=1;
//   }
//  return siva;
//}
//void completar(solution &s, para p,solution &list)
//{
//    int i,j,k,l,tf,van=-1;
//    for(j=s.tl;j<p.v;j++)
//     {
//       van=-1;
//       for(i=0;i<list.tabu;i++)
//        if(list.gen[i][0]!=-1)
//         {
//           for(k=0;k<p.b;k++)
//             s.xi[k]=list.gen[i][k];
//           if(!igual(s,p))
//             {
//              tf=Lproducto(s,p,j,s.xi);
//              if(tf>van)
//               {
//                 for(k=0;k<p.b;k++)
//                   list.xi[k]=s.xi[k];
//                 van=tf;
//               }
//             }
//          }
//       if(van>0)
//         {
//           for(k=0;k<p.b;k++)
//             s.gen[s.tl][k]=list.xi[k];
//           s.tl++;
//           
//         }
//     }
//}
//void llenar(solution &s, para p)
//{
//   int *vec,k,j;
//   vec=(int *)malloc(sizeof(int)*(p.b));
//   do{
//      for(j=0;j<p.b;j++)
//        vec[j]=0;
//      j=0;
//      while(j<p.r)
//       {
//         k=URand(p.b);
//         if(vec[k]==0)
//          { 
//            vec[k]=1;
//            j++;
//          }
//       }
//      for(j=0;j<p.b;j++)
//         s.gen[s.tl][j]=vec[j];
//      s.tl++;
//   }while(s.tl<p.v);
// free(vec);
//}
//void diversidad(solution &s, para p,solution &list)
//{
//  int j,col;
//  //Agregar a la Lista Tabu
//  for(j=0;j<s.tl;j++)
//   {
//      for(col=0;col<p.b;col++)
//        list.xi[col]=s.gen[j][col];
//      if(URand(2))
//       {
//         if(!igual(list,p))
//           add_tabu(list,p);
//       }
//   }
//  rand_row(s,p);
//}
//void chek(solution &s, para p)
//{
//  int i,fin=1,sinMejora=0;
//  solution lista,major;
//  for(i=0;i<MAXI;i++)
//    lista.gen[i][0]=-1;
//  lista.tl=0;
//  lista.tabu=MAXI;
//  s.tl=0;
//  //srand(p.seed);
//  major.tl=-1;
//  if(p.cual==0)
//   {
//     if(URand(2))
//      {
//        for (i=0;i<p.b;i++)
//         if (i<p.r) 
//          s.xi[i]=1; 
//         else 
//          s.xi[i]=0;
//        Expand_X(s,p);
//        for(i=0;i<p.b;i++)
//          if (i<p.lmd || (i>=p.r && i<2*p.r-p.lmd)) 
//            s.xi[i]=1;
//          else 
//           s.xi[i]=0;
//      }   
//     else
//       rand_row(s,p);
//     Expand_X(s,p);
//   }
//  else
//    coopera(s,p);
//  fin=8;
//  do{
//      int inp=find_row(s,p,lista);
//      if(inp==-2)
//        suprimir(s,p,lista);
//      else
//       {
//         if(!iguales(s,p))
//          {
//            if(producto(s,p))
//             {
//               if(!igual(lista,p))
//                 fin=Expand_X(s,p);
//               else
//                 suprimir(s,p,lista);
//             }
//           if(s.tl>major.tl)
//             {
//               major=s;
//               sinMejora=0;
//             }
//           else
//             sinMejora++;
//          }
//       }  
//      if(s.tl<1||sinMejora>p.b*p.v)
//       {
//         diversidad(s,p,lista);
//         s.tl=0;
//         Expand_X(s,p);
//         sinMejora=0;
//       }
//  }while(s.tl<p.v&&fin&&convert_time(st,clock())<p.tiempo);
//  if(major.tl<p.v)
//    completar(major,p,lista);
//  if(major.tl<p.v)
//     llenar(major,p);
//  s=major;
//}
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//void replacerow(Individuo* i, ProblemInfo *pp)
//{
//    int fi,co,ki;
//    solution xcand;
//    para p;
//    p.v = pp->v;
//    p.b = pp->b;
//    p.r = pp->r;
//    p.k = pp->k;
//    p.lmd=pp->lmd;
//    //printf("\nCrack...\n");
//    
//    //p.tiempo=15.0;
//    p.tiempo=(float)(p.b/4+4);//URand(8)+15;
//    p.seed=0;
//    p.cual=9;
//    xcand.fit=(int)FLT_MAX;
//    //Tomar Solucion anterior
//    ki=0;
//    for(fi=0;fi<p.v;fi++)
//      for(co=0;co<p.b;co++)
//        {
//          xcand.gen[fi][co]=i->genoma[ki];
//          ki++;
//        }
//    ki=0;
//   do{
//       st=clock();
//       p.seed++;
//       chek(xcand,p);
//       ki++;
//    }while(ki<6&&eval_fit(xcand, p)>0);
//    //cpu=convert_time(st,clock());
//    xcand.fit=eval_fit(xcand, p); 
//    // Tomar nueva soluciuon
//    ki=0;
//    for(fi=0;fi<p.v;fi++)
//      for(co=0;co<p.b;co++)
//        {
//          i->genoma[ki]=xcand.gen[fi][co];
//          ki++;
//        }
//    i->fitness=xcand.fit;
//    
//    
//}
///////////////////////////////////////////////////////////////////////////////////////
/////                 Colaborative Diversification Function                    ////////
///////////////////////////////////////////////////////////////////////////////////////
void DColaborative(Individuo* i, ProblemInfo *pp)
{
   int fi,co,ki;
   solution xcand;
   FILE *xxxfp;
   HINSTANCE pLib1;
   typedef void (WINAPI *hFunc1)(int,int,int,int,int,float,int,int);
   hFunc1 hDiversifica;
   pLib1 = LoadLibrary("inicpool.dll");
   if(!pLib1)
     {
       printf("\nError al Tomar Puntero de la DLL\n");
       FreeLibrary(pLib1);
       system("pause");
     }       
    xxxfp = fopen ("tempo.txt","wt");
    Esc_Sol(i,pp,xxxfp);
    fclose(xxxfp);
    hDiversifica=(void (__stdcall *) (int,int,int,int,int,float,int,int)) GetProcAddress(pLib1,"Cdiversification");
    hDiversifica(pp->v,pp->b,pp->r,pp->k,pp->lmd,pp->tColab*1.0,kxSeed++,9);
    FreeLibrary(pLib1);
    // Tomar nueva soluciuon
    GetSolution(i,pp,"tempo.txt");
    i->fitness=EvaluarRestricion(i,pp);
    //Imp_Indiv(i,pp);
    //printf("\nFitness Final: %10.2f\n",i->fitness);
    //system("pause");
}
////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion de evaluacion del Fitness del Individuo
///////////////////////////////////////////////////////////////////////////////////////////
ValorFitness EvaluarRestricion(Individuo* i, ProblemInfo *p){
    int j,k,r,s;
    int fila[100],col[100];
    
    i->fitness = 0;
    
    for (j=0; j<p->v; j++)
       fila[j]=0;
    for (j=0; j<p->b; j++)
       col[j]=0;
    r=0;
    for (j=0; j<p->v; j++)
      for(k=0;k<p->b;k++)
        {
           fila[j]+=i->genoma[r];
           col[k]+=i->genoma[r];
           r++;
        }
    for (j=0; j<p->v; j++)
      for(k=j+1;k<p->v;k++)
        {
           s=0;
          for(r=0;r<p->b;r++)
            s+=i->genoma[(j)*p->b+r]*i->genoma[(k)*p->b+r];
          i->fitness+=abs(s-p->lmd);
        }
    for (j=0; j<p->v; j++)
       i->fitness+=abs(fila[j]-p->r);
    for (j=0; j<p->b; j++)
       i->fitness+=abs(col[j]-p->k);
    return i->fitness;
}/*END*/

////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion de evaluacion del Fitness del Individuo
///////////////////////////////////////////////////////////////////////////////////////////
void Imp_Indiv(Individuo* ind, ProblemInfo *p)
{
  int i,j,k=0;
  printf("\n\n");
  for(i=0;i<p->v;i++)
   {
     for(j=0;j<p->b;j++)
      {
        printf("%2d",ind->genoma[k]);
        k++;
      }
     printf("\n");
  }
 printf("\n\nFitness=  %5.1lf",ind->fitness);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion de evaluacion del Fitness del Individuo
///////////////////////////////////////////////////////////////////////////////////////////
void Esc_Indiv(Individuo* ind, ProblemInfo *p,FILE *fp,double kEvalMin)
{
  int i,j,k=0;
  fprintf(fp,"\n%d  %12ld   %10.0lf  %8.0lf  ",p->nrun,p->TotalEval,kEvalMin,ind->fitness);
  for(i=0;i<p->v*p->b;i++)
   {
        fprintf(fp,"%2d",ind->genoma[k]);
        k++;
    }
}     
////////////////////////////////////////////////////////////////////////////////////////////
//
// Funcion Escribe Individuo en Archivo
///////////////////////////////////////////////////////////////////////////////////////////
void Esc_Sol(Individuo* ind, ProblemInfo *p,FILE *fp)
{
  int i,j,k=0;
  fprintf(fp,"%d\n",p->v);
  for(i=0;i<p->v*p->b;i++)
   {
        fprintf(fp,"%2d",ind->genoma[k]);
        k++;
    }
}     


void HcSwap(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,FILE *fp)
{
  int nc=0,enc=FALSE;
  double nu=0,dif=p->v*p->r*(p->b-p->r)*(2+p->v-1)/(p->v+p->b+p->v*(p->v-1)/2);
  while((nu<(double)xNum_Eval)&&!enc)  
   {
     if(ind->fitness<(double)1)
          enc=TRUE;
     else
         enc=!vecinos(ind,p);
     nu+=dif;
     p->TotalEval+=((long)dif)*p->vMp;
     if(p->TotalEval>=puntos[p->van])
       {
         p->van++;
         Esc_Indiv(ind,p,fp,p->nEvalMin+nu);
       }
   }
  p->nEvalMin+=nu;
  if(nu>=(double)xNum_Eval)
    xNum_Eval=0;
  else
    xNum_Eval=xNum_Eval-(long)nu;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera operar sobre las Filas de la matriz  (Hacer swap sobre cada par de (0,1) o (1,0)
/////////////////////////////////////////////////////////////////////////////////////////////////
int  vecinos(Individuo *ind,ProblemInfo *p)
{
  int i=0,j,fi,sw=FALSE,t=0;
  Individuo *vecino,*zmajor;
  vecino=CrearIndividuo(p->v*p->b);
  zmajor=CrearIndividuo(p->v*p->b);
  CopiarIndividuo(zmajor,ind);
  for(fi=0;fi<p->v;fi++)
    {
      
      for(i=0;i<p->b;i++)
        for(j=i+1;j<p->b;j++)
          {
            if(ind->genoma[fi*p->b+i]+ind->genoma[fi*p->b+j]==1) // Si hay una pareja de (o,1) o (1,0) Intercambiable
             {
               CopiarIndividuo(vecino,ind);
               vecino->genoma[fi*p->b+i]=1-vecino->genoma[fi*p->b+i];
               vecino->genoma[fi*p->b+j]=1-vecino->genoma[fi*p->b+j];
               vecino->fitness=EvaluarRestricion(vecino,p);
               if(vecino->fitness<zmajor->fitness)
                 {
                   CopiarIndividuo(zmajor,vecino);
                   sw=TRUE;
                 }
             }
          }
    }
  if(sw)
    CopiarIndividuo(ind,zmajor);
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;

}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Aplicar Hill Climbing
// Consideramos la Vecindad Bit.Flip 
/////////////////////////////////////////////////////////////////////////////////////////////////
void HcBitFlip(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,FILE *fp)
{
  int nc=0,enc=FALSE;
  double nu=0,dif=(p->v*p->b*(2+(p->v-1)))/(p->v+p->b+p->v*(p->v-1)/2);
  while((nu<xNum_Eval)&&!enc)  // Para esta instancia se prueba que con 500 vueltas es
   {
     if(ind->fitness<1)
          enc=TRUE;
     else
         enc=!vecinosBF(ind,p);
     nu+=dif;
     p->TotalEval+=((long)dif)*p->vMp;
     if(p->TotalEval>=puntos[p->van])
       {
         p->van++;
         Esc_Indiv(ind,p,fp,p->nEvalMin+nu);
       }
   }
  p->nEvalMin+=nu;
  p->TotalEval+=(long)nu;
  if(nu>=(double)xNum_Eval)
      xNum_Eval= 0;//(long)nu;
  else
      xNum_Eval=(long)(xNum_Eval-nu);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera operar sobre las Filas de la matriz  (Hacer Bit Flip sobre cada Bit
/////////////////////////////////////////////////////////////////////////////////////////////////
int vecinosBF(Individuo *ind,ProblemInfo *p)
{
  int i=0,j,fi,sw=FALSE,t=0;
  Individuo *vecino,*zmajor;
  vecino=CrearIndividuo(p->v*p->b);
  zmajor=CrearIndividuo(p->v*p->b);
  CopiarIndividuo(zmajor,ind);
  for(fi=0;fi<p->v*p->b;fi++)
    {
      CopiarIndividuo(vecino,ind);
      vecino->genoma[fi]=1-vecino->genoma[fi];
      vecino->fitness=EvaluarRestricion(vecino,p);
      if(vecino->fitness<zmajor->fitness)
       {
         CopiarIndividuo(zmajor,vecino);
         sw=TRUE;
       }
    }
  if(sw)
    CopiarIndividuo(ind,zmajor);
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;   
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Aplicar Tabu Search
// Consideramos la Vecindad Swap Bits (2 bits 0-->1 or  1-->0 
/////////////////////////////////////////////////////////////////////////////////////////////////
void TsSwap(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,long nEvalIntens,FILE *fp)
{
  //printf("\nTs Swap\n");
  //Individuo *aqui;
  TabuList *Listas;
  double sinMejora=0;
  int enc=FALSE,change=0; 
  double nu=0,dif=p->v*p->r*(p->b-p->r)*(2+p->v-1)/(p->v+p->b+p->v*(p->v-1)/2);
  Listas=(TabuList *)malloc(sizeof(TabuList)*(p->v));
  for(int i=0;i<p->v;i++)
    Listas[i].Lt=(int *)malloc(sizeof(int)*(p->b*p->b));
  InicTl(Listas,p->v,p->b);
  //**************************************
  // Determinamos el Valor Tenure de la Lista Tabu   
   int b1,b2,bdif;
   b1=(p->v*p->b*p->r)/2;
   b2=3*(p->v*p->b*p->r)/2;
   time_t ttt;
   bdif=b2-b1;

  //*************************************************************
  // Opcion con reinicio desde otro punto (aleatorio)
  Individuo *xmejor;
  xmejor=CrearIndividuo(p->v*p->b);
  CopiarIndividuo(xmejor,ind);
  double xEvalIntens=(double)nEvalIntens*dif,TCPU;
  //***********************************************************
  while((nu<xNum_Eval)&&!enc)  // Para esta instancia se prueba que con 500 vueltas es
   {
     if(ind->fitness<1)
          enc=TRUE;
     else
        {                                      
          if(sinMejora>=3||p->tInic!=9)
             vecinosTsSw(ind,p,Listas,xmejor,URand(bdif)+b1);
          if(ind->fitness<xmejor->fitness)
            { 
               CopiarIndividuo(xmejor,ind);
               p->nEvalMin=nu+dif;
            }
          else
            {
                  sinMejora++;
                  if(sinMejora>2&&xmejor->fitness>0)
                   {
                     CopiarIndividuo(ind,xmejor);
                     if(p->tInic==9)
                      {
                        if(change<3)
                         {
                           DColaborative(ind,p); // Colaborative Diversification
                           if(ind->fitness<xmejor->fitness)
                             { 
                              CopiarIndividuo(xmejor,ind);
                              p->nEvalMin=nu+dif;
                            }
                           change++;
                         }
                        nu+=dif*3;
                      }
                     sinMejora=0;
                   }
             }
            
        }
     nu+=dif;
     p->TotalEval+=((long)dif)*p->vMp;
     if(p->TotalEval>=puntos[p->van])
       {
         p->van++;
         Esc_Indiv(xmejor,p,fp,p->nEvalMin);
       }
    
   }
  p->TotalEval+=(long)nu;
  if(xmejor->fitness<ind->fitness)
     CopiarIndividuo(ind,xmejor);
  for(int i=0;i<p->v;i++)
    if(Listas[i].Lt)
      free(Listas[i].Lt);
  free(Listas);
  DestruirIndividuo(xmejor);
}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera operar sobre las Filas de la matriz  (Hacer Swap Bits
/////////////////////////////////////////////////////////////////////////////////////////////////
int vecinosTsSw(Individuo *ind,ProblemInfo *p,TabuList *List,Individuo *xmejor,int mov)
{
   int max_mov=mov,primero=1; // max_mov sera el valor tenure de las Lista Tabu
   int i,j,fi,xcan=0,sw=FALSE,posfi,posi,posj;
   Individuo *vecino,*zmajor;
   vecino=CrearIndividuo(p->v*p->b);
   zmajor=CrearIndividuo(p->v*p->b);
   CopiarIndividuo(zmajor,ind);

   for(fi=0;fi<p->v;fi++)
    {
      for(i=0;i<p->b;i++)
        for(j=i+1;j<p->b;j++)
          {
            
            if(ind->genoma[fi*p->b+i]+ind->genoma[fi*p->b+j]==1) // Si hay una pareja de (o,1) o (1,0) Intercambiable
             {
               int *aux=List[fi].Lt+i*p->b,tabu;
               tabu=aux[j]; // Tomamos el indicador tabu de la lista Tabu
               CopiarIndividuo(vecino,ind);              
               vecino->genoma[fi*p->b+i]=1-vecino->genoma[fi*p->b+i];
               vecino->genoma[fi*p->b+j]=1-vecino->genoma[fi*p->b+j];
               vecino->fitness=EvaluarRestricion(vecino,p);
               if(tabu>0)
                {
                  if(vecino->fitness<xmejor->fitness)
                   {
                    CopiarIndividuo(zmajor,vecino);
                    posfi=fi;
                    posi=i;
                    posj=j;
                    sw=TRUE;
                   }
                }
               else
                  if(tabu==0)
                    {
                      if(primero)
                      {
                          CopiarIndividuo(zmajor,vecino);
                          posfi=fi;
                          posi=i;
                          posj=j;
                          primero=0;
                          sw=TRUE;
                      }else
                      if(vecino->fitness<zmajor->fitness)
                       {
                         CopiarIndividuo(zmajor,vecino);
                         posfi=fi;
                         posi=i;
                         posj=j;
                         sw=TRUE;
                       }
                    }
                 
             }
          }
    }
  if(sw)
    {
      CopiarIndividuo(ind,zmajor);
      // Actualizamos Lista Tabu
      int *aux=List[posfi].Lt+posi*p->b;
      aux[posj]=max_mov+1;              
    }
  decLt(List,p->v,p->b); // Decrementamos Lista Tabu
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;  
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Aplicar Tabu Search
// Consideramos la Vecindad Bit.Flip 
/////////////////////////////////////////////////////////////////////////////////////////////////
void TsBF(Individuo *ind,ProblemInfo *(&p),long &xNum_Eval,long nEvalIntens,FILE *fp)
{
  //Individuo *aqui;
  TabuList *Listas;
  double sinMejora=0;
  int nc=0,enc=FALSE,primero=1;
  double nu=0,dif=p->v*p->b*(2+p->v-1)/(p->b+p->v*(p->v-1)/2);
  Listas=(TabuList *)malloc(sizeof(TabuList)*(p->v));
  for(int i=0;i<p->v;i++)
    Listas[i].Lt=(int *)malloc(sizeof(int)*(p->b));
  InicTlFlip(Listas,p->v,p->b);
    //**************************************
  // Determinamos el Valor Tenure de la Lista Tabu   
   int b1,b2,bdif;
   b1=(p->v*p->b*p->r)/2;
   b2=3*(p->v*p->b*p->r)/2;
   time_t ttt;

   bdif=b2-b1;

     // Opcion con reinicio desde otro punto (aleatorio)
  Individuo *xmejor,*punto;
  xmejor=CrearIndividuo(p->v*p->b);
  punto=CrearIndividuo(p->v*p->b);
  CopiarIndividuo(xmejor,ind);
  CopiarIndividuo(punto,ind);
  //***********************************************************
  while((nu<xNum_Eval)&&!enc)  // Para esta instancia se prueba que con 500 vueltas es
   {
     if(ind->fitness<1)
          enc=TRUE;
     else
        {
          vecinosTsBF(ind,p,Listas,xmejor,punto,URand(bdif)+b1);
          if(ind->fitness<xmejor->fitness)
            {
              sinMejora=0;
              CopiarIndividuo(xmejor,ind);
              p->nEvalMin=nu+dif;
            }
          else
            {
                  sinMejora+=dif;
                  if(sinMejora>nEvalIntens){
                     CopiarIndividuo(ind,xmejor);
                     sinMejora=0;
                     }
             }
        }
     nu+=dif;
     p->TotalEval+=((long)dif)*p->vMp;
     if(p->TotalEval>=puntos[p->van])
       {
         p->van++;
         Esc_Indiv(xmejor,p,fp,p->nEvalMin);
       }
     
   }
  p->TotalEval+=(long)nu;
  if(xmejor->fitness<ind->fitness)
     CopiarIndividuo(ind,xmejor);
  for(int i=0;i<p->v;i++)
    if(Listas[i].Lt)
      free(Listas[i].Lt);
  free(Listas);
  DestruirIndividuo(xmejor);
  DestruirIndividuo(punto);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para generar los vecinos para un determiando estado de entrada
// Se considera aplicar Bit Flip
/////////////////////////////////////////////////////////////////////////////////////////////////
int vecinosTsBF(Individuo *ind,ProblemInfo *p,TabuList *List,Individuo *xmejor,Individuo *punto,int mov)
{
    int max_mov=mov,primero=1;  // max_mov == Valor Tenure de laLista Tabu
   int i,j,fi,xcan=0,sw=FALSE,posi,posj;
   Individuo *vecino,*zmajor;
   int b1,b2,bdif;

   vecino=CrearIndividuo(p->v*p->b);
   zmajor=CrearIndividuo(p->v*p->b);
 //  CopiarIndividuo(vecino,ind);
   CopiarIndividuo(zmajor,ind);

   for(fi=0;fi<p->v*p->b;fi++)
    {
      i=(int)(fi/p->b);
      j=(int)(fi%p->b);
      CopiarIndividuo(vecino,ind);
      vecino->genoma[fi]=1-vecino->genoma[fi];
      vecino->fitness=EvaluarRestricion(vecino,p);
      int tabu=List[i].Lt[j];
      //if(pto==fi)
      //  CopiarIndividuo(punto,vecino);
      if(tabu>0)
        {
           if(vecino->fitness<xmejor->fitness)
             {
               CopiarIndividuo(zmajor,vecino);
               posi=i;
               posj=j;
               sw=TRUE;
             }
        }
      else
        if(tabu==0)
            {
              if(primero)
                {
                 CopiarIndividuo(zmajor,vecino);
                 posi=i;
                 posj=j;
                 primero=0;
                 sw=TRUE;
             }else
               if(vecino->fitness<zmajor->fitness)
                 {
                   CopiarIndividuo(zmajor,vecino);
                   posi=i;
                   posj=j;
                   sw=TRUE;
                 }
          }
    }
  if(sw)
    {
      CopiarIndividuo(ind,zmajor);
      // Actualizamos Lista Tabu
      List[posi].Lt[posj]=max_mov+1;              
      
    }
  decLtFlip(List,p->v,p->b); // Decrementamos Lista Tabu
  DestruirIndividuo(vecino);
  DestruirIndividuo(zmajor);
  return sw;  
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Inicializar la Lista Tabu
/////////////////////////////////////////////////////////////////////////////////////////////////
void InicTl(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
   {
     for(unsigned int i=0;i<tList;i++)
       {
          int *aux=List[t].Lt +i*tList;
          for(unsigned int j=i+1;j<tList;j++)
            aux[j]=0;
        }
   }
}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para DEcrementar la Lista Tabu
/////////////////////////////////////////////////////////////////////////////////////////////////
void decLt(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
   {
     for(unsigned int i=0;i<tList;i++)
       {
          int *aux=List[t].Lt +i*tList;
          for(unsigned int j=i+1;j<tList;j++)
            if(aux[j]>0)
              aux[j]--;
       }
   }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Inicializar la Lista Tabu (caso Bit Flip)
/////////////////////////////////////////////////////////////////////////////////////////////////
void InicTlFlip(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
    for(unsigned int i=0;i<tList;i++)
       List[t].Lt[i]=0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para DEcrementar la Lista Tabu 
/////////////////////////////////////////////////////////////////////////////////////////////////
void decLtFlip(TabuList *List,unsigned int nList,unsigned int tList)
{
  for(unsigned int t=0;t<nList;t++)
    for(unsigned int i=0;i<tList;i++)
      if(List[t].Lt[i]>0)
         List[t].Lt[i]-=1;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Funcion para Realizar Una reparacion por Columnas a un Individuo con 'r' unos por Fila
/////////////////////////////////////////////////////////////////////////////////////////////////
void ReparaCol(Individuo *ind,ProblemInfo *p)
{
   int j,k,r,i,cp,ka;
   int col[100],point[100];
   Individuo *cand,*xmej;
   cand=CrearIndividuo(p->v*p->b);
   xmej=CrearIndividuo(p->v*p->b);
   CopiarIndividuo(cand,ind);
   CopiarIndividuo(xmej,ind);
   for (j=0; j<p->b; j++)
     col[j]=0;
   r=0;
   for (i=0; i<p->v; i++)
     for(j=0;j<p->b;j++)
      {
        col[j]+=ind->genoma[r];
        r++;
      }
   for(j=0;j<p->b;j++)
     col[j]-=p->k;
   while(1)
   {
     i=0; //punto donde sobran unos
     while(i<p->b&&col[i]<=0)
       i++;
     j=0; //punto donde faltan unos
     while(j<p->b&&col[j]>=0)
       j++;
     if(i>=p->b||j>=p->b)
       break;
     else
      {
        cp=0;// buscamos un par de (1,0) en la misma fila
        for(ka=0;ka<p->v;ka++) //para hacer swap y evaluar
         {
           k=ka*p->b+i; // [k,r] = Posicion en Genoma a evaluar celda de la matriz
           r=ka*p->b+j;  
           if(ind->genoma[k]==1&&ind->genoma[r]==0)
            {
              point[cp]=ka; // fila donde hay un par de (1,0)
              cp++;  // Cantidad de puntos factibles de swap
            }        // Num. de candidatos
          }
         if(cp)
           {
             CopiarIndividuo(xmej,ind);
             for(ka=0;ka<cp;ka++)
              {
                CopiarIndividuo(cand,ind);
                cand->genoma[point[ka]*p->b+i]=1-cand->genoma[point[ka]*p->b+i];
                cand->genoma[point[ka]*p->b+j]=1-cand->genoma[point[ka]*p->b+j];
                cand->fitness=EvaluarRestricion(cand,p);
                if(cand->fitness<xmej->fitness)
                  CopiarIndividuo(xmej,cand); //candidato con menor fitness
              }
             if(xmej->fitness<ind->fitness)
                CopiarIndividuo(ind,xmej);
             col[i]--;
             col[j]++;
           }
      }
   }
  DestruirIndividuo(cand);
  DestruirIndividuo(xmej);
}



