$(document).ready( function () {

  function saveOptions() {
    var invertSelect = document.getElementById("selectInvert");
    var options = {
      "invertColors": invertSelect.options[invertSelect.selectedIndex].value
    }
    return options;
  }

  $('#btnCancel').on('click', function () {
    console.log("Cancel");
    document.location = "pebblejs://close";
  });

  $('#btnSubmit').on('click', function () {
    console.log("Submit");
    var location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(saveOptions()));
    console.log(location);
    document.location = location;
  });
});