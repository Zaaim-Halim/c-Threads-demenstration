#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//############################## declaration des variables globals ###############################
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;      /* initialisation dynamique des mutex et des conditions */
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex0 = PTHREAD_MUTEX_INITIALIZER;
#define NPE 2500 // le nombre initiale des pansements dans le stock
#define NCI 250  // le nombre initiale des ciseaux dans le stock
#define NAN 1500 // le nombre initials des antiseptiques dans le stock
int npe_rest_dans_stock,nci_rest_dans_stock,nan_rest_dans_stock;// le nombre des pansements 
                                                                //,ciseaux,antiseptiques rest dans le stock
                                                                // apres le tirage de chaque medicine 
int npe,nci,nant; // le nombre des pansements ,ciseaux,antiseptiques tire par chaque medicine
int i;
//#################################################################################################
//############################## thread "medicine" ################################################
void* medicins(void *arg)
{  
   pthread_mutex_lock(&mutex0);
       int medecin_nb;
        medecin_nb = *((int*)arg);
    	srand(time(NULL));
		printf("\n\t\t# ----------------------------------------------------------------- #\n");
		printf("\t\t#\t          HALIM ZAAIM FSDM                                #\n");
		printf("\t\t#\t            groupe 4  SMI 2018/2019                         #");
		printf("\n\t\t# ----------------------------------------------------------------- #\n");
			do{
			npe=rand();
			}while(npe>80);
		printf("le medecin num %d va prendre: \n\t\t\t   %d pensements \n",medecin_nb,npe);
			do{
			nci=rand();
			}while(nci>15);		
		printf("\t\t\t   %d ciseaux \n",nci);
			do{
			nant=rand();
			}while(nant > 25);		
		printf("\t\t\t   %d antiseptiques \n",nant);
		
		npe_rest_dans_stock-=npe;	
		nci_rest_dans_stock-=nci;
		nan_rest_dans_stock-=nant;
		 
		if(npe_rest_dans_stock < 150){
			pthread_mutex_lock(&mutex1);
			pthread_cond_signal(&cond1);
			pthread_mutex_unlock(&mutex1);system("pause");			
		}
		if(nci_rest_dans_stock < 70){// change hereeeee
			pthread_mutex_lock(&mutex2);
			pthread_cond_signal(&cond2);
			pthread_mutex_unlock(&mutex2);
			system("pause");			
		}
		if(nan_rest_dans_stock < 180){
			pthread_mutex_lock(&mutex3);
			pthread_cond_signal(&cond3);
			pthread_mutex_unlock(&mutex3);system("pause");			
		}				
		
		printf("\n nombre de pensements dans le stock : npe_rest_dans_stock= %d\n nombre de ciseaux dans le stock : nci_rest_dans_stock=%d\n nombre des antiseptiques dans le stock : nan_rest_dans_stock= %d\n ",npe_rest_dans_stock,nci_rest_dans_stock,nan_rest_dans_stock);	
		sleep(1);
		system("cls");	// fonction pour vider terminal apres chaque affichage 
		pthread_mutex_unlock(&mutex0);
	    pthread_exit(NULL);
} 
//############################## thread "ciseaux" ##################################################
void* ciseaux(void* arg)
{
		while(1)
		{
			pthread_mutex_lock(&mutex2);
			pthread_cond_wait(&cond2,&mutex2);
			printf("\nil faut initialiser le stock de ciseaux car le  NCI= %d <70 \n",nci_rest_dans_stock);
			nci_rest_dans_stock=NCI;//initialiser le stock de ciseaux par la valeur initial
			printf("le nombre des ciseaux devient %d\n",nci_rest_dans_stock);
			pthread_mutex_unlock(&mutex2);
		}
}
//############################## thread "antiseptique" ################################################
void* antisptique(void* arg)
{    
        while(1)
        {
			pthread_mutex_lock(&mutex3);
			pthread_cond_wait(&cond3,&mutex3);
			printf("\nil faut initialiser le stock d'antiseptiques car le NAN= %d < 180 \n",nan_rest_dans_stock);
			nan_rest_dans_stock=NAN;////initialiser le stock d'antiseptiques par la valeur initial
			printf("le nombre des pendements devient %d\n",nan_rest_dans_stock);
			pthread_mutex_unlock(&mutex3);
		}
 }
 //############################## thread "pansements" ################################################ 
void* pensement(void* arg)
{ 
            while(1)
            {
	        pthread_mutex_lock(&mutex1);
			pthread_cond_wait(&cond1,&mutex1);
			printf("\nil faut initialiser le stock des pensements car le NPE= %d < 150 \n",npe_rest_dans_stock);
			npe_rest_dans_stock=NPE;//initialiser le stock des pensements par la valeur initial
			printf("le nombre des pendements devient %d\n",npe_rest_dans_stock);
			pthread_mutex_unlock(&mutex1);
		    }
		 
}
//###################################################################################################
int main(){
	
	pthread_t medecin[20];int j,me,ci,an,pe;
	pthread_t cis;
	pthread_t anti;
	pthread_t pend;
	srand(time(NULL));
	
	npe_rest_dans_stock = NPE;  
	nci_rest_dans_stock = NCI;     // initialisation des stocks //
	nan_rest_dans_stock = NAN;

    ci = pthread_create(&cis,NULL,ciseaux,NULL);
    if(ci != 0)
    {
    	printf("erreur de creation -thread ciseaux-");
    	exit(EXIT_FAILURE);
	}
	an = pthread_create(&anti,NULL,antisptique,NULL);
	if(an != 0)
	{
	  printf("erreur de creation -thread antiseptique-");
	  exit(EXIT_FAILURE);	
	}
    pe = pthread_create(&pend,NULL,pensement,NULL);
    if(pe != 0)
    {
       printf("erreur de creation -thread pensement-");	
       exit(EXIT_FAILURE);
	}
	
	  while(1)    /*  creation a neaveau de 20 thread 'medecine' a chaque fois les 20 threads sont executer */
	  {    
	   for(j=0;j<20;j++) /* creation de 20 thread 'medecin' */
	   {
	   	  i = rand()%20;    /* generate a random nuumber pour un thread medecine*/
	      me = pthread_create(&medecin[j],NULL,medicins,(void*)&i);//creation de thread 
	      if(me != 0)
	      {
	      	 printf("erreur de creation -thread medecin-");
	      	 exit(EXIT_FAILURE);
		  }
          if( 0 != pthread_join(medecin[j],NULL))
          {
          	printf("erreur d'attendre -thread medecine-");
		  }
          
       }//end for
	 }//end while
    
    //*************************** wait for the threads to terminate *******************************
	if(0 != pthread_join(cis,NULL))
	 {
	   printf("erreur d'attendre -thread ciseaux-"); 	
	 }                                                   //******************************************************
    if(0 != pthread_join(anti,NULL))                     //******************************************************
	{                                                    //******************************************************
	  printf("erreur d'attendre -thread antiseptique-"); //******************************************************
	}                                                    // ***** wait for threads : ciseaux, antiseptique, pansements
    if(0 != pthread_join(pend,NULL))                     //*******************************************************
	{                                                    //*******************************************************
		printf("erreur d'attendre -thread pensements-"); //*******************************************************
	}                                                    //*******************************************************
   //**************************************************************************************************	     
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2); // liberation des véreux
	pthread_mutex_destroy(&mutex3);
	return 0;
}

