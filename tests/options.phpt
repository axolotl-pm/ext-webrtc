--TEST--
PeerConnectionOptions setters are chainable and round-trip
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\IceServer;
use pmmp\webrtc\PeerConnectionOptions;

$defaults = PeerConnectionOptions::create();
var_dump($defaults->getMaxMessageSize());
var_dump($defaults->getIceServers());
var_dump($defaults->getBindAddress());
var_dump($defaults->getCertificatePemFile());
var_dump($defaults->isIceTcpEnabled());

$options = PeerConnectionOptions::create();
var_dump($options->setMaxMessageSize(262144) === $options);

$pem = __FILE__;

$options
	->setPortRange(50000, 50100)
	->setBindAddress("127.0.0.1")
	->setCertificate($pem, $pem, "secret")
	->setIceTcpEnabled(true)
	->setIceServers(
		IceServer::stun("a.example.com"),
		IceServer::turn("b.example.com", 3478, "user", "pass"),
	);

var_dump($options->getMaxMessageSize());
var_dump($options->getPortRangeBegin());
var_dump($options->getPortRangeEnd());
var_dump($options->getBindAddress());
var_dump($options->getCertificatePemFile() === $pem);
var_dump($options->getKeyPemFile() === $pem);
var_dump($options->getKeyPemPass());
var_dump($options->isIceTcpEnabled());

$servers = $options->getIceServers();
var_dump(count($servers));
var_dump($servers[0]->getHostname());
var_dump($servers[1]->isTurn());

var_dump($options->setBindAddress(null)->getBindAddress());
?>
--EXPECT--
int(0)
array(0) {
}
NULL
NULL
bool(false)
bool(true)
int(262144)
int(50000)
int(50100)
string(9) "127.0.0.1"
bool(true)
bool(true)
string(6) "secret"
bool(true)
int(2)
string(13) "a.example.com"
bool(true)
NULL
