--TEST--
WEBRTC_VERSION reports the linked libdatachannel version
--EXTENSIONS--
webrtc
--FILE--
<?php
var_dump(is_string(pmmp\webrtc\WEBRTC_VERSION));
var_dump((bool) preg_match('/^\d+\.\d+\.\d+$/', pmmp\webrtc\WEBRTC_VERSION));
?>
--EXPECT--
bool(true)
bool(true)
