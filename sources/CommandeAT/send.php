<?php
// Ouvrir $sock = fsockopen($host, $port);
// Lire fgets($sock);
// Ecrire fputs($sock, $str);

$host = "192.168.1.1";
$port_at = "5556";
$port_navdata = "5554";
$ident = 0;
$tmpAT;

$stdin = fopen("php://stdin", "r");

echo "Ouverture de la socket AT\n";
$socket_at = fsockopen("udp://" .$host, $port_at);
if(!$socket_at)
{
	echo "Erreur d'ouverture de la Socket AT\n";
	echo "Goodbye\n";
}
else
{
	echo "Ouverture de la socket Nav data\n";
	$socket_navdata = fsockopen("udp://" .$host, $port_at);
	if(!$socket_at)
	{
		echo "Erreur d'ouverture de la socket Nav data\n";
		echo "Goodbye\n";
	}
	else
	{
		echo "Envoi sur le port NAVDATA d'une donnée\n";
		$tmpAT = "12345";
		echo "Trame : " . $tmpAT . "\n";
		fwrite($socket_navdata, $tmpAT);
		
		echo "Envoi de l'activation des données de navigation\n";
		$tmpAT = "AT*CONFIG=" . ++$ident . ",\"general:navdata_demo\",\"TRUE\"\r";
		echo "Trame : " . $tmpAT . "\n";
		fwrite($socket_at, $tmpAT);
		
		echo "Envoi d'un 1 sur le port NAVDATA\n";
		$tmpAT = "12345\r";
		fwrite($socket_navdata, $tmpAT);
		
		echo "Envoi d'un ACK\n";
		$tmpAT = "AT*CTRL=" . ++$ident . ",5,0\r";
		fwrite($socket_at, $tmpAT);
		
		echo "Envoi d'un 1 sur le port NAVDATA\n";
		$tmpAT = "12345\r";
		fwrite($socket_navdata, $tmpAT);
	}
}

?>
