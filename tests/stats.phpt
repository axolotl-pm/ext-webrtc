--TEST--
The byte counters follow real traffic and clearStats() resets them
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;

function options(): PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setIceTcpEnabled(false)
		->setBindAddress("127.0.0.1");
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

$outgoing = $offerer->createDataChannel("stats");

waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
$answerer->setRemoteOffer($offerer->getLocalDescription());
waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
$offerer->setRemoteAnswer($answerer->getLocalDescription());

waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");
waitFor(fn() => $outgoing->isOpen(), "channel");

$incoming = [];
waitFor(function () use ($answerer, &$incoming): bool {
	$incoming = array_merge($incoming, $answerer->pollDataChannels());
	return count($incoming) > 0;
}, "incoming channel");

$payload = str_repeat("s", 8192);
$outgoing->send($payload);
waitFor(fn() => $incoming[0]->receive() !== null, "delivery");

// the counters are per connection, so each side only sees its own direction
var_dump($offerer->getBytesSent() >= 8192);
var_dump($answerer->getBytesReceived() >= 8192);

// SCTP measures this rather than accumulating it
$rtt = $offerer->getRoundTripTime();
var_dump($rtt === null || $rtt >= 0);

$offerer->clearStats();
var_dump($offerer->getBytesSent());
var_dump($offerer->getBytesReceived());

// clearing one side leaves the other alone
var_dump($answerer->getBytesReceived() >= 8192);

$offerer->close();
$answerer->close();
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
int(0)
int(0)
bool(true)
