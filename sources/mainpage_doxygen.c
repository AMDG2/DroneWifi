/**
*@mainpage Projet de deuxième année DUT Geii - Télécommande wifi pour A.R.Drone 2.0 Parrot

<h2>Documentation de la télécommande wifi pour A.R.Drone 2.0 Parrot</h2>

<p>Ce projet a pour but de créer une télécommande matérielle pour piloter l'AR.drone 2.0 de Parrot. Ce quadricopter commercial se commande normalement grâce à une application mobile par WiFi.
Il a été réalisé par Baudouin Feildel (<a href="https://twitter.com/amdg2">\@AMDG2</a>) et Thibaut Marty (<a href="https://twitter.com/ThibMarty">\@ThibMarty</a>) dans le cadre du projet de 2<sup>e</sup> année de DUT Génie Electrique et Informatique Industrielle de 90 heures à l'IUT d'Angers.</p>
<p>La documentation est divisée en plusieurs modules et sous-modules que voici :
	<ul>
		<li>libARDrone : fonctions et structure liées à la connexion et à l'initialisation du drone
			<ul>
				<li>Navdata : fonctions et structures liées à la récupération et l'analyse des données de navigation</li>
				<li>ATCommands : fonctions liées à au pilotage du drone via les commandes AT</li>
			</ul>
		</li>
		<li>Rabbit
			<ul>
				<li>Connexion : fonctions liées à l'utilisation de l'interface WiFi de module RCM5600W</li>
				<li>UDP : fonctions liées à l'envoi de datagramme UDP sur le réseau</li>
				<li>IO : fonctions et structure liées à l'utilisation de la carte de pilotage avec le module RCM5600W</li>
			</ul>
		</li>
	</ul>
	N.B. : Dans le module Rabbit vous trouverez le programme principal (main) de la télécommande.
</p>
<p>Les sources complètes sont disponibles sur le dépot GitHub du projet, vous pourrez y trouver tous les fichiers documentés ici, ainsi qu'une bibliographie complète, le schéma et le routage de la carte de pilotage, ainsi que différents documents concernant la restitution du projet à nos professeurs.<br>
Dépôt github : <a href="https://github.com/AMDG2/DroneWifi" title="Dépot github du projet">https://github.com/AMDG2/DroneWifi</a><br>
Pour le récupérer directement sur votre ordinateur : <span style="background-color: black; color: white; display: inline-block; padding: 2px 10px 2px 10px;">git clone <a href="https://github.com/AMDG2/DroneWifi.git" style="color: white; text-decoration: none;">https://github.com/AMDG2/DroneWifi.git</a></span>
</p>


*/
