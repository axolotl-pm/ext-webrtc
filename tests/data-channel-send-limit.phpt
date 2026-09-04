--TEST--
send() refuses once the outgoing queue reaches its limit
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

const SEND_LIMIT = 1024;
const PAYLOAD = 262143;

function options() : PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setMaxMessageSize(262144)
		->setIceTcpEnabled(false)
		->setMaxSendQueueSize(SEND_LIMIT)
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

$channel = $offerer->createDataChannel("outgoing");

waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
$answerer->setRemoteOffer($offerer->getLocalDescription());
waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
$offerer->setRemoteAnswer($answerer->getLocalDescription());
waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");
waitFor(fn() => $channel->isOpen(), "the channel to open");

// The transport buffers whatever it cannot write yet and never refuses on its
// own, so without this limit a peer that stops reading would decide how much
// memory this process uses.
$payload = str_repeat("x", PAYLOAD);
$threw = false;
$message = "";
for ($i = 0; $i < 2000; $i++) {
	try {
		$channel->send($payload);
	} catch (WebRtcException $e) {
		$threw = true;
		$message = $e->getMessage();
		break;
	}
}

var_dump($threw);
var_dump(str_contains($message, "send queue"));
var_dump($channel->getBufferedAmount() >= SEND_LIMIT);

$offerer->close();
$answerer->close();
echo "done", PHP_EOL;
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
done
