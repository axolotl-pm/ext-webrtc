--TEST--
IceCandidate parses a candidate line and rejects a malformed one
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\IceCandidate;
use pmmp\webrtc\WebRtcException;

$candidate = IceCandidate::create("candidate:1 1 UDP 2122317823 192.0.2.10 54321 typ host");
var_dump($candidate->getCandidate());
var_dump($candidate->getMid());

// the "a=" prefix is what appears in an SDP, so both spellings have to work
$prefixed = IceCandidate::create("a=candidate:1 1 UDP 2122317823 192.0.2.10 54321 typ host");
var_dump($prefixed->getCandidate() === $candidate->getCandidate());

var_dump(IceCandidate::create("candidate:1 1 UDP 2122317823 192.0.2.10 54321 typ host", "video")->getMid());

try {
	IceCandidate::create("not a candidate");
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}

// an empty line parses into a placeholder candidate rather than failing, so it
// has to be turned away before it reaches libdatachannel
try {
	IceCandidate::create("");
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	new IceCandidate();
} catch (Error $e) {
	echo get_class($e), PHP_EOL;
}
?>
--EXPECTF--
string(54) "candidate:1 1 UDP 2122317823 192.0.2.10 54321 typ host"
string(1) "0"
bool(true)
string(5) "video"
Invalid candidate format
%smust not be empty
Error
