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
%smust be greater than 0
%smust be between 1 and 65535
%smust be between the start port and 65535
TypeError
Error
