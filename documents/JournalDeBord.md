Journal de bord – projet de fin d’année
=======================================

Séance du mardi 15 janvier 2013 (4h?)
-------------------------------------
- Choix du projet
- Recherche de bibliographie sur le WiFi
- Écriture de la *liste des tâches*

Séance du vendredi 17 janvier 2013 (4h?)
----------------------------------------
- Diagramme de Gantt
- Installation du *SDK Rabbit*
- Lecture de la documentation de la carte et du SDK
 Tentatives infructueuses de compilation d’exécution d’un code d’exemple sur la carte RCM5600W

Séance du lundi 21 janvier 2013 (4h)
------------------------------------
- On a réussi à compiler et à envoyer un programme sur la carte
- Test du module WiFi :
    - Connexion avec le Drone
    - Utilisation d’un programme d’exemple

Séance du mardi 22 janvier 2013 (4h)
------------------------------------
- Finalisation du diagramme de Gantt
- Programmation de l'interface WiFi

Séance du vendredi 25 janvier 2013 (3h)
---------------------------------------
- Baudouin
    - Étude des *commandes AT*
    - Essai d'envoi des commandes
- Thibaut
    - Codage de la fonction de *connexion WiFi*
    - Succès de la connexion au WiFi

Séance du lundi 28 janvier 2013 (4h)
------------------------------------
- Baudouin
    - Envoi de commandes AT, le drone s'envole et atterit
    - écriture d'un programme utilisant les sockets pour envoyer des commandes
- Thibaut
    - Automatisation de la connexion au WiFi
    - Essais d'envoi de commande AT avec les sockets Dynamic C

Séance du mardi 29 janvier 2013 (3h)
------------------------------------
- Baudouin
    - Suite de l'étude des commandes AT
    - Récupération des flux vidéos avec ffplay
- Thibaut
    - Mise en parallèle d'un condensateur de 10mF pour les pics de courants : on penseait que le drone se reposait à cause d'une baisse de courant, mais c'est juste parce qu'il faut lui envoyer des commandes en continu
	- Envoi de trames UDP
	- écriture de documentation

Séance extra du jeudi 31 janvier 2013 (3h)
------------------------------------------
- Baudouin
    - Programmation d'un logiciel de commande du drone avec QT dans le but d'étudier les différentes commandes possibles
- Thibaut
    - Réglage du problème d'alimentation : coupe du câble à la moitié

Séance du vendredi 1er février 2013 (3h)
----------------------------------------
- Recherche d'un câble HDMI/DVI pour le raspberry pi
- Baudouin
    - Continue sur le logiciel
- Thibaut
    - Création de la carte de joystick
    - Recherche sur la conversion analogique/numérique sur le Rabbit

Séance du lundi 4 février 2013 (4h)
-----------------------------------
- Baudouin
	- fin de test des commandes AT
	- écriture d'un nouveau programme Qt plus structuré pour meilleure portabilité vers Dynamic C
- Thibaut
	- Renseignements sur la conversion analogique -> numérique directement avec le Rabbit 5600W : ce n'est pas possible
	- Recherche d'un composant pouvant faire la conversion de 4 signaux analogiques (2 joysticks (x, y pour chaque)) et dialoguant en I²C
	- Refonte du routage pour ces nouveaux élements
	- Lecture de la documentation I²C du Rabbit pour trouver les broches correspondante et voir si on peut les modifier.
- Essai des commandes AT disponibles : tourner sur lui-même horaire/antihoraire ; monter/descendre ; incliner avant-arrière / gauche-droite

Séance du mardi 5 février 2013 (3h + 1h30 (+ 1h30 pour Thibaut))
--------------------------------------------------------------
- Baudouin
	- Finition du programme Qt avec commandes par touche de clavier
- Thibaut
	- Modification du fil d'alimentation pour pouvoir avoir un fil plus long (donc fil de plus grosse section)
	- Renseignements sur le bus I²C avec le Rabbit : on peut finalement changer les bits du port D (lui, immuable) pour l'horloge et la data du bus. Ainsi, on peut utiliser l'I²C et le convertisseur qui était prévu
	- Finition du routage
	- Ecriture de deux fonctions pour lire et écrire les informations de la carte joystick

Séance du vendredi 8 février 2013 (3h + 1h30)
---------------------------------------------
- Baudouin
 	- Adaptation du programme Qt en Dynamic C pour le Rabbit
- Thibaut
	- Soudure de la carte joystick

