--TEST--
Two connections negotiate and exchange messages over a data channel
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\DataChannel;
use pmmp\webrtc\DataChannelOptions;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;

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

$outgoing = $offerer->createDataChannel(
	"reliable",
	DataChannelOptions::create()->setProtocol("test")
);
var_dump($outgoing instanceof DataChannel);
var_dump($outgoing->getLabel());
var_dump($outgoing->getProtocol());

waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
$answerer->setRemoteOffer($offerer->getLocalDescription());

waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
$offerer->setRemoteAnswer($answerer->getLocalDescription());

waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");

// the answerer never creates a channel; it receives the one the offerer opened
$incoming = [];
waitFor(function () use ($answerer, &$incoming): bool {
	$incoming = array_merge($incoming, $answerer->pollDataChannels());
	return count($incoming) > 0;
}, "incoming channel");

var_dump(count($incoming));
var_dump($incoming[0]->getLabel());
var_dump($incoming[0]->getProtocol());

// draining twice must not hand the same channel over again
var_dump($answerer->pollDataChannels());

waitFor(fn() => $outgoing->isOpen() && $incoming[0]->isOpen(), "channels to open");

var_dump($outgoing->getMaxMessageSize());

// once a candidate pair is selected both ends report host:port
var_dump((bool) preg_match('/^\S+:\d+$/', (string) $offerer->getLocalAddress()));
var_dump((bool) preg_match('/^\S+:\d+$/', (string) $offerer->getRemoteAddress()));
var_dump($offerer->getRemoteAddress() === $answerer->getLocalAddress());

// binary payloads must survive untouched, including NUL bytes
$payload = "\x00\x01\xfe\xffhello";
$outgoing->send($payload);

waitFor(fn() => $incoming[0]->getAvailableAmount() > 0, "message");

var_dump($incoming[0]->peek() === $payload);
var_dump($incoming[0]->receive() === $payload);
var_dump($incoming[0]->receive());

// a large message exercises SCTP fragmentation
$large = str_repeat("x", 200000);
$outgoing->send($large);
waitFor(fn() => $incoming[0]->getAvailableAmount() > 0, "large message");
var_dump(strlen($incoming[0]->receive()));

$outgoing->close();
$offerer->close();
$answerer->close();
echo "done", PHP_EOL;
?>
--EXPECT--
bool(true)
string(8) "reliable"
string(4) "test"
int(1)
string(8) "reliable"
string(4) "test"
array(0) {
}
int(262144)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
NULL
int(200000)
done
