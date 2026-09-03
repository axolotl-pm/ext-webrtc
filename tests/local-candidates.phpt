--TEST--
Gathered candidates are handed over once each, and a remote one needs a description first
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\IceCandidate;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

function options(): PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setIceTcpEnabled(false)
		->setBindAddress("127.0.0.1");
}

function gather(PeerConnection $connection, string $label): void
{
	$connection->createDataChannel($label);

	$deadline = microtime(true) + 15.0;
	while ($connection->getGatheringState() !== GatheringState::COMPLETE) {
		if (microtime(true) > $deadline) {
			echo "timed out waiting for ICE gathering", PHP_EOL;
			exit(1);
		}
		usleep(20000);
	}
}

$connection = new PeerConnection(options());

// nothing has been gathered and no description applied yet
var_dump($connection->pollLocalCandidates());

try {
	$connection->addRemoteCandidate(
		IceCandidate::create("candidate:1 1 UDP 2122317823 192.0.2.10 54321 typ host")
	);
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}

gather($connection, "trickle");

// COMPLETE has to mean every candidate has been handed over, not just that the
// connection stopped gathering, or a caller that polls once here loses them
$candidates = $connection->pollLocalCandidates();
var_dump(count($candidates) > 0);
var_dump($candidates[0] instanceof IceCandidate);
var_dump((bool) preg_match('/^candidate:\S+ \d+ (UDP|TCP) \d+ /i', $candidates[0]->getCandidate()));
var_dump($candidates[0]->getMid());

// taking them is what empties the queue, so a second call sees nothing
var_dump($connection->pollLocalCandidates());

// every gathered candidate also ends up in the description
$sdp = $connection->getLocalDescription();
var_dump(preg_match_all('/^a=candidate:/m', $sdp) === count($candidates));

$connection->close();
var_dump($connection->pollLocalCandidates());
?>
--EXPECT--
array(0) {
}
Got a remote candidate without remote description
bool(true)
bool(true)
bool(true)
string(1) "0"
array(0) {
}
bool(true)
array(0) {
}
