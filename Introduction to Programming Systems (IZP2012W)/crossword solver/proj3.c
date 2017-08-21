#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#define MALE 32
#define dlzka_search 9
#define EOLN_UNIX 13

typedef struct
{
    unsigned int r,s;
    char * pole;
}Tpole2d;

void printerr(int kodchyby);
void Print_help();

int vyrob_maticu(FILE* subor,Tpole2d*matrix);
int nacitaj_maticu(FILE* subor, Tpole2d *matrix);
void vypis_maticu(Tpole2d matrix);
void vypis_tajnicku(Tpole2d matrix);
int prehladaj_maticu(Tpole2d *matrix,char* slovo);

int check_params(int argc, char** argv);
char* orezslovo(char*slovo,int hodnota);
char nacitaj_pismeno(FILE*subor);
char to_upper(char a);

void to_upper_vertikalne(unsigned int volba,unsigned int i,unsigned int j,int k,Tpole2d * matrix);
void to_upper_horizontalne(unsigned int volba,unsigned int i,unsigned int j,int k,Tpole2d * matrix);
void to_upper_uhlopriecka(unsigned int volba,unsigned int i,unsigned int j,int k,Tpole2d * matrix);
int test_vertikalne(unsigned int strana,char* slovo,int i, int j, int k, Tpole2d * matrix);
int test_horizontalne(unsigned int strana,char* slovo,int i, int j, int k, Tpole2d * matrix);
int test_uhlopriecok(unsigned int volba,char* slovo,unsigned int i, unsigned int j, unsigned int k, Tpole2d * matrix);
enum kody{
NACITANIESUBORU,
CHYBNA_DLZKA_SUBORU,
CHYBNY_POCET_ARG,
CHYBNY_FORMAT_ARG,
HELP,
TEST,
SEARCH,
SOLVE,
CHYBA_ALOKACIE,
U1,
U2,
U3,
U4,
VPRAVO,
VLAVO,
HORE,
DOLE,
};

