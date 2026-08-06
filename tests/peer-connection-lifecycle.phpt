--TEST--
PeerConnection construction, close() and use after close
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

$connection = new PeerConnection(PeerConnectionOptions::create()->setMaxMessageSize(262144));

var_dump($connection->getState() === ConnectionState::NOT_STARTED);
var_dump($connection->getGatheringState() === GatheringState::NOT_STARTED);
var_dump($connection->getLocalDescription());
var_dump($connection->getLocalFingerprint());

$connection->close();
$connection->close();

try {
	$connection->getState();
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	new PeerConnection("not options");
} catch (TypeError $e) {
	echo get_class($e), PHP_EOL;
}
?>
--EXPECT--
bool(true)
bool(true)
NULL
NULL
PeerConnection is closed
TypeError
