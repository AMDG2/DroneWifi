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

Séance du vendredi 8 février 2012 (3h + 1h30)
---------------------------------------------
- Baudouin
 	- Adaptation du programme Qt en Dynamic C pour le Rabbit
- Thibaut
	- Soudage de la carte joystick
