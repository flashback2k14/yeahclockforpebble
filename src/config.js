/**
 * https://ninedof.wordpress.com/2014/05/24/pebble-sdk-2-0-tutorial-9-app-configuration/
 * https://github.com/pebble-hacks/js-configure-demo/blob/master/src%2Fjs%2Fpebble-js-app.js
 * http://www.sitepoint.com/advanced-pebble-watch-configuration/
 * http://developer.getpebble.com/guides/js-apps/pebblekit-js/app-configuration/#tip-use-json-to-transfer-configuration-information
 * http://stackoverflow.com/questions/24230407/creating-a-configuration-page-and-passing-variables-to-a-simply-js-app
 */
var initialized = false;
var options = {};

Pebble.addEventListener("ready", function() {
  console.log("ready called!");
  initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
	console.log("showing configuration");
	Pebble.openURL('http://flashback2k14.github.io/yeahclockforpebble/index.html?'+encodeURIComponent(JSON.stringify(options)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed");
  // webview closed
  //Using primitive JSON validity and non-empty check
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
  } else {
    console.log("Cancelled");
  }
});