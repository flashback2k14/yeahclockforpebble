//######################################
//             Variables
//######################################
var initialized = false;
var options = {};
var weatherInfo = {};

//######################################
//              Weather
//######################################
//function for request to OpenWeatherMap
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

//callback functions for weather
function locationSuccess(pos) {
  //construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude;
	console.log('DEBUG: Url: ' + url);
  //send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      //responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      //temperature in Kelvin requires adjustment
      weatherInfo.temperature = Math.round(json.main.temp - 273.15);
      //conditions
      weatherInfo.conditions = json.weather[0].main;      
			console.log('DEBUG: Temperature is ' + weatherInfo.temperature);
      console.log('DEBUG: Conditions are ' + weatherInfo.conditions);
    }      
  );
	//send to Pebble
	Pebble.sendAppMessage(
		{"KEY_TEMPERATURE": weatherInfo.temperature, "KEY_CONDITIONS": weatherInfo.conditions},
		function(e) {
			console.log('Weather info sent to Pebble successfully!');
		},
		function(e) {
			console.log('Error sending weather info to Pebble!');
		}
	);
}

function locationError(err) {
  console.log('Error requesting location!');
}

//get weather
function getWeather() {
	console.log('DEBUG: getWeather entered');
	navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

//######################################
//          Pebble EventListener
//######################################
//listen for when the watchface is opened
Pebble.addEventListener("ready", function() {
  console.log("ready called!");
  initialized = true;
	getWeather();
});

//listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getWeather();
	}
);

//listen for open configuration page
Pebble.addEventListener("showConfiguration", function() {
	console.log("showing configuration");
	Pebble.openURL('http://flashback2k14.github.io/yeahclockforpebble/index.html?' + encodeURIComponent(JSON.stringify(options)));
});

//listen for close configuration page
Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed");
  //JSON validity and non-empty check
  if (e.response.charAt(0) === "{" && e.response.slice(-1) === "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
		console.log("DEBUG: Options: " + JSON.stringify(options));
		//Send to Pebble, persist there
		Pebble.sendAppMessage(
			{"KEY_INVERT": options.selectInvert, "KEY_SHOW_DATE_WEATHER": options.selectShowDate},
			function (e) {
				console.log("Sending settings data...");
			}, 
			function(e) {
				console.log("Settings feedback failed!");
			}
		);
  } else {
		console.log("webviewclosed: Cancelled");
  }
});
