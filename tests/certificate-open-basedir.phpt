--TEST--
Certificate paths honour open_basedir
--EXTENSIONS--
webrtc
--INI--
open_basedir={PWD}
--FILE--
<?php
use pmmp\webrtc\PeerConnectionOptions;

$outside = __DIR__ . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR . 'README.md';

var_dump(is_file(__FILE__));

try {
	PeerConnectionOptions::create()->setCertificate($outside, $outside);
	echo "BUG: accepted a path outside open_basedir", PHP_EOL;
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
%sArgument #1 ($certPemFile) must be a readable file ("%sREADME.md": path is restricted by PHP's open_basedir setting)
