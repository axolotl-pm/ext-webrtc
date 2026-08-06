--TEST--
Objects release their properties, and a rejected call has no side effect
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\DataChannelOptions;
use pmmp\webrtc\PeerConnection;
use pmmp\webrtc\PeerConnectionOptions;
use pmmp\webrtc\WebRtcException;

// A custom free_obj handler replaces the standard one, so it has to run the
// standard destructor itself. Without that call everything held in the
// properties table outlives the object.
$options = DataChannelOptions::create();
@($options->child = new stdClass());
$weak = WeakReference::create($options->child);
unset($options);
var_dump($weak->get());

$before = memory_get_usage();
for ($i = 0; $i < 2000; $i++) {
	$object = DataChannelOptions::create();
	@($object->pad = str_repeat("x", 4096));
	unset($object);
}

var_dump(memory_get_usage() - $before < 1024 * 1024);

$connection = new PeerConnection(PeerConnectionOptions::create()->setIceTcpEnabled(false));

try {
	$connection->close("unexpected");
} catch (TypeError $e) {
	echo "close(): ", $e->getMessage(), PHP_EOL;
}

// still usable, so the rejected close() really did nothing
var_dump($connection->getState() !== null);

try {
	$connection->pollDataChannels("unexpected");
} catch (TypeError $e) {
	echo "pollDataChannels(): ", $e->getMessage(), PHP_EOL;
}

// a close() that is actually accepted does take effect
$connection->close();

try {
	$connection->getState();
	echo "BUG: still usable after close()", PHP_EOL;
} catch (WebRtcException $e) {
	echo "after close(): ", $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
NULL
bool(true)
close(): %s::close() expects exactly 0 arguments, 1 given
bool(true)
pollDataChannels(): %s::pollDataChannels() expects exactly 0 arguments, 1 given
after close(): PeerConnection is closed
