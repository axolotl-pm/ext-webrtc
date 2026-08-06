--TEST--
The receive allowance is shared, enforced, and given back
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

const BUDGET = 1048576;
const MESSAGE = 262143;
const THREE = MESSAGE * 3;

function options() : PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setMaxMessageSize(262144)
		->setIceTcpEnabled(false)
		->setMaxReceiveQueueSize(BUDGET)
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

$first = $offerer->createDataChannel("first");

waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
$answerer->setRemoteOffer($offerer->getLocalDescription());
waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
$offerer->setRemoteAnswer($answerer->getLocalDescription());
waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");

/**
 * @param resource|null $unused
 */
function collect(PeerConnection $answerer, string $what) : object
{
	$found = [];
	waitFor(function () use ($answerer, &$found) : bool {
		$found = array_merge($found, $answerer->pollDataChannels());
		return count($found) > 0;
	}, $what);

	waitFor(fn() => $found[0]->isOpen() || $found[0]->isClosed(), "$what to settle");

	return $found[0];
}

$firstIn = collect($answerer, "the first channel");
waitFor(fn() => $first->isOpen(), "the first channel to open");

// three quarters of the allowance, left unread
for ($i = 0; $i < 3; $i++) {
	$first->send(str_repeat("x", MESSAGE));
}
waitFor(fn() => $firstIn->getAvailableAmount() >= THREE, "the first channel to fill");
var_dump($firstIn->getAvailableAmount() === THREE);
var_dump($firstIn->isClosed());

// a second channel shares the same allowance rather than getting its own, so it
// runs out and is closed instead of doubling what this connection can hold
$second = $offerer->createDataChannel("second");
$secondIn = collect($answerer, "the second channel");
waitFor(fn() => $second->isOpen(), "the second channel to open");

for ($i = 0; $i < 3; $i++) {
	if (!$second->isClosed()) {
		$second->send(str_repeat("y", MESSAGE));
	}
}

waitFor(fn() => $secondIn->isClosed(), "the second channel to be closed for overrunning");
var_dump($firstIn->getAvailableAmount() + $secondIn->getAvailableAmount() <= BUDGET);

try {
	$secondIn->receive();
	echo "BUG: reading from an overrun channel succeeded", PHP_EOL;
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}

// dropping a channel unread has to hand its share back, or the allowance stays
// spent for as long as the connection lives
$first->close();
$second->close();
unset($first, $second, $firstIn, $secondIn);
gc_collect_cycles();

$third = $offerer->createDataChannel("third");
$thirdIn = collect($answerer, "the third channel");
waitFor(fn() => $third->isOpen(), "the third channel to open");

for ($i = 0; $i < 3; $i++) {
	if (!$third->isClosed()) {
		$third->send(str_repeat("z", MESSAGE));
	}
}

waitFor(fn() => $thirdIn->getAvailableAmount() >= THREE, "the third channel to fill from the returned allowance");
var_dump($thirdIn->getAvailableAmount() === THREE);
var_dump($thirdIn->isClosed());

$offerer->close();
$answerer->close();
echo "done", PHP_EOL;
?>
--EXPECT--
bool(true)
bool(false)
bool(true)
DataChannel exceeded its receive queue limit and lost messages
bool(true)
bool(false)
done
