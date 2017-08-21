/*
Projekt: IZP č. 1 - tvorba histogramu
// ******************************************** (R) Martin Fajcik 2012*******************************************************************/
// ************************************************xfajci00*******************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
/*
                        ****Spocita pocet znakov a vypise (spusti sa bez parametrov)****
*/
void vytlac(int max,int vymenna_ordh)
{
    if (max>0)
      {
          if (max != UCHAR_MAX) //pocet mensi ako 255
          {
              if (!isprint(vymenna_ordh)) //nie je tlacitelny
              {
                  printf("%d: %d\n",vymenna_ordh,max);
              }
              else // je tlacitelny
              {
                  printf("%d '%c': %d\n",vymenna_ordh,vymenna_ordh,max);
              }

          }
          else  //pocet je  255
          {
              if (!isprint(vymenna_ordh)) //nie je tlacitelny
              {
                  printf("%d: NNN\n",vymenna_ordh);
              }
              else // je tlacitelny
              {
                  printf("%d '%c': NNN\n",vymenna_ordh,vymenna_ordh);

              }
          }
      }
}
void spoc_bez_para(void)
{
  int hodnch;
  unsigned char pole[256] = {0}; // 256 = UCHAR_MAX +1
  // Nacitanie textu do pola
  while ((hodnch = getchar())!= EOF)
  {
      if (pole[hodnch] != UCHAR_MAX)
      {
          pole[hodnch]++;
      }

  }
  //Vypis
  // UCHARMAX = 255;
  for (int i=0;i<(UCHAR_MAX+1);i++)
  {
      vytlac(pole[i],i);
  }
}


/*
        ****Spoocíta pocet znakov, vypise pocet riadkov podla velkosti parametru, riadky sú zoradene podla pocetnosti(spusti sa iba z parametrom)****
*/

void spoc_z_para(int arg)
{
  int hodnch,vymenna_ordh,max;
  unsigned char pole[256] = {0};
  // Nacítanie textu do pola
  while ((hodnch = getchar())!= EOF)
  {
      if (pole[hodnch] != UCHAR_MAX)
      {
          pole[hodnch]++;
      }

  }

  for (int i = 1; i<=arg;i++)
  {
     max = 0;
     for (int j = UCHAR_MAX;j>=0;j--)
     {
         if (pole [j]>= max)
         {
           max = pole[j];
           vymenna_ordh = j;
         }
     }
    vytlac(max,vymenna_ordh);
    pole[vymenna_ordh]=0;
  }
}

int dlzka (char*arg)
{
    int i=0;
    while (arg[i]!= 0)
    {
        i++;
    }
    return i;
}

// zisti ci je to cislo
int jecislo (char*arg)
{
    for (int i = 0; i != dlzka(arg);i++)
    {
        if (arg[i]<'0' || arg[i]>'9')
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

//retazec na cislo
int strnai (char*arg)
{
    int hodn = 0;
    for (int i=0;i<dlzka(arg);i++)
    {
        hodn = hodn*10+(arg[i] -'0');
    }
    return hodn;
}

// CHYBOVE HLASENIA
enum Errcodes
{
    chyba1,
    chyba2,
    chyba3,
};
const char* const ERRMSG[] =
{
  [chyba1] ="CHYBA1: chybny pocet argumentov , pre help zadajte argument \n-h alebo --help \n",
  [chyba2] ="CHYBA2: chybny rozsah argumentu - mimo(1 - 256), pre help zadajte argument \n-h alebo --help \n",
  [chyba3] ="CHYBA3: neplatny format argumentu - mimo(1 - 256), pre help zadajte argument \n-h alebo --help \n",
};
void ERRREPORT(int code)
{
  fprintf(stderr, "%s\n", ERRMSG[code]);
}
// CHYBOVE HLASENIA


//pomoc
void printhelp ()
{
    printf("\n\n");
    printf("**********************************NAPOVEDA************************************\n");
    printf("----FUNKCIA-------------------PRIKLAD ---------------------INFO---------------\n");
    printf("spustenie bez argumentu       'proj1' vypis vs. znakov zo stdin podla ord. h. \n");
    printf("spustenie s ciselnym          'proj1 5 '      vypis zadaneho poctu znakov zo  \n");
    printf("argumentom                            stdin podla pocetnosti(v pr. rovnakej   \n");
    printf("                                      pocetnosti podla ord.h.)                \n");
    printf("spustenie s arg. -h ,--help   'proj1 -h'      vytlaci napovedu                \n");
    printf("Developed by Martin Fajcik (R)2012\n");
    printf("******************************************************************************\n");
    printf("\n\n");
}





//Hlavna funkcia
int main(int argc, char**argv)
{
    if (argc == 1)
    {
        spoc_bez_para();

    }
    else if (argc == 2)
    {
      if (jecislo(argv[1])==EXIT_SUCCESS)
      {
          if (strnai(argv[1])>0 && strnai(argv[1])<=(UCHAR_MAX+1)) //( 1-256)
          {
              spoc_z_para(strnai(argv[1]));
          }
          else
          {
              ERRREPORT(1);
          }
      }
      else
      {
            if ((strcmp(argv[1],"-h") == EXIT_SUCCESS)|| //-h
                (strcmp(argv[1],"--help")==EXIT_SUCCESS) ) //--help
             {
                 printhelp();
             }
             else
             {
                 ERRREPORT(2);
             }
      }

    }
    else
    {
        ERRREPORT(0);
    }

    return 0;
}
