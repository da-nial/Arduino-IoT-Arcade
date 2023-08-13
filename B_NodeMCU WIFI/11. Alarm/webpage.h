const char webpage[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
<style>
.button {
  border: none;
  color: white;
  padding: 16px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  transition-duration: 0.4s;
  cursor: pointer;
}

.button1 {
  background-color: #ccffe6; 
  color: black; 
  border: 2px solid #4CAF50;
  border-radius: 8px;
}

.button1:hover {
  background-color: #4CAF50;
  color: white;
  box-shadow: 0 10px 14px 0 rgba(0,0,0,0.24), 0 14px 40px 0 rgba(0,0,0,0.19);
}

.button1:active {
  background-color: #33ff99;
  box-shadow: 0 5px #666;
  transform: translateY(4px);
}

.button2 {
  background-color: #ff8566; 
  color: black; 
  border: 2px solid red;
  border-radius: 8px;
}

.button2:hover {
  background-color: red;
  color: white;
  box-shadow: 0 10px 14px 0 rgba(0,0,0,0.24), 0 14px 40px 0 rgba(0,0,0,0.19);
}

.button2:active {
  background-color: #ff6666;
  box-shadow: 0 5px #666;
  transform: translateY(4px);
}


</style>
</head>
<body>

<div class="container">
<h2>ESP Web server | IoT Course</h2>
<h1>Alarm</h1>

<form action="/set-alarm">
  <label for="time">Time</label><br>
  <input type="text" id="time_input" name="time_input" value=""><br><br>

  <div class="slidecontainer">
    <input type="range" min="300" max="1500" value="900" class="slider" id="myRange" name="freq"><br/><br/>
    <p>Value: <span id="demo"></span></p>
  </div>

  <input type="submit" value="Submit"><br/><br/>
</form>


<button class="button button1" onclick="location.href='/stop'" type="button">Stop</button>
<button class="button button2" onclick="location.href='/snooze'" type="button">Snooze</button>

<script>
var slider = document.getElementById("myRange");
var output = document.getElementById("demo");
output.innerHTML = slider.value;

slider.oninput = function() {
  output.innerHTML = this.value;
}
</script>

</body>
</div>
</html>
)rawliteral";
