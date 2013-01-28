<?php
// Ouvrir $sock = fsockopen($host, $port);
// Lire fgets($sock);
// Ecrire fputs($sock, $str);

$host = "192.168.1.1";
$port = "5556";
$ident = 0;
$tmpAT;

$stdin = fopen("php://stdin", "r");

echo "Ouverture de la socket";
$socket = fsockopen("udp://" .$host, $port);
if(!$socket)
{
	echo "Erreur d'ouverture de la Socket\n";
	echo "Goodbye\n";
}
else
{
	echo "Test d'envoi de donnée\n";

	echo "Configuration du drone\n";
	$tmpAT = "AT*CONFIG=" . ++$ident . ",\"control:altitude_max\",\"2000\"";
	echo "Commande AT: " . $tmpAT . "\n";
	fwrite($socket, $tmpAT);

	echo "Décollage !!!\n";
	echo "Commande AT: ";
	$tmpAT = "AT*REF=" . ++$ident .",290718208\r";
	echo $tmpAT . "\n";
	fwrite($socket, $tmpAT);

	fgetc($stdin);

	echo "Atterissage...\n";
	$tmpAT = "AT*REF=" . ++$ident .",290718208\r";
	echo "Commande AT: " . $tmpAT . "\n";
	fwrite($socket, $tmpAT);
}

?>