--TEST--
PeerConnectionOptions validates its arguments
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\PeerConnectionOptions;

$options = PeerConnectionOptions::create();

try {
	$options->setMaxMessageSize(0);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setMaxMessageSize(256 * 1024 * 1024 + 1);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setCertificate("cert.pem\0/../../secret.pem", "key.pem");
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setCertificate(__DIR__ . "/no-such-cert.pem", __FILE__);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setCertificate(__FILE__, __DIR__ . "/no-such-key.pem");
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setPortRange(0, 100);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setPortRange(200, 100);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setIceServers("not an ice server");
} catch (TypeError $e) {
	echo get_class($e), PHP_EOL;
}

try {
	new PeerConnectionOptions();
} catch (Error $e) {
	echo get_class($e), PHP_EOL;
}
?>
--EXPECTF--
%smust be between 1 and 268435456
%smust be between 1 and 268435456
%smust not contain any null bytes
%sArgument #1 ($certPemFile) must be a readable file ("%sno-such-cert.pem": no such file or directory)
%sArgument #2 ($keyPemFile) must be a readable file ("%sno-such-key.pem": no such file or directory)
%smust be between 1 and 65535
%smust be between the start port and 65535
TypeError
Error
