$(document).ready(function(){
  var ID = new Date().getTime();
  $("#chatbox").html("Hello World");
  $("#submit").click(function(){
    var text = $("#text").val();
    var cont = $("#cont").val();
    var currentdate = new Date();
    var timestr = currentdate.getHours() + ":" + currentdate.getMinutes() + ":" + currentdate.getSeconds();
    // Returns successful data submission message when the entered information is stored in database.
    var dataString = '$ID='+ ID +'&text='+ text + '&time=' + timestr;
    if(text=='')
    {
      alert("Please Fill All Fields");
    }
    else
    {
      // AJAX Code To Submit Form.
      $.ajax({
        type: 'POST',
        url: 'chat',
        data: dataString,
        cache: false,
        success: function(result){
          alert(result);
	  $("#chatbox").html(result);
        }
      });
    }
    return false;
  });
  (function poll() {
    setTimeout(function () {
        $.ajax({
            type: 'GET',
            url: 'chat',
            success: function (result) {
                alert(result); //DO ANY PROCESS HERE  function format(res){
    		$("#chatbox").html(result);
  //s};
            },
            complete: poll
        });
    }, 5000);
})();
});

