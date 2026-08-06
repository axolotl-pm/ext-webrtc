--TEST--
A peer cannot queue an unbounded number of data channels
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;

function options() : PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setMaxMessageSize(262144)
		->setIceTcpEnabled(false);
}

function waitFor(callable $condition, string $what, float $seconds = 30.0) : void
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

const OPENED = 300;

$offerer = new PeerConnection(options());
$answerer = new PeerConnection(options());

// Queued channels are held by shared_ptr, so each one keeps itself and anything
// it received alive until PHP collects it. A peer that opens channels faster
// than they are collected would otherwise decide how much memory this process
// uses, so the queue has a ceiling and the rest are dropped.
$channels = [];
for ($i = 0; $i < OPENED; $i++) {
	$channels[] = $offerer->createDataChannel("channel-$i");
}

waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
$answerer->setRemoteOffer($offerer->getLocalDescription());
waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
$offerer->setRemoteAnswer($answerer->getLocalDescription());

waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");

$collected = 0;
$deadline = microtime(true) + 10.0;
while (microtime(true) < $deadline) {
	$collected += count($answerer->pollDataChannels());
	usleep(50000);
}

var_dump($collected > 0);
var_dump($collected < OPENED);


$offerer->close();
$answerer->close();

// closing releases anything still queued rather than holding it for as long as
// PHP keeps the object around
var_dump($answerer->pollDataChannels());
echo "done", PHP_EOL;
?>
--EXPECT--
bool(true)
bool(true)
array(0) {
}
done
