--TEST--
The receive allowance counts messages, not only the bytes they measure
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

const MESSAGES = 64;
// deliberately generous, so nothing here can be attributed to the byte ceiling
const BYTES = 1048576;

function options() : PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setMaxMessageSize(262144)
		->setIceTcpEnabled(false)
		->setMaxReceiveQueueSize(BYTES)
		->setMaxReceiveQueueMessages(MESSAGES)
		->setMaxPendingDataChannels(0);
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

$offerer = new PeerConnection(options());
$answerer = new PeerConnection(options());

$channel = $offerer->createDataChannel("empty");

waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
$answerer->setRemoteOffer($offerer->getLocalDescription());
waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
$offerer->setRemoteAnswer($answerer->getLocalDescription());
waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");

$found = [];
waitFor(function () use ($answerer, &$found) : bool {
	$found = array_merge($found, $answerer->pollDataChannels());
	return count($found) > 0;
}, "the incoming channel");
$incoming = $found[0];
waitFor(fn() => $channel->isOpen(), "the channel to open");

// An empty message measures nothing, so the byte ceiling can never see it. Only
// the message ceiling stands between a peer and an unbounded queue.
for ($i = 0; $i < 100000 && !$channel->isClosed(); $i++) {
	$channel->send("");
}

waitFor(fn() => $incoming->isClosed(), "the channel to be closed for overrunning");

var_dump($incoming->getQueuedMessageCount() === MESSAGES);
var_dump($incoming->getAvailableAmount() === 0);

try {
	$incoming->receive();
	echo "BUG: reading from an overrun channel succeeded", PHP_EOL;
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}

$offerer->close();
$answerer->close();
echo "done", PHP_EOL;
?>
--EXPECT--
bool(true)
bool(true)
DataChannel exceeded its receive queue limit and lost messages
done
