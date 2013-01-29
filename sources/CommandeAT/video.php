<?php
$host = "192.168.1.1";
$port_video = "5555";

$stdin = fopen("php://stdin", "r");

echo "Ouverture de la socket\n";
$socket_video = fsockopen("" .$host, $port_video);
if(!$socket_video)
{
	echo "Erreur d'ouverture de la Socket\n";
	echo "Goodbye\n";
}
else
{
	while(1)
	{
		fwrite($socket_video, "");
		//fgetc($stdin);
		usleep(500000);
	}
}
?>