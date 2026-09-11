--TEST--
Both ends of the accepted MTU range still carry a message larger than the MTU
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;

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

function exchange(int $mtu): void
{
	$options = fn() => PeerConnectionOptions::create()
		->setIceTcpEnabled(false)
		->setBindAddress("127.0.0.1")
		->setMtu($mtu);

	$offerer = new PeerConnection($options());
	$answerer = new PeerConnection($options());

	$outgoing = $offerer->createDataChannel("mtu");

	waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");
	$answerer->setRemoteOffer($offerer->getLocalDescription());
	waitFor(fn() => $answerer->getGatheringState() === GatheringState::COMPLETE, "answer gathering");
	$offerer->setRemoteAnswer($answerer->getLocalDescription());

	// an MTU usrsctp rejects fails the connection here rather than at construction
	waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection at mtu $mtu");

	$incoming = [];
	waitFor(function () use ($answerer, &$incoming): bool {
		$incoming = array_merge($incoming, $answerer->pollDataChannels());
		return count($incoming) > 0;
	}, "incoming channel");
	waitFor(fn() => $outgoing->isOpen() && $incoming[0]->isOpen(), "channels to open");

	// many times the MTU, so it only arrives if every fragment fits the wire
	$payload = random_bytes(200000);
	$outgoing->send($payload);
	waitFor(fn() => $incoming[0]->getAvailableAmount() > 0, "message at mtu $mtu");
	var_dump($incoming[0]->receive() === $payload);

	$outgoing->close();
	$offerer->close();
	$answerer->close();
}

exchange(620);
exchange(4144);
echo "done", PHP_EOL;
?>
--EXPECT--
bool(true)
bool(true)
done
