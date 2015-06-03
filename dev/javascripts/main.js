function saveOptions() {
    var options = {}
    //Add all textual values
    $('textarea, select, [type="hidden"], [type="password"], [type="text"]').each(function(){options[$(this).attr('id')] = $(this).val();})
    //Add all checkbox type values
    $('[type="radio"], [type="checkbox"]').each(function(){options[$(this).attr('id')] = $(this).is(':checked');})
    return options;
}
$().ready(function() {
    $("#b-cancel").click(function() {
        console.log("Cancel");
        document.location = "pebblejs://close";
    });
    $("#b-submit").click(function() {
        console.log("Submit");
        var location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(saveOptions()));
        console.log("Warping to: " + location);
        console.log(location);
        document.location = location;
    });
    //Set form values to whatever is passed in.
    var obj = jQuery.parseJSON(decodeURIComponent(window.location.search.substring(1)));
    for(key in obj) {
        $("#"+[key]).val(obj[key]);
        $("#"+[key]).val(obj[key]).slider("refresh");
    }
});