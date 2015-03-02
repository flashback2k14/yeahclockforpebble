function saveOptions() {
	var invertSelect = document.getElementById("selectInvert");
	var options = {
		"invertColors": invertSelect.options[invertSelect.selectedIndex].value
	}
	return options;
}

/**function submitCustomization() {
	console.log("Submit");
	var options = saveOptions();
	var location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(options));
	console.log("Warping to: " + location);
	document.location = location;
}

function cancelCustomization(){
	console.log("Cancel");
	document.location = "pebblejs://close";
}*/

var submitButton = document.getElementById("btnSubmit");
submitButton.addEventListener("click", 
	function() {
		console.log("Submit");
		var options = saveOptions();
		var location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(options));
		console.log("Warping to: " + location);
		document.location = location;
	}
,false);

var cancelButton = document.getElementById("btnCancel");
cancelButton.addEventListener("click", 
	function(){
		console.log("Cancel");
		document.location = "pebblejs://close";
	}
,false);