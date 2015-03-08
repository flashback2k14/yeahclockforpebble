var initialized = false;
var options = {};

Pebble.addEventListener("ready", function() {
  console.log("ready called!");
  initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
	console.log("showing configuration");
	Pebble.openURL('http://flashback2k14.github.io/yeahclockforpebble/index.html?' + encodeURIComponent(JSON.stringify(options)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed");
  //JSON validity and non-empty check
  if (e.response.charAt(0) === "{" && e.response.slice(-1) === "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
		//Send to Pebble, persist there
		Pebble.sendAppMessage({"KEY_INVERT": options.selectInvert, "KEY_SHOW_DATE": options.selectShowDate},
		//Pebble.sendAppMessage({"KEY_INVERT": options.selectInvert},	
			function(e) { console.log("Sending settings data..."); },
			function(e) { console.log("Settings feedback failed!"); }
		);
  } else {
		console.log("Cancelled");
  }
});