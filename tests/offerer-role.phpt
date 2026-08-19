--TEST--
An offerer drives the exchange through the signaling state
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\SignalingState;

$connection = new PeerConnection(
	PeerConnectionOptions::create()
		->setIceTcpEnabled(false)
		->setBindAddress("127.0.0.1")
);

var_dump($connection->getSignalingState());
var_dump($connection->isNegotiationNeeded());
var_dump($connection->getRemoteDescription());
var_dump($connection->getRemoteFingerprint());

// an m-line is what an offer is made of, so a channel has to exist first
var_dump($connection->getLocalDescription());

$connection->createDataChannel("ReliableDataChannel");

$deadline = microtime(true) + 15.0;
while ($connection->getGatheringState() !== GatheringState::COMPLETE) {
	if (microtime(true) > $deadline) {
		echo "timed out waiting for ICE gathering", PHP_EOL;
		exit(1);
	}
	usleep(20000);
}

// the state is what distinguishes an offer from an answer here
var_dump($connection->getSignalingState());

$offer = $connection->getLocalDescription();
var_dump((bool) preg_match('/^a=setup:actpass/m', $offer));
var_dump(preg_match_all('/^m=/m', $offer));

$connection->close();
?>
--EXPECT--
enum(pmmp\webrtc\SignalingState::STABLE)
bool(false)
NULL
NULL
NULL
enum(pmmp\webrtc\SignalingState::HAVE_LOCAL_OFFER)
bool(true)
int(1)