const char* const errors[] =
{
    [CHYBA_ALOKACIE] = "\nChyba, alokacia sa nepodarila\n",
    [NACITANIESUBORU] = "\nChyba pri nacitani suboru:",
    [CHYBNY_POCET_ARG] = "\nChyba, chybny pocet argumentov",
    [CHYBNA_DLZKA_SUBORU] = "\nChyba, chybny pocet znakov v subore\n",
    [CHYBNY_FORMAT_ARG] = "\nZadany format argumentu nie je spravny\n"
};
///----------------------------------------------FUNKCIA MAIN-----------------------------------------------------
int main(int argc,char ** argv)
{
    FILE* subor;
    int volba = check_params(argc,argv);
    // help & chybne formaty
    if (volba == HELP)
    {
        Print_help();
        return EXIT_SUCCESS;
    }
    if (volba == CHYBNY_FORMAT_ARG)
    {
        printerr(CHYBNY_FORMAT_ARG);
        return EXIT_FAILURE;
    }
    if (volba == CHYBNY_POCET_ARG)
    {
        printerr(CHYBNY_POCET_ARG);
        return EXIT_FAILURE;
    }
    //----------------------------------------------------------------------------------
    if ((subor=fopen(argv[2],"r"))==NULL)
    {
        printerr(NACITANIESUBORU);
        fprintf(stderr,"%s\n",strerror(errno));
        return EXIT_FAILURE;
    }
    //-vyroba matice
    Tpole2d matrix;
    if (vyrob_maticu(subor,&matrix)==EXIT_FAILURE)
    {
        printerr(NACITANIESUBORU);
        return EXIT_FAILURE;
    }
    if (nacitaj_maticu(subor,&matrix)==EXIT_FAILURE)
    {
        printerr(CHYBNA_DLZKA_SUBORU);
        return EXIT_FAILURE;
    }
    //-----------------------------
    if (volba==TEST)
    {
        vypis_maticu(matrix);
        free (matrix.pole);
    }
    else if(volba==SEARCH)
    {
        char* slovo;
        if ((slovo=malloc(matrix.s*sizeof(char)))==NULL)
        {
            printerr(CHYBA_ALOKACIE);
            return EXIT_FAILURE;
        }
        slovo=orezslovo(argv[1],dlzka_search); //aj konvertuje ch
        if (!prehladaj_maticu(&matrix,slovo))
            fprintf(stderr,"Slovo %s nebolo najdene\n",argv[1]);
        vypis_maticu(matrix);
        free (matrix.pole);
    }
    else //volba == SOLVE
    {
        FILE* zdroj;
        if ((zdroj=fopen(argv[3],"r"))==NULL)
        {
            printerr(NACITANIESUBORU);
            fprintf(stderr,"%s\n",strerror(errno));
            return EXIT_FAILURE;
        }
        char*slovo;
        if ((slovo=malloc(matrix.s*sizeof(char)))==NULL)
        {
            printerr(CHYBA_ALOKACIE);
            return EXIT_FAILURE;
        }
        char*slovo_nahr;
        if ((slovo_nahr=malloc(matrix.s*sizeof(char)))==NULL)
        {
            printerr(CHYBA_ALOKACIE);
            return EXIT_FAILURE;
        }
        while (fscanf(zdroj,"%s",slovo)!=EOF)
        {
            slovo_nahr=slovo;
            slovo=orezslovo(slovo,0);//konvertacia ch
            if (!prehladaj_maticu(&matrix,slovo))
                fprintf(stderr,"Slovo %.*s nebolo najdene\n",matrix.s,slovo_nahr);
        }
        free (slovo_nahr);
        vypis_tajnicku(matrix);
        free(matrix.pole);
        fclose(zdroj);
    }
    fclose(subor);
    return EXIT_SUCCESS;
}
///----------------------------------------------KONIEC FUNKCIE MAIN-----------------------------------------------------
char to_upper(char a) //funkcia zvacsi pismeno (aj ch)
{
    if (a>='`' && a<='z')
        return (a-MALE);
    else
        return a;
}
int prehladaj_maticu(Tpole2d*matrix, char* slovo)
{
    unsigned int  dlzka=strlen(slovo);
    int vysledok=0;
    if (dlzka==1) //ak je dlzka 1, iba pozvacsujem vyskyty pismena v osemsmerovke
    {
       for (unsigned int i=0;i<matrix->r;i++)
         for (unsigned int j=0;j<matrix->s;j++)
         {
            int sur=matrix->s*i+j;
            if (matrix->pole[sur]==slovo[0]) //najdem prve pismenko slova
            {
                matrix->pole[sur]=to_upper(matrix->pole[sur]);
                vysledok=1;
            }
         }
       return vysledok;
    }
    //pre dlzku veciu ako 1
    dlzka--;// hladam podla 1veho pismena-> ked ho raz najdem znova ho neoverujem -> dlzka--
    for (unsigned int i=0;i<matrix->r;i++)
        for (unsigned int j=0;j<matrix->s;j++)
        {
            unsigned int const sur_i=matrix->s*i;
            if (matrix->pole[sur_i+j]==slovo[0]||matrix->pole[sur_i+j]==slovo[0]-MALE) //najdem prve pismenko slova
            {
                unsigned int j0=matrix->s-j;
                unsigned int i0=matrix->r-i;
                // uhlopriecka je vzdy taka velka ako mensi z vektorov jej suctu
                /**

                   u4 \   |i  /u1
                       \  |  /
                        \ | /
                         \|/
                ----------O---------
                j        /|\        J0
                        / | \
                       /  |  \
                   u3 /   |I0 \u2
                */
                unsigned int u1= i>j0 ? j0:i; //
                unsigned int u2= j0>i0 ? i0:j0;
                unsigned int u3= i0>j ? j:i0;
                unsigned int u4= j>i ? i:j;
                if (dlzka<=j) //ak je vlavo miesto
                {
                    int zhoda=test_horizontalne(VLAVO,slovo,i,j,dlzka,matrix);
                    if (zhoda==EXIT_SUCCESS)// check vlavo
                    {
                        to_upper_horizontalne(VLAVO,i,j,dlzka,matrix);
                        vysledok=1;
                    }
                    if (j==u3 && j!=i0) //check na uhlopriecke u3 -->ak i0=j, uprednostnim i0--------------------------------
                        if (test_uhlopriecok(U3,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                        {
                            to_upper_uhlopriecka(U3,i,j,dlzka,matrix);
                            vysledok=1;
                        }
                    if (j==u4) //check na uhlopriecke u4----------------------------------------
                      if(test_uhlopriecok(U4,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                      {
                            to_upper_uhlopriecka(U4,i,j,dlzka,matrix);
                            vysledok=1;
                      }
                }
                if (dlzka<j0) // ak je v pravo miesto
                {
                    int zhoda=test_horizontalne(VPRAVO,slovo,i,j,dlzka,matrix);
                    if (zhoda==EXIT_SUCCESS) //check vpravo
                    {
                        to_upper_horizontalne(VPRAVO,i,j,dlzka,matrix);
                        vysledok=1;
                    }
                    if (j0==u1 && j0!=i) //check na uhopriecke u1-->ak i=j0, uprednostnim i
                        if(test_uhlopriecok(U1,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                        {
                            to_upper_uhlopriecka(U1,i,j,dlzka,matrix);
                            vysledok=1;
                        }
                    if (j0==u2) //check na uhlopriecke u2
                      if(test_uhlopriecok(U2,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                      {
                            to_upper_uhlopriecka(U2,i,j,dlzka,matrix);
                            vysledok=1;
                      }

                }
                if (dlzka<=i) //ak je miesto hore
                {
                    int zhoda=test_vertikalne(HORE,slovo,i,j,dlzka,matrix);
                    if (zhoda==EXIT_SUCCESS) //check hore
                    {
                        to_upper_vertikalne(HORE,i,j,dlzka,matrix);
                        vysledok=1;
                    }
                    if (i==u1)// check na uhlopriecke u1
                        if(test_uhlopriecok(U1,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                        {
                            to_upper_uhlopriecka(U1,i,j,dlzka,matrix);
                            vysledok=1;
                        }
                    if (i==u4 && i!=j) //check na uhlopriecke u4-->ak j=i, uprednostnim j----------------------------------------
                        if(test_uhlopriecok(U4,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                        {
                            to_upper_uhlopriecka(U4,i,j,dlzka,matrix);
                            vysledok=1;
                        }
                }
                if (dlzka<i0)//ak je miesto dole
                {
                    int zhoda=test_vertikalne(DOLE,slovo,i,j,dlzka,matrix);
                    if (zhoda==EXIT_SUCCESS) //check dole
                    {
                        to_upper_vertikalne(DOLE,i,j,dlzka,matrix);
                        vysledok=1;
                    }
                    if (i0==u2 && i0!=j0) //check na uhlopriecke u2-->ak j0=i0, uprednostnim j0--------------------------------
                        if(test_uhlopriecok(U2,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                        {
                            to_upper_uhlopriecka(U2,i,j,dlzka,matrix);
                            vysledok=1;
                        }
                    if (i0==u3) //check na uhlopriecke u3--------------------------------
                        if(test_uhlopriecok(U3,slovo,i,j,dlzka,matrix)==EXIT_SUCCESS)
                        {
                            to_upper_uhlopriecka(U3,i,j,dlzka,matrix);
                            vysledok=1;
                        }
                }
            }
        }
    return vysledok;
}
int test_vertikalne(unsigned int strana,char* slovo,int i, int j,  int k, Tpole2d * matrix)
{
    if (strana==HORE)
        i=-i;
    else if (strana!=DOLE)
    {
        fprintf(stderr,"funkcia: test_vertikalne: Neznama volba\n");
        return EXIT_FAILURE;
    }
        while (k>0)
    {
        char pismeno=matrix->pole[matrix->s*abs(i+k)+j];
        if (slovo[k]!=pismeno && slovo[k]!=pismeno+MALE)
            return EXIT_FAILURE;
        k--;
    }
    return EXIT_SUCCESS;
}
int test_horizontalne(unsigned int strana,char* slovo, int i, int j, int k, Tpole2d * matrix)
{
    if (strana==VLAVO)
         j=-j;
    else if (strana!=VPRAVO)
    {
        fprintf(stderr,"funkcia: test_horizontalne: Neznama volba\n");
        return EXIT_FAILURE;
    }
    const int sur_i=matrix->s*i;
    while (k>0)
    {
        char pismeno=matrix->pole[sur_i+abs(j+k)];
        if (slovo[k]!=pismeno && slovo[k]!=pismeno+MALE)
            return EXIT_FAILURE;
        k--;
    }

    return EXIT_SUCCESS;
}
int test_uhlopriecok(unsigned int volba,char* slovo,unsigned int i, unsigned int j, unsigned int k, Tpole2d * matrix)
{
    //k=dlzka
    if (volba==U1)
        i=-i;
    else if (volba==U3)
        j=-j;
    else if (volba==U4)
    {
        i=-i;
        j=-j;
    }
    else if (volba!=U2)
    {
        fprintf(stderr,"funkcia: test_uhlopriecok: Neznama volba\n");
        return EXIT_FAILURE;
    }     //k = dlzka retazca
    while (k>0)
    {
         char pismeno=matrix->pole[matrix->s*abs(i+k)+abs(j+k)];
         if (slovo[k]!=pismeno && slovo[k]!=pismeno+MALE)
            return EXIT_FAILURE;
         k--;
    }
    return EXIT_SUCCESS;
}
void to_upper_vertikalne(unsigned int volba,unsigned int i,unsigned int j,int k,Tpole2d * matrix)
{
    if (volba==HORE)
        i=-i;
    else if (volba!=DOLE)
        fprintf(stderr,"funkcia: to_upper_vertikalne: Neznama volba\n");
    while (k>=0)
    {
        matrix->pole[matrix->s*abs(i+k)+j]= to_upper(matrix->pole[matrix->s*abs(i+k)+j]);
        k--;
    }
}
void to_upper_horizontalne(unsigned int volba,unsigned int i,unsigned int j,int k,Tpole2d * matrix)
{
    if (volba==VLAVO)
        j=-j;
    else if (volba!=VPRAVO)
        fprintf(stderr,"funkcia: to_upper_horizontalne: Neznama volba\n");
    const int sur_i=matrix->s*i;
    while (k>=0)
    {
        matrix->pole[sur_i+abs(j+k)]= to_upper(matrix->pole[sur_i+abs(j+k)]);
        k--;
    }
}
void to_upper_uhlopriecka(unsigned int volba,unsigned int i,unsigned int j,int k,Tpole2d * matrix)
{
    if (volba==U1)
        i=-i;
    else if (volba==U3)
        j=-j;
    else if (volba==U4)
    {
        i=-i;
        j=-j;
    }
    else if (volba!=U2)
        fprintf(stderr,"funkcia: test_uhlopriecok: Neznama volba\n");
    while (k>=0)
    {
        matrix->pole[matrix->s*abs(i+k)+abs(j+k)]= to_upper(matrix->pole[matrix->s*abs(i+k)+abs(j+k)]);
        k--;
    }
}
char* orezslovo(char*slovo,int hodnota)
{

    int a=strlen(slovo);
    int i;
    for (i=hodnota;i<a;i++)
    {
        if (slovo[i]=='c' && slovo[i+1]=='h')
        {
            slovo[i-hodnota]='`';
            i++;
            hodnota++;
        }
        else
        slovo[i-hodnota]=slovo[i];
    }
    slovo[i-hodnota]='\0';
    return slovo;
}
void Print_help ()
{
    printf("**********************************NAPOVEDA************************************\n");
    printf("----FUNKCIA-------------------PRIKLAD ---------------------INFO---------------\n");
    printf("Spustenie s argumentmi  '--test data.txt'Skontroluje,ci data vo vstupnom      \n");
    printf("--test <nazov_suboru>.txt                subore neobsahuju chyby a spravne    \n");
    printf("                                         naformatovanou osemsmerovku vytlaci  \n");
    printf("                                         na standardny vystup.                \n");
    printf("                                                                              \n");
    printf("Spustenie s argumentmi  '--search=slovo     Pokusi sa najst vyskyty zadaneho  \n");
    printf("--search=<slovo>         osemsmerovka.txt'  slova v osemsmerovke nacitanej    \n");
    printf("                                            zo zadaneho suboru                \n");
    printf("                                                                              \n");
    printf("Spustenie s argumentmi  'proj3 --solve      Pokusi sa vylustit osemsmerovku   \n");
    printf("proj3 --solve            osm.txt slova.txt' vyskrtanim zadanych slov zo       \n");
    printf("<nazov.txt> <nazov.txt>                     zadanej osemsmerovky              \n");
    printf("                                                                              \n");
    printf("                                                                              \n");
    printf("spustenie s arg. -h           'proj3 -h'       Vytlaci napovedu               \n");
    printf("                                          pozn. Presnost musi byt vacsia ako 0\n");
    printf("Developed by Martin Fajcik (R)2012\n");
    printf("****************************************************************************\n");
}
int check_params(int argc, char** argv)
{
    if (argc<2 || argc >4)
        return CHYBNY_POCET_ARG;
    else if (argc ==2 && strcmp(argv[1],"-h")==EXIT_SUCCESS)
            return HELP;
    else if (argc==3)
    {

        if (strcmp(argv[1],"--test")==EXIT_SUCCESS)
            return TEST;
        else if (strlen(argv[1])>dlzka_search) //9 = dlzka vyrazu --search=
        {
            char a = argv[1][dlzka_search];
            argv[1][dlzka_search]=0;
            if (strcmp("--search=",argv[1])==EXIT_SUCCESS)
            {
                argv[1][dlzka_search]=a;
                return SEARCH;
            }
            else
                return CHYBNY_FORMAT_ARG;
        }
        else
          return CHYBNY_FORMAT_ARG;
    }
    else //argc=4
    {
        if(strcmp("--solve",argv[1])==EXIT_SUCCESS)
           return SOLVE;
        else
          return CHYBNY_FORMAT_ARG;
    }
}
int vyrob_maticu(FILE* subor,Tpole2d *matrix)
{
    if (fscanf(subor,"%u %u",&matrix->r,&matrix->s)!=2)
        return EXIT_FAILURE;
    if (matrix->r==0 || matrix->s==0)
        return EXIT_FAILURE;
    if ((matrix->pole=malloc(sizeof(char)*(matrix->r*matrix->s)))==NULL)
    {
        printerr(CHYBA_ALOKACIE);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
int nacitaj_maticu(FILE* subor, Tpole2d *matrix)
{
   for (unsigned int i=0;i<(matrix->r*matrix->s);i++)
   {
    char a;
    matrix->pole[i]=nacitaj_pismeno(subor);
    if ((a=getc(subor))=='h'&&matrix->pole[i]=='c')
        matrix->pole[i]='`';
    else
        ungetc(a,subor);

    if ((a=nacitaj_pismeno(subor))==EOF && i!=matrix->r*matrix->s-1)
    {
         return EXIT_FAILURE;
    }
    else
        ungetc(a,subor);
   }
   return EXIT_SUCCESS;
}
void vypis_tajnicku(Tpole2d matrix)
{
    for (unsigned int i=0;i<matrix.r;i++)
        for(unsigned int j=0;j<matrix.s;j++)
        {
            if (matrix.pole[matrix.s*i+j]>'Z')
            printf("%c",matrix.pole[matrix.s*i+j]);
        }
    return;
}
void vypis_maticu(Tpole2d matrix)
{
    for (unsigned int i=0;i<matrix.r;i++)
    {
        for(unsigned int j=0;j<matrix.s;j++)
        {
            char a= matrix.pole[matrix.s*i+j];
            if (a=='`')
                printf("ch ");
            else if (a=='@')
                printf("CH ");
            else
            printf("%c ",a);
        }
       printf("\n");
    }
    return;
}
char nacitaj_pismeno(FILE*subor)
{
    char pismeno;
         do
           {
              pismeno = getc(subor);
           }while (isblank(pismeno)||pismeno=='\n' || pismeno==EOLN_UNIX);
         return pismeno;
}

void printerr(int kodchyby)
{
    fprintf(stderr,"%s\n",errors[kodchyby]);
}
