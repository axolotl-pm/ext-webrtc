--TEST--
ICE UDP mux puts every connection on one port, and rules out TURN
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\IceServer;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

function options(bool $mux): PeerConnectionOptions
{
	return PeerConnectionOptions::create()
		->setIceTcpEnabled(false)
		->setBindAddress("127.0.0.1")
		->setPortRange(51200, 51299)
		->setIceUdpMuxEnabled($mux);
}

/** @return int[] */
function gatherPorts(PeerConnection $connection): array
{
	$connection->createDataChannel("mux");

	$deadline = microtime(true) + 15.0;
	while ($connection->getGatheringState() !== GatheringState::COMPLETE) {
		if (microtime(true) > $deadline) {
			echo "timed out waiting for ICE gathering", PHP_EOL;
			exit(1);
		}
		usleep(20000);
	}

	preg_match_all(
		'/^a=candidate:\S+ \d+ UDP \d+ 127\.0\.0\.1 (\d+) typ host/mi',
		$connection->getLocalDescription(),
		$matches
	);
	return array_map('intval', $matches[1]);
}

/** @return int[] */
function gatherAll(bool $mux, int $count): array
{
	$ports = [];
	$connections = [];

	for ($i = 0; $i < $count; $i++) {
		$connections[] = $connection = new PeerConnection(options($mux));
		$ports = array_merge($ports, gatherPorts($connection));
	}
	foreach ($connections as $connection) {
		$connection->close();
	}
	return $ports;
}

var_dump(PeerConnectionOptions::create()->isIceUdpMuxEnabled());
var_dump(options(true)->isIceUdpMuxEnabled());

// Without UDP mux, each connection binds to a distinct port.
$ports = gatherAll(false, 3);
var_dump(count($ports) === 3);
var_dump(count(array_unique($ports)) === 3);

// With UDP mux, all connections share the same port.
$ports = gatherAll(true, 3);
var_dump(count($ports) === 3);
var_dump(count(array_unique($ports)) === 1);

// UDP mux cannot be used together with TURN servers.
try {
	new PeerConnection(
		options(true)->setIceServers(IceServer::turn("127.0.0.1", 3478, "user", "pass"))
	);
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}

// STUN is supported with UDP mux.
$connection = new PeerConnection(
	options(true)->setIceServers(IceServer::stun("127.0.0.1", 3478))
);
var_dump($connection instanceof PeerConnection);
$connection->close();
?>
--EXPECT--
bool(false)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
TURN servers cannot be used with ICE UDP mux
bool(true)
