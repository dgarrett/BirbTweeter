R"(
<script>
setInterval(function() {
	document.getElementById('timetext').value = (new Date()).toLocaleString();
}, 1000);
</script>
<form action="/send" method="post">
Current Time: <input id="timetext" type="text" name="currenttime"><br>
To Send: <input type="text" name="tosend"><br>
<input type="submit" value="Submit">
</form>
)"
