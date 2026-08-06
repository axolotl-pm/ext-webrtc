--TEST--
An offer produces a complete answer with the expected transport attributes
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;

$offer = implode("\r\n", [
	'v=0',
	'o=rtc 1234567890 0 IN IP4 127.0.0.1',
	's=-',
	't=0 0',
	'a=group:BUNDLE 0',
	'a=msid-semantic:WMS *',
	'a=ice-options:ice2,trickle',
	'a=fingerprint:sha-256 BF:A6:F6:27:33:78:4D:CA:74:B1:15:94:F1:AC:66:35:0B:BF:95:3B:E1:27:27:1E:A7:CC:95:05:02:48:C3:C3',
	'm=application 61374 UDP/DTLS/SCTP webrtc-datachannel',
	'c=IN IP4 127.0.0.1',
	'a=mid:0',
	'a=sendrecv',
	'a=sctp-port:5000',
	'a=max-message-size:262144',
	'a=setup:actpass',
	'a=ice-ufrag:dy94',
	'a=ice-pwd:Q4T4/6fJaBllzmqgSv33yF',
	'a=candidate:1 1 UDP 2114977791 127.0.0.1 61374 typ host',
	'a=end-of-candidates',
	'',
]);

$connection = new PeerConnection(
	PeerConnectionOptions::create()
		->setMaxMessageSize(262144)
		->setIceTcpEnabled(false)
);

$connection->setRemoteOffer($offer);

$deadline = microtime(true) + 15.0;
while ($connection->getGatheringState() !== GatheringState::COMPLETE) {
	if (microtime(true) > $deadline) {
		echo "timed out waiting for ICE gathering", PHP_EOL;
		exit(1);
	}
	usleep(20000);
}

$answer = $connection->getLocalDescription();

// attributes that must reflect real transport state
var_dump((bool) preg_match('/^a=ice-ufrag:\S+/m', $answer));
var_dump((bool) preg_match('/^a=ice-pwd:\S+/m', $answer));
var_dump((bool) preg_match('/^a=fingerprint:sha-256 [0-9A-F:]+/m', $answer));
var_dump((bool) preg_match('/^a=sctp-port:5000\r?$/m', $answer));
var_dump((bool) preg_match('/^a=max-message-size:262144\r?$/m', $answer));
var_dump(preg_match_all('/^a=candidate:/m', $answer) > 0);

// data channels only, no media
var_dump(preg_match_all('/^m=/m', $answer));
var_dump((bool) preg_match('#^m=application \d+ UDP/DTLS/SCTP webrtc-datachannel\r?$#m', $answer));

// RFC 5763: an answerer facing "actpass" must pick "active"
preg_match('/^a=setup:(\S+)/m', $answer, $matches);
var_dump($matches[1]);

// the getter must agree with what ended up in the SDP
preg_match('/^a=fingerprint:sha-256 ([0-9A-F:]+)/m', $answer, $matches);
var_dump($connection->getLocalFingerprint() === $matches[1]);

$connection->close();
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
int(1)
bool(true)
string(6) "active"
bool(true)
