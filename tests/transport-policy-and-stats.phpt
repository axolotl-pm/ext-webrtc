--TEST--
The transport policy round-trips and the stats counters start empty
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\IceServer;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\RelayType;
use pmmp\webrtc\TransportPolicy;

$options = PeerConnectionOptions::create()->setIceTcpEnabled(false);
var_dump($options->getIceTransportPolicy());

var_dump($options->setIceTransportPolicy(TransportPolicy::RELAY) === $options);
var_dump($options->getIceTransportPolicy());

try {
	$options->setIceTransportPolicy("relay");
} catch (TypeError $e) {
	echo get_class($e), PHP_EOL;
}

// nothing has been sent yet, and SCTP has not measured anything
$connection = new PeerConnection(
	PeerConnectionOptions::create()->setIceTcpEnabled(false)->setBindAddress("127.0.0.1")
);
var_dump($connection->getBytesSent());
var_dump($connection->getBytesReceived());
var_dump($connection->getRoundTripTime());

$connection->close();

// the counters go with the connection
try {
	$connection->getBytesSent();
} catch (Throwable $e) {
	echo get_class($e), PHP_EOL;
}
?>
--EXPECT--
enum(pmmp\webrtc\TransportPolicy::ALL)
bool(true)
enum(pmmp\webrtc\TransportPolicy::RELAY)
TypeError
int(0)
int(0)
NULL
pmmp\webrtc\WebRtcException
