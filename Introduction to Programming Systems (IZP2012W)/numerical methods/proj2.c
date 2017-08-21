/*
Projekt 2 - Iteracne vypocty
    vypracoval: Martin Fajcik @2012 - xfajci00
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

#define odmz3     1.7320508075688772935274463415059
#define IZP_PI_2  1.57079632679489661923    // pi/2
#define IZP_PI_6  0.52359877559829887307710723054658
#define LN10      2.30258509299404568402
#define ODM_10_2  3.1622776601683793319988935444327
#define e         2.71828182845904523536028747135266249775724709369995

void ERRREPORT (int code);
void printhelp();
void nahrflush(FILE *in, const char delim);

int relpres (double y, double y0,int sigdig);

double mocninova(int sigdig,double exp,double x);
double arctan (int sigdig,double x);
double arghypsin(int sigdig,double x);
double mocnina (const double a,int n);
double odmocnina(double cislo,unsigned int odm);
double lnx (const double x,int sigdig);
//**************************************************** HLAVNA FUNKCIA**********************************************
int main(int argc, char**argv)
{
    double x;
    double a;

    int chybnyvstup = 0;
    double sigdig;
          if (argc == 2)
      {
          if (strcmp(argv[1], "-h")==EXIT_SUCCESS)
          {
              printhelp();
              return EXIT_SUCCESS;
          }
          else
            ERRREPORT(0);
      }
      else if (argc == 3)
      {
          sigdig=trunc(strtod(argv[2],NULL));
          if (sigdig>15)
          {
              sigdig=15;
              ERRREPORT(2);
          }
          if (((strcmp(argv[1],"--arctg"))==0) && (sigdig>0))
          {

            while ((chybnyvstup= (scanf("%lf",&x)))!=EOF)
            {
                 if (chybnyvstup == 0)
                 {
                      printf("%f\n",NAN);
                      nahrflush(stdin,'\n');
                      continue;
                 }
                 printf("%.10e\n",arctan(sigdig,x)); //Arcus Tangens
            }
          }
          else if (((strcmp(argv[1],"--argsinh"))==0) && (sigdig>0))
          {
            while ((chybnyvstup= (scanf("%lf",&x)))!=EOF)
            {
                 if (chybnyvstup == 0)
                 {
                      printf("%f\n",NAN);
                      nahrflush(stdin,'\n');
                      continue;
                 }
                 printf("%.10e\n",arghypsin(sigdig,x)); //argument hyperbolickeho sinu
            }
          }
          else                  //
              ERRREPORT(0);
      }
      else if (argc == 4)
      {
          sigdig = trunc(strtod(argv[2],NULL));
          if (sigdig>15)
          {
              sigdig=15;
              ERRREPORT(2);
          }
          a = strtod(argv[3],NULL);
          if ((((strcmp(argv[1],"--powxa"))== 0) && (sigdig>0) && ((a != 0 ))) ||
              (((strcmp(argv[1],"--powxa"))== 0) && (sigdig>0) && (strcmp(argv[3],"0")  == 0)))
          {
            while ((chybnyvstup = (scanf("%lf",&x)))!=EOF)
            {
                 if (chybnyvstup == 0)
                 {
                      printf("%f\n",NAN);
                      nahrflush(stdin,'\n');
                      continue;
                 }
                 printf("%.10e\n",mocninova(sigdig,a,x));// mocninova funkcia
            }
          }
          else
              ERRREPORT(0);
      }

      else                  //
          ERRREPORT(1);

    return EXIT_SUCCESS;
}

enum Errcodes
{
    chyba0,
    chyba1,
    chyba2,
};
const char* const ERRMSG[] =
{
    [chyba0] = "CHYBA 0x0: Nespravny format argumentu\n pre napovedu zadajte argument -h.\n",
    [chyba1] = "CHYBA 0x1: Nespravny pocet argumentov \n pre napovedu zadajte argument -h.\n",
    [chyba2] = "CHYBA 0x2: Privelky parameter pre SigDig, program pocita hodnotou sigdig = 15\n",
};

void ERRREPORT (int code)
{
    fprintf(stderr, "%s\n",ERRMSG[code]);
}

void nahrflush(FILE *in, const char delim) //nahrada za fflush(stdin), ktory v linux gcc kompileri nefunguje spravne
{
    int c;
    do
    {
        c = getc(in);
    }
    while (c != delim && c != EOF);
}

int relpres (double y, double y0,int sigdig)
{
    if (y==0.0 || y0==0.0)
        return EXIT_FAILURE;
    y = fabs(y);
    y0 = fabs(y0);
    unsigned long int a = mocnina(10,sigdig);
    unsigned long int b = mocnina(10,sigdig-1);
    while (y>a)
    {
        y = y/10.0;
    }

    while (y<b)
    {
        y = y*10.0;
    }

    while (y0>a)
    {
       y0 = y0/10.0;
    }

    while (y0<b)
    {
       y0 = y0*10.0;
    }

    if (floor(y)==floor(y0))
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}
double mocnina (const double a,int n) // vyuzivam rekurzivne volania
{
    if (n==0)
        return 1.0;
    if (n==1)
        return a;
    if (n<0)
        return 1.0/(mocnina(a,abs(n)));

    if (n!=8 && (n%8)==0)
        return mocnina(mocnina(a,n/8),8);
    if (n!=5 && (n%5)==0)
        return mocnina(mocnina(a,n/5),5);
    if (n!=3 && (n%3)==0)
        return mocnina(mocnina(a,n/3),3);
    if (n!=2 && (n%2)==0)
        return mocnina(mocnina(a,n/2),2);

    double pomocna = 1.0;
    for (int i = 0;i<n;i++)
    {
        pomocna=pomocna*a;

    }
    return pomocna;
}
double odmocnina(double cislo,unsigned int odm)
{
  if (cislo == 0.0)
        return 0.0;
  int desiatky=0;
  if (odm == 2)
  {
      while (cislo>100.0)
        {
            cislo=cislo/100.0;
            desiatky++;
        }
  }
  const double eps=0.00000000001;
  double odhad,dolna,horna;
  if (cislo<0.0)
  {
      return NAN;
  }
  while (cislo<1.0)
  {
      cislo = cislo * 100.0;
      desiatky--;
  }
      dolna = 1.0;
      horna = cislo;

  while ((horna-dolna)>eps)
  {
      odhad = (dolna + horna)/2.0;
      if (mocnina(odhad,odm) > cislo)
         horna = odhad;
      else
         dolna = odhad;
  }
  cislo = (dolna + horna)/2.0;
    while (desiatky>0.0)
    {
       cislo =cislo*10.0;
       desiatky--;
    }
    while (desiatky<0.0)
    {
        cislo = cislo/10.0;
        desiatky++;
    }
  return cislo;
}
double lnx (double x,int sigdig)
{
    //algorytmus je najefektivnejsi pre co najmensie cislo, preto sa ho snazim zmensit
    if (x<=0)
    {
        return NAN;
    }
    double i=0.0;
    while (x>=10.0)
    {
        x=x/10.0;
        i= i+1.0;
    }
    while (x<1.0)
    {
        x=x*10.0;
        i= i-1.0;
    }
    double zvysok = LN10*i;
    if (x==1.0)
    {
        return  zvysok;
    }
    i = 0.0;
    while (x>ODM_10_2)
    {
        x=odmocnina(x,2); //vyjmem dvojku pred log
        i= i+1.0;
    }
    double y = (x-1.0)/(x+1.0);//vyjadrim y
    double yzakl=y*y;
    double a = 3.0;
    double ln=y;
    double poslln;
    do
     {
         y*=yzakl;
         poslln=ln;
         ln = ln + y/a;
         a = a + 2.0;
     }
     while ((relpres(ln,poslln,sigdig))==EXIT_FAILURE);
     ln=ln*2.0;
     double vysledok=0.0;
     if (i<1.0)
     {
        vysledok = ln+zvysok;
     }
     else
     {
         vysledok = i*2.0*ln+zvysok;
     }
   return vysledok; //spatna substituciu
}
// Help
void printhelp()
{
    printf("**********************************NAPOVEDA************************************\n");
    printf("----FUNKCIA-------------------PRIKLAD ---------------------INFO---------------\n");
    printf("Spustenie s argumentmi  'proj2 --powxa 3 2.1'  Pouzitie mocninovej     funkcie\n");
    printf("--powxa,<cele c.>,<realne c.>               s presnostou zadanou celym cislom \n");
    printf("                                            a exponentom danym realnym cislom \n");
    printf("                                                                              \n");
    printf("Spustenie s argumentmi  'proj2 --arctg 8'      Pouzitie operacie arcus tangens\n");
    printf("--arctg,<cele.c>                            s presnostou zadanou celym cislom \n");
    printf("                                                                              \n");
    printf("Spustenie s argumentmi                         Pouzitie operacie argument     \n");
    printf("--argsinh,<cele.c>      'proj2 --argsinh 7' hyperbolickeho sinu s  presnostou \n");
    printf("                                            zadanou celym cislom              \n");
    printf("                                                                              \n");
    printf("spustenie s arg. -h           'proj2 -h'       Vytlaci napovedu               \n");
    printf("                                          pozn. Presnost musí byt vacsia ako 0\n");
    printf("Developed by Martin Fajcik (R)2012\n");
    printf("****************************************************************************\n");
}

//************************************************* Matematicke Funkcie*******************************************
double mocninova(int sigdig,double exp,double x)
{

        int otoc = 0;
        int zapexp = -1;

        if (exp == 0.0)
            return 1.0;
        if (exp == 1.0)
            return x;

        if (x == 0.0)
            return 0.0;
        if (x == 1.0)
            return 1.0;
        if (fabs(exp)>8e2)
           return INFINITY;
        if ((fabs(x)>1e308) || (fabs(x)<1e-308))
            return INFINITY;
        if (x<0.0)
        {
            if((exp-trunc(exp))!=0)
                return NAN;
            else
                return mocnina(x,exp);
        }

        int rexp= trunc(exp);
        if ((fabs(exp) -abs(rexp))==0.0)
        {
            return mocnina(x,rexp);
        }
        if (x<1.0)
        {
            otoc = 1;
            x = 1.0/x;
            zapexp = 0;
        }
        if (exp<0.0)
        {
            exp=fabs(exp);
            zapexp = 0;
        }
        x = exp*lnx(x,sigdig+1);
        double n = trunc(x);
        double r = x-n; //rozdelenie - e^celu cast * e ^desatinnu cast

        n = mocnina(e,n);

        const double rzakl = r;
        double posl;
        double y=1.0;
        double spodna = 1.0;
        double i=1.0;
        do
        {
            spodna *= i;
            posl = y;
            y+=(r/spodna);
            r *=rzakl;
            i = i +1.0;
        }
        while((relpres(y,posl,sigdig))==EXIT_FAILURE);
        x=n*y; //e^n * e^y
        if (zapexp==0)
           return 1.0/x;
          else
           return x;
}

double arctan (int sigdig,double x)
{
        if (x == 0.0)
            return 0.0;
        if ((fabs(x)>1e308) || (fabs(x)<1e-308))
            return INFINITY;
        int doplnkovy = -1;
        int zapc=-1;
        if (x<0.0)
        {
           zapc = 0;
           x=-x;
        }
        if (x>1.0)
        {
            x=1.0/x;
            doplnkovy = 0;
        }
        int identita = -1;
        if (x>(2.0-odmz3))
        {
            identita = 0;
            x = (odmz3*x-1.0)/(odmz3 + x);
        }
        double a = 3.0;
        double y = x;
        double yposl;
        double mocnx=x;
        const double xna2 = x*x;
        do
        {
           mocnx = -mocnx*xna2;
           yposl = y;
           y += mocnx/a;
           a = a + 2.0;
        }
        while ((relpres(y,yposl,sigdig))==EXIT_FAILURE);
        if (!identita)
        {
           y=y+IZP_PI_6;
        }

        if (!doplnkovy)
        {
            y = IZP_PI_2 - y;
        }
    if (zapc==-1)
        return y;
    else
        return -y;
}

double arghypsin(int sigdig,double x)
{
    if (x ==0)
        return 0;
    if (fabs(x)>1e54 || fabs(x)<1e-54)
    {
             return INFINITY;
    }
    if (fabs(x)<1e-15)
        return x;
    if (x>0.0)
    {
        x = lnx(x+mocninova(sigdig+1,0.5,x*x+1),sigdig);
    }
    else
    {
        x = fabs(x);
        x = -lnx(x+mocninova(sigdig+1,0.5,x*x+1),sigdig);
    }

    return x;
}
