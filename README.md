Le problème du dîner des cinq philosophes
______________________________________________

Le dîner des philosophes est un problème particulièrement intéressant, car il met en œuvre dans sa réalisation, deux techniques d'utilisations différentes des sémaphores : l'exclusion mutuelle classique, mais aussi la possibilité de bloquer un processus grâce à un sémaphore privé.


Présentation du problème
___________________________

Considérons cinq philosophes, installés autour d'une table circulaire, et qui passent leurs temps à penser et à manger.

La table est mise avec cinq fourchettes qui sont disposés entre chacun des philosophes. 
De temps en temps, un philosophe a faim et essaye de prendre les fourchettes qui sont immédiatement a cotés de lui (ceux qui sont entre lui et son voisin de gauche et de droite). Un philosophe a besoin de  deux fourchettes pour manger, et ne peut évidemment pas prendre un fourchette qui est dans la main d'un voisin. 
Quand un philosophe affamé a ses deux fourchettes dans les mains en même temps, il mange sans libérer ses fourchettes. Dans le cas contraire, il doit attendre que ceux-ci deviennent libres.

Enfin, quand il a finit de manger, il repose ses deux fourchettes et commence à penser à nouveau. 
Précisons que les philosophes mangent et pensent durant des temps aléatoires différents, leur changements d'état, se produisent donc de manière totalement asynchrone. 

Le problème même du dîner des philosophes consiste donc à réguler les changements d'état de ces derniers suivant la disponibilité des fourchettes, qui dépend bien entendu de l'état des voisins du philosophe concerné. 


Pour réaliser ce problème, nous allons supposer que, pour chaque philosophe, nous allons attribuer un processus dans la machine. 
L'état des philosophes sera stocké dans un tableau alloué dans un segment de mémoire partagé. 

Résolution
________________________

L'exclusion mutuelle sur la table d'état des philosophes

Le stockage de l'état des philosophes dans un tableau alloué en mémoire partagé, implique immédiatement l'usage d'un sémaphore d'exclusion mutuelle (mutex).

Quand un philosophe a faim et les fourchettes sont disponibles il mange ; sinon il attend.

Comment gérer le fait que le philosophe qui veut manger, attende avant de pouvoir le faire, et surtout sache lorsqu'il peut le faire ? 


Utilisation d'un sémaphore privé pour bloquer un processus

Pour faire patienter le philosophe qui veut manger, nous allons utiliser pour chacun des philosophes, un sémaphore privé initialisé à 0. 
Cette pratique va servir à bloquer (en endormant le processus) le philosophe pour le faire attendre. Ce sont ces voisins, lorsqu'ils arrêteront de manger, qui le réveilleront pour qu'il puisse manger à son tour. 


Ainsi, on peut alors décrire les procédures de changement d'état des philosophes, de la manière suivante : 

Philosophe désirant manger :
___________________________________

	Début 

	P(mutex) 

		Si les deux voisins immédiats ne mangent pas Alors 

			 V(sémaphore privé) 

			Etat = mange 

		Sinon 

			Etat = veut manger 

		FSi 

	V(mutex) 

	P(sémaphore privé) 

	mange ... 

	Fin 


Examinons maintenant les conditions pour que celui-ci soit libérer. 

Philosophe arrêtant de manger, passage à l'état "pense" : 
__________________________________________________________

	Début 

	P(mutex) 

	Si le voisin de gauche veut manger ET son voisin ne mange pas Alors 

		Etat du voisin = mange 

		V(sémaphore privé du voisin) 

	FSi 

	(même chose pour le voisin de droite) 

	Etat = pense 

	V(mutex) 

	pense ... 

	Fin 

Un processus endormit, c'est à dire bloqué en état "veut manger" juste avant de se mettre à manger, n'est réveillé que par un de ses voisins (de gauche ou de droite), lorsque celui-ci pose ses fourchettes et qu'il s'est assuré que l'autre voisin du processus endormit n'occupe pas ses fourchettes. 

Alors, il effectue l'opération V sur le sémaphore privé du processus endormit : le sémaphore redevient positif , et le processus est réveillé, il poursuit son code en se mettant à manger. 


	NB : V : sem_post, P : sem_wait


Le code est écrit en C dans le présente projet.







