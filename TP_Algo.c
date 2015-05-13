#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <windows.h>

//  chaque  page  contient 3 ligne
#define lp 3
// lp : ligne  par page
// chaque  ligne  peut avoir au max 100 char


/*

  la fonction check nous permet de verifier si le mot chain est parmi les mots qui
  ne devront  pas  etre mis dans l'index    ou pas
  et dans cette  fonction on a utilise  la recherche   "Dichotomique"

 */
int check (char chain[50]) {
               int i=0 ,max = 30,pos;
               char forb[30][11]={"ainsi","aussi","aussitot","avant","bien","cependant","ces","cet","cette","comme","donc","elles","enfin","ils"
                                 ,"jusqu'a","lorsque","mais","malgre","neamoins","parce","pourtant","puis","puisque","que","quoi","sinon","soit","tandis"
                                 ,"tant","toutefois"};


        while (i < max) {
          pos = (i + max)/2 ;
            if ((strcasecmp(chain,forb[pos]))== 0) {
                    return 1 ;
                    } else if ((strcasecmp(chain,forb[pos])) < 0) {
                       max = pos ;
                          } else {
                                i = pos +1 ;
                                 }
                         }
 return 0 ;
}



struct page {   // enregistrement  de la page  ; chaque page  doit contenir :
                //le numero de la page  et le nombre d'occurence  et un ptr vers la  prochainne  page de meme type bien sur
            int numpg ;
            int nbrocc ;
            struct page *next ;
};



typedef struct page page;


struct mot1 { // l'enregistrement  de  chaque  mot
             page *prempg ;
             page *derpg ;
             char *motindex;
             struct mot1 *next ;

};



typedef struct mot1 mot ;

/*

    on a  declare  la procedure write pour nous facilite le travaile "la lecture des mots"
    a chaque fois on prend  une ligne de  100 char  on  l'ecris dans
    un fichier  temporaire  tmp.txt  a chaque fois  on ecrase  ce qu'il ya dedans  et on  reecrit ;
    apres on lit en utilisant  la fonction   fscanf jusqu'a la fin de fichier tmp.txt
    Qui contient   seulement  une seule  ligne  avec  une autre  fonction


  */
void write (char ch[100]){
                          FILE *ft;
                          ft = fopen("tmp.txt","w");
                          if (ft == NULL) {
                          printf("can't create temporary File ");
                          }
                          fputs(ch,ft);
                          fclose(ft);
                         };



int verif2 (char  mot[30]) {

     int  i , j,k ;
     k=1;
     i = 0 ;
     j = strlen(mot);


     while  ((i<j) && (k==1)){

           if (isalpha(mot[i]) || isdigit((int )fabs(mot[i]))){

                               i++ ;

                               } else  {

                                       k = 0 ;
                                       }




           }


     return k ;



    }



/*


   la fonction word retourne  2 si le mot  exist deja et dans  la meme page  Qu'on veut ajoute
   par exemple : on veut ajouter le mot  apple dans la page num 1   mais  comme on a  déja apple dans la page 1
   alors  on  sort  avec 2  pour  incrementer le  nombre d'occurence  seulement


  [+] elle retourne  1 si le mot existe mais dans une autre  page  avant la page  Qu'on veux  ajouter
  par exemple  je veux ajouter  apple  dans  la page  num 5  et on a  apple mais dans  page  3 c tt  pas dans  5
  alors la  on sors  avec   un 1;


  [+]  elle  retourne 3 si le  mot  existe  mais  dans une autre page apres la  page Qu'on veux  ajouter
  alors la prem_page   change
  par exemple on  veut  ajouter le mot apple  dans  la page  1  et nous on a  apple  dans la page 3


  [+] elle retourne 0  si le mot Qu'on veux ajouter  n'existe  pas  dans la liste

*/

