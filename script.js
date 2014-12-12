$(document).ready(function(){
  var ID = new Date().getTime();
  (function poll() {
    var dataS = "$ID=" + ID;
    setTimeout(function () {
      $.ajax({
        type: 'GET',
        url: 'chat',
	data: dataS,
        success: function (result) {
          $("#chatbox").html(result);
          IDchange();
        },
        complete: poll
      });
    }, 5000);
  })();
  function IDchange(){
	$('li').each(function(){
		var currid = $(this).find("#user").text();
		if(currid==ID){
			$(this).find("#user").text("Me");
		}
		else {
			$(this).find("#user").text("Stranger");
		}
	});
  };
  $("#submit").click(function(){
      var text = $("#text").val();
      $("#text").val('');
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
            $("#chatbox").html(result);
            IDchange();
          }
        });
      }
    return false;
  });
});

