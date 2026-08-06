--TEST--
IceServer factories expose host, port and credentials
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\IceServer;
use pmmp\webrtc\RelayType;

$stun = IceServer::stun("stun.example.com");
var_dump($stun->getHostname());
var_dump($stun->getPort());
var_dump($stun->isTurn());
var_dump($stun->getUsername());
var_dump($stun->getPassword());
var_dump($stun->getRelayType());

var_dump(IceServer::stun("stun.example.com", 19302)->getPort());

$turn = IceServer::turn("turn.example.com", 3478, "user", "pass");
var_dump($turn->isTurn());
var_dump($turn->getUsername());
var_dump($turn->getPassword());
var_dump($turn->getRelayType() === RelayType::UDP);

var_dump(IceServer::turn("turn.example.com", 5349, "user", "pass", RelayType::TLS)->getRelayType() === RelayType::TLS);
?>
--EXPECT--
string(16) "stun.example.com"
int(3478)
bool(false)
NULL
NULL
NULL
int(19302)
bool(true)
string(4) "user"
string(4) "pass"
bool(true)
bool(true)