int word (mot*  tete1[26],char ch[100],int z){

    mot *tete;
    tete = tete1[tolower(ch[0]) - 'a'];
    page *pg ;

    while (tete != NULL) {


    if (strcasecmp(tete->motindex,ch)==0){
       pg = tete->prempg;

       while (pg!=NULL) {

          if (pg->numpg==z){

            return 2;

          }
        pg = pg->next;
       }



    }
     tete=tete->next;

    }

     tete = tete1[tolower(ch[0]) - 'a'];

     while (tete != NULL) {


       if (strcasecmp(tete->motindex,ch)==0 && tete->derpg->numpg == z){

           return 2 ;

       } else if (strcasecmp(tete->motindex,ch)==0 && tete->derpg->numpg < z){

          return 1 ;
                    }   else if (strcasecmp(tete->motindex,ch)==0 && z < tete->prempg->numpg ) {

                        return 3 ;

                          }

       tete = tete->next;


     }
return 0 ;

}


/*

   la fonction  word2 " retourne  le ptr Qui  pointe vers le  mot  Qu'on veux l'ajouter  si il existe bien  sur
   par exemple  je veux ajouter  apple dans  page  2  et moi j'ai dans la  liste bcp de mot qui debute par "a"
   ex: amira(1234)==>anes(3466)===>apple(100)   alors il retourn avec 100  (address)

   la fonction word2 c'est la suite  de   la fonction word


*/

mot* word2 (mot* tete,char ch[100],int z){

     mot *ptr;
     ptr = tete;


     while (ptr != NULL) {


       if (strcasecmp(ptr->motindex,ch)==0 && ptr->derpg->numpg == z){

           return ptr ;

       } else if (strcasecmp(ptr->motindex,ch)==0 && ptr->derpg->numpg != z) {

           return ptr ;

       }

      ptr = ptr->next;

     }

     return NULL;

}




/*

     la procedure  creat_index   créé  l'index du fichier  text
     dont le nom  est  donne par l'utilisateur   ..

*/