Séance du lundi 11 février 2013 (4h)
------------------------------------
- Le drone à un problème d'hélices, il ne vole plus correctement
- Baudouin
 	- Fin de la réécriture du programme Qt en Dynamic C
 	- Essai de récupération de données depuis le drone
- Thibaut
	- Débugage de la carte
	- Écriture des fonctions d'E/S

Séance du mardi 12 février 2013 (3h + 4h)
-----------------------------------------
- Réception du Raspberry Pi
- Achat de nouvelles hélices pour le drone
- Après avoir laissé les hélices du drone droites pendant une journée, le drone vole mieux...
- Baudouin
	- Écriture de la fonction principale (main) pour la télécommande : gestion de la connexion, de la lecture des commandes, de l'envoi des commandes...
- Thibaut
	- Recherche sur les problème autour de l'I²C : résistance de tirage pour le bit de données du bus I²C, configuration des ports, etc...
	- Étude du bus I²C sur le Rabbit : écriture d'une fonction de configuration des deux bits utilisés sur le port D (la fonction fournie ne fonctionne pas...)
	- et tests... Il semble, à la fin de la séance, qu'un des bits (1) du port D ne fonctionne plus

Séance du vendredi 15 février 2013 (3h)
---------------------------------------
- Baudouin
	- Découverte de l'environnement du Raspberry Pi
	- Test de la récupération de la vidéo sur le RPi : échoué
	- Travail sur les données de navigation
- Thibaut
	- Changement de méthode pour la récupération des valeurs analogiques, bus I²C -> SPI : changement du circuit intégré, modification de la carte, changement du connecteur, écriture de routines pour le programme

Séance extra du samedi 16 février 2013 - Portes ouvertes (Baudouin : 9h ; Thibaut : 2h30)
-----------------------------------------------------------------------------------------
- Accueil des lycéens, démonstration du projet, réponse aux questions...
- Thibaut
	- Finalisation du changement en SPI, réglages des temps. La conversion fonctionne et est lue par le Rabbit

Séance du lundi 18 février 2013 (4h + 1h)
-----------------------------------------
- Mise en commun des travaux pour le "programme final"
- Résolution des derniers problèmes
- Travail sur l'oral de mi-projet
- Thibaut
	- Finalisation de la conversion avec le SPI : réglage de bugs dans le programme

Séance du mardi 19 février 2013 - Oraux de mi-parcours (3h + 2h30)
------------------------------------------------------------------
- Oral de mi-parcours : passage sur le projet et écoute du projet de Paul de Poix et Antoine Jouet
- Résolution de problèmes du programme sur le Rabbit : le drone vole commandé avec la télécommande !
- Test de vol : test des joysticks, test des boutons, test de toutes les procédures possibles
- Résolution du sens des joysticks

Séance extra du jeudi 21 février 2013 (2h (+ 4h pour Thibaut))
--------------------------------------------------------------
- Baudouin
- Thibaut
	- Modification du programme pour plus de sureté de fonctionnement
	- Tests de la carte, les conversions ne fonctionnent plus. Après test, il semble que tout fonctionne (horloge, choix du canal) même la réponse du convertisseur jusqu'à la patte. Cependant, le programme lit uniquement des 1 sur la patte Dout pendant quelques secondes (ce qui fait une conversion égale à 4095) puis que des 0 (conversion égale à 0)
	- Création d'une nouvelle carte plus ergonomique, avec convertisseur SPI, grand connecteur, boutons des joysticsk pris en compte, et sans les erreurs de la première carte.

Séance du vendredi 22 février (4h)
-----------------------------------
- Thibaut
	- Réparation de la première carte : c'était un problème de contact

Séance du lundi 25 février (4h)
--------------------------------
- Thibaut
	- Soudure de la seconde carte
	- Tests

Séance du mardi 26 février (3h)
-------------------------------
- Thibaut
	- Modification du programme pour fonctionner avec la seconde carte
	- Tests

Séance du vendredi 1er mars (4h pour Thibaut)
----------------------------------------------
- Thibaut
	- Tentatives de récupération de la vidéo sur le Raspberry Pi
	- Tests croisés avec un ordinateur plus puissant : résultats incohérents

Séance du lundi 11 mars (4h)
----------------------------
- Thibaut
	- Tentatives de récupération de la vidéo sur le Raspberry Pi
	- Tests de configuration du drone pour baisser le nombre de frames par seconde de la vidéo

Séance du mardi 12 mars (3h)
----------------------------
- Thibaut
	- Tentatives de récupération de la vidéo sur le Raspberry Pi
	- Tests de configuration du drone : il faut avant obtenir le droit de changer ces configurations... Tentatives
