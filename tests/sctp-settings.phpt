--TEST--
set_sctp_settings() validates its arguments and detects an unresponsive peer
--EXTENSIONS--
webrtc
--SKIPIF--
<?php
if (!is_readable("/proc/self/cmdline")) die("skip /proc/self/cmdline is required to relaunch PHP with the same options");
?>
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use function pmmp\webrtc\set_sctp_settings;

function waitFor(callable $condition, string $what, float $seconds = 20.0): void
{
	$deadline = microtime(true) + $seconds;
	while (!$condition()) {
		if (microtime(true) > $deadline) {
			echo "timed out waiting for $what", PHP_EOL;
			exit(1);
		}
		usleep(10000);
	}
}

// the same php with the same options, so the peer loads the same extension
function peerCommand(): array
{
	$args = explode("\0", rtrim(file_get_contents("/proc/self/cmdline"), "\0"));
	$script = array_search("-f", $args, true);
	$options = array_slice($args, 1, ($script === false ? count($args) - 1 : $script) - 1);
	return [PHP_BINARY, ...$options, "-f", __DIR__ . "/sctp-settings-peer.inc"];
}

set_sctp_settings();
set_sctp_settings(10000, 5, 200, 10000, 1000);

foreach ([
	fn() => set_sctp_settings(heartbeatInterval: 0),
	fn() => set_sctp_settings(maxRetransmitAttempts: -1),
	fn() => set_sctp_settings(minRetransmitTimeout: 4294967296),
	fn() => set_sctp_settings(maxRetransmitTimeout: 0),
	fn() => set_sctp_settings(initialRetransmitTimeout: 0),
] as $call) {
	try {
		$call();
	} catch (ValueError $e) {
		echo $e->getMessage(), PHP_EOL;
	}
}

// three unanswered heartbeats a few hundred milliseconds apart, whereas
// the defaults rely on ICE consent checks after thirty seconds
set_sctp_settings(
	heartbeatInterval: 100,
	maxRetransmitAttempts: 2,
	minRetransmitTimeout: 100,
	maxRetransmitTimeout: 300,
	initialRetransmitTimeout: 300,
);

$offerer = new PeerConnection(PeerConnectionOptions::create()->setIceTcpEnabled(false));
$channel = $offerer->createDataChannel("liveness");
waitFor(fn() => $offerer->getGatheringState() === GatheringState::COMPLETE, "offer gathering");

$peer = proc_open(peerCommand(), [0 => ["pipe", "r"], 1 => ["pipe", "w"], 2 => ["file", "php://stderr", "w"]], $pipes);
var_dump(is_resource($peer));
// a peer left behind would keep the test runner's pipes open forever
register_shutdown_function(function () use ($peer): void {
	if (is_resource($peer)) {
		proc_terminate($peer, 9);
	}
});

fwrite($pipes[0], $offerer->getLocalDescription() . "--end--\n");
fflush($pipes[0]);

$answer = "";
while (($line = fgets($pipes[1])) !== false) {
	if ($line === "--end--\n") {
		break;
	}
	$answer .= $line;
}
$offerer->setRemoteAnswer($answer);

waitFor(fn() => $offerer->getState() === ConnectionState::CONNECTED, "connection");
waitFor(fn() => $channel->isOpen(), "channel to open");
var_dump($offerer->getFailureState());

proc_terminate($peer, 9);
waitFor(fn() => $offerer->getState() === ConnectionState::CLOSED, "peer disconnection", 10.0);
var_dump($offerer->getFailureState());

fclose($pipes[0]);
fclose($pipes[1]);
proc_close($peer);
$offerer->close();
echo "done", PHP_EOL;
?>
--EXPECT--
pmmp\webrtc\set_sctp_settings(): Argument #1 ($heartbeatInterval) must be between 1 and 4294967295
pmmp\webrtc\set_sctp_settings(): Argument #2 ($maxRetransmitAttempts) must be between 1 and 4294967295
pmmp\webrtc\set_sctp_settings(): Argument #3 ($minRetransmitTimeout) must be between 1 and 4294967295
pmmp\webrtc\set_sctp_settings(): Argument #4 ($maxRetransmitTimeout) must be between 1 and 4294967295
pmmp\webrtc\set_sctp_settings(): Argument #5 ($initialRetransmitTimeout) must be between 1 and 4294967295
bool(true)
NULL
enum(pmmp\webrtc\ConnectionState::DISCONNECTED)
done