void  creat_index (mot* *tab,char name[26]) {

page *ptrpg;
mot *ptr,*ptr1,*ptr2;
int s = 0, z = 0,f;

float num_page ;

for (s = 0 ; s <26 ; s++) {

    tab[s]=NULL;

}


s=0 ;   //  on a  utilise la variable  s  comme  compteur pour compter  le nbr  du  ligne  :)
char chain[100],chain1[100];
FILE *fp;
fp = fopen(name,"rt");
if (fp == NULL) {

                printf("\n\n\t     Impossible   d'ouvrir le fichier  \n") ;
                printf("  veuillez verifier Que le Fichier %s exist et dans le meme directory\n\n\n\n",name);
                Sleep(5000);
                exit (3);
                } else {
                        while (fgets (chain,100,fp)!= NULL) { // on prend une  ligne de 100 carectere

                        s = s +1 ; //  on a  utilise la variable  s  comme  compteur pour compter  le nbr  du  ligne  :)



                        if (strlen(chain) != 1 ) { // on  verifie si  la chaine  n'est pas vide  or \n  only


                        write(chain); //  on ecrase et on ecrit la ligne dans un fichier  tmp.txt


                        FILE *ft ; //  on ouvre le fichier  tmp.txt cette fois  en mode  lire  seulement
                         ft = fopen("tmp.txt","rt");

                        while (fscanf(ft,"%s",chain1) != EOF) {
                          float j ;
                            j = s ;    // on lit  jusqu"a  la  fin du fichier  tmp.txt  Qui contient une seul ligne
                                    num_page=ceil(j/lp);



                                     if (check(chain1) !=1 && strlen(chain1) > 2 && isalpha(chain1[0])) {







                                        if(tab[tolower(chain1[0]) - 'a'] == NULL){





                                        ptr =(mot *)malloc(sizeof(mot ));
                                        tab[tolower(chain1[0]) - 'a'] = ptr ;
                                        ptr->motindex=(char *)malloc(strlen(chain1)+1);
                                        strcpy(ptr->motindex,chain1);


                                        ptr->next=NULL;
                                        ptr->prempg=(page *)malloc(sizeof(page ));
                                        ptr->prempg->nbrocc=1;
                                        ptr->prempg->numpg=(int )num_page;
                                        ptr->prempg->next=NULL;
                                        ptr->derpg=(page *)malloc(sizeof(page));
                                        ptr->derpg=ptr->prempg;











                                        } else {

                                               ptr = tab[tolower(chain1[0]) - 'a'];


                                               z = word(tab,chain1,(int )num_page);

                                               switch (z) {


                                               case 3 :

                                               ptrpg = ptr->prempg;

                                               ptr->prempg=(page *)malloc(sizeof(page));
                                               ptr->prempg->nbrocc=1;

                                               ptr->prempg->numpg=(int )num_page;
                                               ptr->prempg->next = ptrpg;


                                               break ;

                                               case 2 :
                                               ptr = word2(tab[tolower(chain1[0]) - 'a'],chain1,(int )num_page);
                                               if (ptr==NULL){

                                                  printf("we have an error \n");
                                               }
                                                ptr->derpg->nbrocc++;





                                               break ;


                                               case 1 :

                                               ptr = word2(tab[tolower(chain1[0]) - 'a'],chain1,( int )num_page);
                                               if (ptr == NULL) {

                                                printf("we have problem");
                                               }


                                               ptrpg=(page *)malloc(sizeof(page ));

                                               ptr->derpg->next=ptrpg;
                                               ptr->derpg=ptrpg;
                                               ptrpg->next=NULL;
                                               ptrpg->nbrocc=1;
                                               ptrpg->numpg=(int )num_page;



                                               break ;

                                               case 0 :


                                               ptr1 = tab[tolower(chain1[0]) - 'a'] ;
                                               f=0;

                                                  while(strcasecmp(chain1,ptr->motindex)>0){
                                                      f=1;
                                                      ptr1 = ptr;
                                                      ptr = ptr->next;
                                                      if (ptr == NULL){

                                                        break;
                                                      }

                                                  }

                                               if (f==0){

                                                   ptr2 = (mot *)malloc(sizeof(mot ));
                                                   tab[tolower(chain1[0]) - 'a'] =ptr2;
                                                   ptr2->next=ptr1;
                                                   ptr2->motindex=(char *)malloc(strlen(chain1)+1);
                                                   strcpy(ptr2->motindex,chain1);
                                                   ptr2->prempg=(page *)malloc(sizeof(page ));
                                                   ptr2->prempg->nbrocc=1;
                                                   ptr2->prempg->numpg=(int )num_page;
                                                   ptr2->prempg->next=NULL;
                                                   ptr2->derpg=ptr2->prempg;




                                               }else {

                                               ptr2 = (mot *)malloc(sizeof(mot ));

                                               ptr2->motindex=(char *)malloc(strlen(chain1)+1);
                                               strcpy(ptr2->motindex,chain1);
                                               ptr2->prempg=(page *)malloc(sizeof(page ));
                                               ptr2->prempg->nbrocc=1;
                                               ptr2->prempg->numpg=(int)num_page;
                                               ptr2->prempg->next=NULL;
                                               ptr2->derpg=(page *)malloc(sizeof(page));
                                               ptr2->derpg=ptr2->prempg;



                                                         ptr2->next=ptr1->next;
                                                         ptr1->next=ptr2;

                                                         }

                                               break ;

                                              default :

                                                  printf("i guess we have a problem here   \n");


                                              break ;

                                               }




                                              }






                                                                                                }
                                                                                                        ;
                                                               }fclose(ft);
                                                              }
                                                              }remove("tmp.txt");
                        }


}

/*  cette procedure affiche  l'index  */

void afficher (mot* tab[26]){

mot *mo;
page *ptrpg;
char repo = 'y' ;
int j =0 ,i=0;

char alpha[27]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    while (j<26 && 'y' ==tolower(repo) ){
printf("\n");
printf("  %c \n\n",alpha[j]);

    if (tab[j]!= NULL) {
         i=0;mo = tab[j];
         while (i<4){
            while (mo != NULL){

            printf("\n");
printf("     %-20s",mo->motindex);
ptrpg=mo->prempg;
printf("pg:%-5d  nbr Occ:%-5d\n",ptrpg->numpg,ptrpg->nbrocc);
ptrpg=mo->prempg->next;
                   while(ptrpg != NULL){

printf("                         pg:%-5d  nbr Occ:%-5d\n",ptrpg->numpg,ptrpg->nbrocc);
ptrpg=ptrpg->next;


                   }


            mo=mo->next;
            }


        i++;
         }

         }
    if (j%4 == 3) {
    printf("\n\n\n voulez vous continiuez l'affichage  ===>(y/n)\n");
      scanf(" %c",&repo);


         }
    j++;
    }


}


