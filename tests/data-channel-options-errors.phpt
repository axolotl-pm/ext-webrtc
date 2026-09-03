--TEST--
DataChannelOptions validates its arguments
--EXTENSIONS--
webrtc
--FILE--
<?php
use pmmp\webrtc\DataChannelOptions;

$options = DataChannelOptions::create();

try {
	$options->setMaxRetransmits(-1);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setMaxRetransmits(4294967296);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$options->setMaxPacketLifeTime(-1);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

// libdatachannel narrows the lifetime to uint32_t when it opens the channel and
// throws where nothing surfaces it, leaving a channel that never opens and never
// reports itself closed. The setter has to be the one that refuses this.
try {
	$options->setMaxPacketLifeTime(4294967296);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

var_dump($options->getMaxPacketLifeTime());
var_dump($options->setMaxPacketLifeTime(4294967295)->getMaxPacketLifeTime());

try {
	$options->setId(65536);
} catch (ValueError $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	new DataChannelOptions();
} catch (Error $e) {
	echo get_class($e), PHP_EOL;
}
?>
--EXPECTF--
%smust be between 0 and 4294967295
%smust be between 0 and 4294967295
%smust be between 0 and 4294967295
%smust be between 0 and 4294967295
NULL
int(4294967295)
%smust be between 0 and 65535
Error
