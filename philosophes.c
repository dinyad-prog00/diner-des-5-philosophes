#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NB_PHILOSOPHE 5

#define G(i) ((NB_PHILOSOPHE+i-1)%NB_PHILOSOPHE) //Gauche du philosophe
#define D(i) ((NB_PHILOSOPHE+i+1)%NB_PHILOSOPHE) //Droite du philosophe

typedef enum {reflechir,faim,manger} etat;

char * philoName[NB_PHILOSOPHE]={"Socrate","Descartes","Fleud","Platon","Nietzsche"};


etat Etat[NB_PHILOSOPHE]={reflechir,reflechir,reflechir,reflechir,reflechir};

sem_t S[NB_PHILOSOPHE];

sem_t mutex;

//Test : verifie si le philosophe peut manger. Si oui le debloque.
void Test(int i)
{
	if(Etat[i]==faim && Etat[D(i)]!=manger && Etat[G(i)]!=manger)
	{
		Etat[i]=manger;
		sem_post(&S[i]);
	}
}

//TestG : verifie si le philosophe de gauche peut manger. Si oui le debloque.
void TestG(int i)
{
	if(Etat[G(i)]==faim && Etat[G(G(i))]!=manger)
	{
		Etat[G(i)]=manger;
		sem_post(&S[G(i)]);
	}
}

//TestD : verifie si le philosophe de droite peut manger. Si oui le debloque.
void TestD(int i)
{
	if(Etat[D(i)]==faim && Etat[D(D(i))]!=manger)
	{
		Etat[D(i)]=manger;
		sem_post(&S[D(i)]);
	}

}

//Affiche les philosophes à table.
void affPhilo()
{
	printf("           	  %s              \n\n\n"
	       "      %s           %s        \n\n\n\n"
	       "           %s        %s\n\n\n______________________________________________\n\n",philoName[0] ,philoName[1] ,philoName[4] ,philoName[2] ,philoName[3] );
}


void * philosophe(void *num)
{
	int i= *(int*)num;
	int nb=2;
	int duree_mg=rand() % 10 + 1; //Durée de prise de repas généré aléatoirement entre 1 et 10 pour chaque philosophe.
	int duree_ref=rand() % 10 + 1; //Durée de reflexion.
	

	printf("Au debut le philosophe [ %d ]:%s reflechit...(%ds)\n\n",i,philoName[i], duree_ref);
	sleep(duree_ref);

	printf("\033[0;31mLe philosophe [ %d ]:%s a faim.\033[0m\n\n",i,philoName[i]);

	sem_wait(&mutex);
	Etat[i]=faim;
	Test(i);
	sem_post(&mutex);

	sem_wait(&S[i]); //Attend le déblocage pour manger.

	printf("\033[0;34mLe philosophe [ %d ]:%s mange...(%ds)\033[0m\n\n",i,philoName[i],duree_mg);
	sleep(duree_mg);

	printf("\033[0;32mLe philosophe [ %d ]:%s a fini de manger.\033[0m\n\n",i,philoName[i] );

		
	sem_wait(&mutex);
		
	TestD(i);
	TestG(i);
		
	Etat[i]=reflechir;
	sem_post(&mutex);

	printf("\033[0mLe philosophe [ %d ] reflechit...\033[0m\n\n",i);
	

	pthread_exit(NULL);

}


int main(int argc, char const *argv[])
{
	int i,indix[NB_PHILOSOPHE];
	pthread_t ph[NB_PHILOSOPHE];

	sem_init(&mutex,0,1);

	affPhilo();

	//Initialisation des sémaphores privé initialisé à 0.
	for (i = 0; i < NB_PHILOSOPHE ; ++i)
	{
		sem_init(&S[i],0,0);
		indix[i]=i;
	}

	//Création des threads pour chaque philosophe.
	for (i = 0; i < NB_PHILOSOPHE ; ++i)
		pthread_create(&ph[i],NULL,philosophe,indix+i);

	//On attend pour que chaque philosophes mange...
	for (int i = 0; i < NB_PHILOSOPHE; ++i)
		pthread_join(ph[i],NULL);
	
	printf("Tous les philosophes ont mangé !!\n\nFIN\n\n");

	return 0;
}