--TEST--
Channels the peer already closed do not hold pending slots
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\ConnectionState;
use pmmp\webrtc\DataChannel;
use pmmp\webrtc\GatheringState;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;

const CEILING = 4;
const FILLERS = 6;

function options(?int $pending) : PeerConnectionOptions
{
	$options = PeerConnectionOptions::create()->setIceTcpEnabled(false);
	if ($pending !== null) {
		$options->setMaxPendingDataChannels($pending);
	}
	return $options;
}

function waitFor(callable $condition, string $what, float $seconds = 30.0) : void
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

/**
 * @return PeerConnection[] the peer and the server, already connected, with the
 *                          server's queue drained of the channel used to
 *                          bootstrap the transport
 */
function connectedPair() : array
{
	$peer = new PeerConnection(options(null));
	$server = new PeerConnection(options(CEILING));

	$bootstrap = $peer->createDataChannel("bootstrap");
	waitFor(fn() => $peer->getGatheringState() === GatheringState::COMPLETE, "the peer to gather");
	$server->setRemoteOffer($peer->getLocalDescription());
	waitFor(fn() => $server->getGatheringState() === GatheringState::COMPLETE, "the server to gather");
	$peer->setRemoteAnswer($server->getLocalDescription());
	waitFor(fn() => $peer->getState() === ConnectionState::CONNECTED, "the connection");
	waitFor(fn() => $bootstrap->isOpen(), "the bootstrap channel");

	waitFor(function () use ($server) : bool {
		return count($server->pollDataChannels()) > 0;
	}, "the bootstrap channel to arrive");

	return [$peer, $server];
}

function labelsOf(array $channels) : array
{
	return array_map(fn(DataChannel $channel) => $channel->getLabel(), $channels);
}

// A channel still open occupies a slot, so the ceiling refuses what comes after
// it. That is the whole point of the setting and must keep working. Nothing
// polls until the end here, because polling is what frees the slots.
[$peer, $server] = connectedPair();
$live = [];
for ($i = 0; $i < FILLERS; $i++) {
	$live[] = $peer->createDataChannel("live-$i");
}
usleep(1000000);

$peer->createDataChannel("after-live");
usleep(1000000);

$collected = $server->pollDataChannels();
var_dump(count($collected));
var_dump(in_array("after-live", labelsOf($collected), true));
$peer->close();
$server->close();

// An empty channel the peer closed cannot be handed to anyone, so holding a
// slot for it only crowds out the next real channel.
[$peer, $server] = connectedPair();
for ($i = 0; $i < FILLERS; $i++) {
	$dead = $peer->createDataChannel("dead-$i");
	$dead->close();
	unset($dead);
}
usleep(1000000);

$peer->createDataChannel("after-dead");
usleep(1000000);

$labels = labelsOf($server->pollDataChannels());
var_dump(in_array("after-dead", $labels, true));
$peer->close();
$server->close();

// Closing after sending is a legitimate way to use a channel, so one that still
// holds messages keeps its slot even while empty ones around it are recycled.
[$peer, $server] = connectedPair();
$sender = $peer->createDataChannel("sent-then-closed");
waitFor(fn() => $sender->isOpen(), "the sending channel");
$sender->send("still readable");
usleep(300000);
$sender->close();
unset($sender);

for ($i = 0; $i < FILLERS; $i++) {
	$dead = $peer->createDataChannel("noise-$i");
	$dead->close();
	unset($dead);
}
usleep(1000000);

$peer->createDataChannel("after-noise");
usleep(1000000);

$kept = null;
$labels = [];
foreach ($server->pollDataChannels() as $channel) {
	$labels[] = $channel->getLabel();
	if ($channel->getLabel() === "sent-then-closed") {
		$kept = $channel;
	}
}
var_dump(in_array("after-noise", $labels, true));
var_dump($kept?->receive());
$peer->close();
$server->close();
?>
--EXPECT--
int(4)
bool(false)
bool(true)
bool(true)
string(14) "still readable"
