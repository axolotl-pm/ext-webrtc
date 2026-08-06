--TEST--
IceServer rejects invalid ports and direct construction
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\IceServer;

try {
	IceServer::stun("stun.example.com", 70000);
} catch (ValueError $e) {
	echo get_class($e), PHP_EOL;
}

try {
	IceServer::turn("turn.example.com", 0, "user", "pass");
} catch (ValueError $e) {
	echo get_class($e), PHP_EOL;
}

try {
	new IceServer();
} catch (Error $e) {
	echo get_class($e), PHP_EOL;
}
?>
--EXPECT--
ValueError
ValueError
Error