void welcom () {


printf("#########################Welcom######################\n");
printf("#=======================TP Algo=====================#\n");
printf("#[+]ce program a ete code par:                      #\n");
printf("#    LAHLOUK ISHAK  S:1  G:2   MAT:201400007549     #\n");
printf("#    DECHICHA DAHMANE S:1 G:2  MAT:201400007984     #\n");
printf("#####################################################\n");
printf("#un programme Qui cree et Gere l'index d'un Fichier #\n");
printf("#####################################################\n\n\n\n");

}

void menu (){

printf("\n\n\n\n VEUILLEZ CHOISIR L'UN DES FONCTIONNALITéS SUIVANTES 1..5 :\n\n\n");
printf(" [+]- 1) Afficher l'index sur écran. \n");
printf(" [+]- 2) Afficher l'index d'un mot . \n");
printf(" [+]- 3) Ajouter un mot à l'index à une page donneé \n");
printf(" [+]- 4) Enlever un mot de l'index D'une page donneé\n");
printf(" [+]- 5) Sauvegarder l'index dans un Fichier\n");
printf(" [+]- 6)  Quit (le programme)\n\n\n\n");
printf(" 0 < F < 6 ===========| Fonctionnalité choisi :     ");

}



// la procedure   index_mot  affiche l'index  d'un  mot donne

void index_mot (mot* tab[26],char chain[50]){


        if (check(chain) ==1 || strlen(chain) < 3 || !isalpha(chain[0])){

          printf(" \n\n  le mot %s n'exist pas dans le fichier => n'est pas dans l'index \n\n\n",chain);
          return ;

        }
page *ptrpg;
mot* ptr;

        ptr = tab[tolower(chain[0]) - 'a'];


        while (ptr != NULL && strcasecmp(chain,ptr->motindex) != 0){

           ptr=ptr->next;
        }

        if (ptr == NULL) {

          printf(" \n\n  le mot %s n'exist pas dans le fichier => n'est pas dans l'index \n",chain);


        } else {

            printf(" ######### ====>l'index de mot :|  %s  |:#########\n\n\n",chain);
            printf("     %-20s",ptr->motindex);
            ptrpg=ptr->prempg;
            printf("pg:%-5d  nbr Occ:%-5d\n",ptrpg->numpg,ptrpg->nbrocc);
            ptrpg=ptrpg->next;
                   while(ptrpg != NULL){

            printf("                         pg:%-5d  nbr Occ:%-5d\n",ptrpg->numpg,ptrpg->nbrocc);
            ptrpg=ptrpg->next;


                   }



         return ;
        }






}



