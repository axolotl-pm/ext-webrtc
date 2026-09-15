<?php
/* Helper peer process for sctp-settings.phpt */
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;

$offer = "";
while (($line = fgets(STDIN)) !== false) {
	if ($line === "--end--\n") {
		break;
	}
	$offer .= $line;
}

$answerer = new PeerConnection(PeerConnectionOptions::create()->setIceTcpEnabled(false));
$answerer->setRemoteOffer($offer);
while ($answerer->getGatheringState() !== GatheringState::COMPLETE) {
	usleep(10000);
}

fwrite(STDOUT, $answerer->getLocalDescription() . "--end--\n");
fflush(STDOUT);

while (true) {
	sleep(1);
}
