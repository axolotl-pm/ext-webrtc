--TEST--
The remote description size limit is configurable and enforced before parsing
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

// The default is 64 KiB, and the setter is chainable and round-trips like the
// other limits.
var_dump(PeerConnectionOptions::create()->getMaxRemoteDescriptionSize());
$o = PeerConnectionOptions::create();
var_dump($o->setMaxRemoteDescriptionSize(4096) === $o);
var_dump($o->getMaxRemoteDescriptionSize());

$head =
	"v=0\r\no=- 0 0 IN IP4 0.0.0.0\r\ns=-\r\nt=0 0\r\n" .
	"m=application 9 UDP/DTLS/SCTP webrtc-datachannel\r\nc=IN IP4 0.0.0.0\r\n" .
	"a=ice-ufrag:aaaa\r\na=ice-pwd:bbbbbbbbbbbbbbbbbbbbbbbb\r\n" .
	"a=fingerprint:sha-256 AB:CD:EF:12:34:56:78:9A:BC:DE:F0:11:22:33:44:55:66:77:88:99:AA:BB:CC:DD:EE:FF:00:11:22:33:44:55\r\n" .
	"a=setup:actpass\r\na=mid:0\r\na=sctp-port:5000\r\n";
$candidates = fn(int $n) => str_repeat("a=candidate:0 1 udp 1 1.2.3.4 5000 typ host\r\n", $n);

function options(?int $limit) : PeerConnectionOptions
{
	$options = PeerConnectionOptions::create()->setIceTcpEnabled(false);
	if ($limit !== null) {
		$options->setMaxRemoteDescriptionSize($limit);
	}
	return $options;
}

// Under the default limit a description with thousands of candidate lines is
// refused, and because the refusal happens before the parser runs it is
// immediate rather than the seconds the parse would take.
$pc = new PeerConnection(options(null));
$start = microtime(true);
try {
	$pc->setRemoteOffer($head . $candidates(50000));
	echo "BUG: oversized offer accepted", PHP_EOL;
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}
try {
	$pc->setRemoteAnswer($head . $candidates(50000));
	echo "BUG: oversized answer accepted", PHP_EOL;
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}
var_dump(microtime(true) - $start < 1.0);
$pc->close();

// A tighter custom limit refuses a smaller description.
$pc = new PeerConnection(options(4096));
try {
	$pc->setRemoteOffer($head . $candidates(500));
	echo "BUG: accepted over custom limit", PHP_EOL;
} catch (WebRtcException $e) {
	echo $e->getMessage(), PHP_EOL;
}
$pc->close();

// Zero lifts the limit: the same large description reaches the parser, so any
// failure is about its contents rather than its size.
$pc = new PeerConnection(options(0));
$blockedBySize = false;
try {
	$pc->setRemoteOffer($head . $candidates(3000));
} catch (WebRtcException $e) {
	$blockedBySize = strpos($e->getMessage(), "byte limit") !== false;
}
var_dump($blockedBySize);
$pc->close();

// A normal-sized description still goes through the parser under the default
// limit, so the limit does not get in the way of real traffic.
$pc = new PeerConnection(options(null));
try {
	$pc->setRemoteOffer("this is not a valid sdp");
} catch (WebRtcException $e) {
	var_dump(strpos($e->getMessage(), "byte limit") === false);
}
$pc->close();

echo "done", PHP_EOL;
?>
--EXPECTF--
int(65536)
bool(true)
int(4096)
remote description is %d bytes, over the %d byte limit
remote description is %d bytes, over the %d byte limit
bool(true)
remote description is %d bytes, over the %d byte limit
bool(false)
bool(true)
done