//  la  procedure  ajout_mot  comme son nom l'indique  ajoute  un mot donne
//  dans  l'index
void ajout_mot  (mot **tableau,char chainn[50],int k){


mot *ptr,*ptr1,*ptr2;
page *ptrpg;
int z,f;





                           if(tableau[tolower(chainn[0]) - 'a'] == NULL){





                                        ptr =(mot *)malloc(sizeof(mot ));
                                        tableau[tolower(chainn[0]) - 'a'] = ptr ;
                                        ptr->motindex=(char *)malloc(strlen(chainn)+1);
                                        strcpy(ptr->motindex,chainn);
                                        ptr->next=NULL;
                                        ptr->prempg=(page *)malloc(sizeof(page ));
                                        ptr->prempg->nbrocc=1;
                                        ptr->prempg->numpg=k;
                                        ptr->prempg->next=NULL;
                                        ptr->derpg=(page *)malloc(sizeof(page));
                                        ptr->derpg=ptr->prempg;











                                        } else {

                                               ptr = tableau[tolower(chainn[0]) - 'a'] ;


                                               z = word(tableau,chainn,k);
                                               printf("======================> Z :%d\n",z);

                                               switch (z) {

                                               case 3 :

                                               ptrpg = ptr->prempg;
                                               ptr->prempg=NULL;
                                               ptr->prempg=(page *)malloc(sizeof(page));
                                               ptr->prempg->nbrocc=1;
                                               ptr->prempg->numpg=k;
                                               ptr->prempg->next = ptrpg;


                                               break ;

                                               case 2 :
                                               ptr = word2(tableau[tolower(chainn[0]) - 'a'],chainn,k);
                                               if (ptr==NULL){

                                                  printf("we have an error \n");
                                               }
                                                ptr->derpg->nbrocc++;
                                                ptr->derpg->next=NULL;




                                               break ;


                                               case 1:

                                               ptr = word2(tableau[tolower(chainn[0]) - 'a'],chainn,k);
                                               if (ptr == NULL) {

                                                printf("we have problem");
                                               }


                                               ptrpg=(page *)malloc(sizeof(page ));

                                               ptr->derpg->next=ptrpg;
                                               ptr->derpg=ptrpg;
                                               ptrpg->next=NULL;
                                               ptrpg->nbrocc=1;
                                               ptrpg->numpg=k;



                                               break ;

                                               case 0:


                                               ptr1 = tableau[tolower(chainn[0]) - 'a'] ;
                                               f=0;

                                                  while(strcasecmp(chainn,ptr->motindex)>0){
                                                      f=1;
                                                      ptr1 = ptr;
                                                      ptr = ptr->next;
                                                      if (ptr == NULL){

                                                        break;
                                                      }

                                                  }

                                               if (f==0){

                                                   ptr2 = (mot *)malloc(sizeof(mot ));
                                                   tableau[tolower(chainn[0]) - 'a'] =ptr2;
                                                   ptr2->next=ptr1;
                                                   ptr2->motindex=(char *)malloc(strlen(chainn)+1);
                                                   strcpy(ptr2->motindex,chainn);
                                                   ptr2->prempg=(page *)malloc(sizeof(page ));
                                                   ptr2->prempg->nbrocc=1;
                                                   ptr2->prempg->numpg=k;
                                                   ptr2->prempg->next=NULL;
                                                   ptr2->derpg = (page * )malloc(sizeof(page));
                                                   ptr2->derpg=ptr2->prempg;




                                               }else {

                                               ptr2 = (mot *)malloc(sizeof(mot ));

                                               ptr2->motindex=(char *)malloc(strlen(chainn)+1);
                                               strcpy(ptr2->motindex,chainn);
                                               ptr2->prempg=(page *)malloc(sizeof(page ));
                                               ptr2->prempg->nbrocc=1;
                                               ptr2->prempg->numpg=k;
                                               ptr2->prempg->next=NULL;
                                               ptr2->derpg=(page *)malloc(sizeof(page));
                                               ptr2->derpg=ptr2->prempg;



                                                         ptr2->next=ptr1->next;
                                                         ptr1->next=ptr2;

                                                         }

                                               break ;



                                               }




                                              }





}




void suprm (mot **tab,char ch1[50]){

mot *ptr , *ptr1 ;

ptr = tab[tolower(ch1[0]) - 'a'];
        if (ptr->motindex==ch1){

           tab[tolower(ch1[0]) - 'a']=ptr->next;

         return ;

        } else  { ptr1 = ptr;
      while (strcasecmp(ch1,ptr->motindex)!=0){
           ptr1 = ptr;
           ptr=ptr->next;

      }

         ptr1->next = ptr->next;
         return ;

      }




}



