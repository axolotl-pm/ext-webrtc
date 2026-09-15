--TEST--
getFailureState() is null on an active connection, records peer disconnection, and throws after close()
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

function options(): PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setMaxMessageSize(262144)
		->setIceTcpEnabled(false);
}

function waitFor(callable $condition, string $what, float $seconds = 20.0): void
{
	$deadline = microtime(true) + $seconds;
	while (!$condition()) {
		if (microtime(true) > $deadline) {
			echo "timed out waiting for $what", PHP_EOL;
			exit(1);
		}
		usleep(10000);
	}
}

$offerer = new PeerConnection(options());
$answerer = new PeerConnection(options());

var_dump($offerer->getFailureState());

$channel = $offerer->createDataChannel("liveness");

waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
$answerer->setRemoteOffer($offerer->getLocalDescription());

waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
$offerer->setRemoteAnswer($answerer->getLocalDescription());

waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");
waitFor(fn() => $channel->isOpen(), "channel to open");

var_dump($offerer->getFailureState());
var_dump($answerer->getFailureState());

// the peer closing transitions the offerer directly to CLOSED as observed
// by getState(), while the failure state remains accessible
$answerer->close();
waitFor(fn() => $offerer->getState() === ConnectionState::CLOSED, "peer close");
var_dump($offerer->getFailureState());

$offerer->close();
try {
	$offerer->getFailureState();
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
NULL
NULL
NULL
enum(pmmp\webrtc\ConnectionState::DISCONNECTED)
PeerConnection is closed
