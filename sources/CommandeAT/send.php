<?php
// Ouvrir $sock = fsockopen($host, $port);
// Lire fgets($sock);
// Ecrire fputs($sock, $str);

$host = "192.168.1.1";
$port = "5556";
$ident = 0;
$tmpAT;

$stdin = fopen("php://stdin", "r");

echo "Ouverture de la socket\n";
$socket_at = fsockopen("udp://" .$host, $port);
if(!$socket_at)
{
	echo "Erreur d'ouverture de la Socket\n";
	echo "Goodbye\n";
}
else
{
	echo "Test d'envoi de donnée\n";

	echo "Configuration du drone\n";
	$tmpAT = "AT*CONFIG=" . ++$ident . ",\"control:altitude_max\",\"2000\"\r";
	echo "Commande AT: " . $tmpAT . "\n";
	fwrite($socket_at, $tmpAT);

	echo "Configuration du drone\n";
	$tmpAT = "AT*CONFIG=" . ++$ident . ",\"general:navdata_demo\",\"TRUE\"\r";
	echo "Commande AT: " . $tmpAT . "\n";
	fwrite($socket_at, $tmpAT);

	echo "Configuration du drone\n";
	$tmpAT = "AT*FTRIM=" . ++$ident . ",\r";
	echo "Commande AT: " . $tmpAT . "\n";
	fwrite($socket_at, $tmpAT);

	echo "Décollage !!!\n";
	echo "Commande AT: ";
	$tmpAT = "AT*REF=" . ++$ident .",290718208\r";
	echo $tmpAT . "\n";
	fwrite($socket_at, $tmpAT);

	for($i = 0 ; $i < 10 ; $i++)
	{
	  usleep(1000);

	  echo "Vol stationnaire\n";
	  $tmpAT = "AT*PCMD=" . ++$ident . ",1,0,0,0,0\r";
	  echo "Commande AT: " . $tmpAT . "\n";
	  fwrite($socket_at, $tmpAT);
	}

	fgetc($stdin);

	echo "Atterissage...\n";
	$tmpAT = "AT*REF=" . ++$ident .",290717696\r";
	echo "Commande AT: " . $tmpAT . "\n";
	fwrite($socket_at, $tmpAT);
}

?>