void  enlever  (mot* *tab,char cha[50],int paj) {

mot*  ptr;
page *ptrpage ,*ptrpage1;

ptr = tab[tolower(cha[0])-'a'];



              while((strcasecmp(ptr->motindex,cha) < 0) && (ptr != NULL) ) {


                   ptr=ptr->next;



              } if(strcasecmp(ptr->motindex,cha) > 0) {

                 printf("le mot %s  n'exist pas dans l'index    \n\n\n\n\n",cha);


                 return ;

                } else {


                       ptrpage=ptr->prempg;
                       if (ptrpage->numpg==paj && ptrpage->next==NULL){

                          suprm(tab,cha);
                          return;


                       } else if (ptrpage->numpg==paj && ptrpage->next != NULL){


                          ptr->prempg=ptr->prempg->next;
                          return ;

                       }

                        else {


                         ptrpage = ptr->prempg;
                         ptrpage1=ptrpage;

                       while (ptrpage != NULL) {


                                      if (ptrpage->numpg==paj){



                                       ptrpage1->next=ptrpage->next;
                                       return ;


                                        }

                                        ptrpage1=ptrpage;
                                        ptrpage=ptrpage->next;

                                         }



                                         if (ptrpage == NULL){


                                          printf("le mot exist mais pas  dans  la page Que  vous avez Donnez :(  \n\n\n\n");

                                           }


                          }



              }






}



void write_to_file(char namef[50],mot*  tab[26]){


int  i=0,j=0 ;

FILE *fptr;
mot *mo;
page *ptrpg;

fptr = fopen(namef,"a+");

if (fptr == NULL){

     printf("Erreur    ===> can't Create   The  File     \n");
     return;

} else {


     char alpha[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    while (j<26){
fprintf(fptr,"\n");
fprintf(fptr,"  %c \n\n",alpha[j]);

    if (tab[j]!= NULL) {
         i=0;mo = tab[j];
         while (i<4){
            while (mo != NULL){

            fprintf(fptr,"\n");
fprintf(fptr,"     %-20s",mo->motindex);
ptrpg=mo->prempg;
fprintf(fptr,"pg:%-5d  nbr Occ:%-5d\n",ptrpg->numpg,ptrpg->nbrocc);
ptrpg=mo->prempg->next;
                   while(ptrpg != NULL){

fprintf(fptr,"                         pg:%-5d  nbr Occ:%-5d\n",ptrpg->numpg,ptrpg->nbrocc);
ptrpg=ptrpg->next;


                   }


            mo=mo->next;
            }


        i++;
         }

         }

    j++;
    }





}



}

/*  Done  */


int main () {
mot*  tabtet[26];
char name [30],mot_tr[30],cont;
int num;

int a;


welcom();
printf("    Donnez  Le  nom  de Fichier     ===> :");
scanf("%s",name);

creat_index(tabtet,name);

skip:
menu();


do {

     scanf("%d",&a);
     if (a<1 || a > 6) {

         printf("\n\n Entrez un nombre Correct SVP  1 <= nombre <= 6 |:");

     }


} while(a<1 || a > 6);


switch (a){

case 1 :

afficher(tabtet);

break ;



case 2 :

printf("\n\n  Donnez  le MOT ==> :");
scanf("%s",mot_tr);
index_mot(tabtet,mot_tr);

break ;

case 3 :



printf("#################################################\n");
printf("\n Donnez  la chain a Ajouter   ==>|:");
scanf(" %s",mot_tr);
printf("\n Donnez la page   ==> |:");
scanf("%d",&num);
printf("\n\n#################################################\n");


ajout_mot(tabtet,mot_tr,num);

break ;


case 4 :


printf("#################################################\n");
printf("\n Donnez  la chain a enlever  ==>|:");
scanf(" %s",mot_tr);
printf("\n Donnez la page   ==> |:");
scanf("%d",&num);
printf("\n\n#################################################\n");
enlever(tabtet,mot_tr,num);


break ;


case 5:


printf("#################################################\n");
printf("\n Donnez le   Nom de Fichier a Creer  ==>|:");
scanf(" %s",mot_tr);
printf("\n\n#################################################\n");

write_to_file(mot_tr,tabtet);


break ;

case 6 :

   printf("\n\n #####################   GOOD  Bye ... !!  #######################\n\n");
   exit(1);

break ;



}

printf("voulez  vous  continuez  (aller au menu principale)  (y/n)");
scanf(" %c",&cont);
if (tolower(cont)=='y'){

   goto skip;

} else {
     exit(1) ;

     };

 system ("PAUSSE");
return 0 ;
}
