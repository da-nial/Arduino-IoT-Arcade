const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<style>

</style>
</head>
<body>

<form action="/order">
 <input type="radio" id="milk" name="product" value="milk">
 <label for="milk">Milk | 7000</label><br/>

 <input type="radio" id="soda" name="product" value="soda">
 <label for="soda">Soda | 5000</label><br/>

 <input type="radio" id="peanut_pack" name="product" value="peanut_pack">
 <label for="peanut_pack">Peanut Pack | 4000</label><br/>

 <input type="radio" id="chips" name="product" value="chips">
 <label for="chips">Chips | 6000</label><br/>

  <input type="submit" value="order"><br/><br/>
</form> 

<script>
  var gateway = `ws://${window.location.hostname}/ws`;

  var websocket;
  window.addEventListener('load', onload);

  function onload(event) {
    initWebSocket();
  }

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
  }

  function onOpen(event) {
    console.log('Connection opened');
  }

  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }

  function onMessage(event) {
    // Get Event
    console.log('eventtt: ', event)
    tempAlert(event.data, 5000);
  }

  function tempAlert(msg, duration) {
    var el = document.createElement("div");
    el.setAttribute("style","position:absolute;top:40%;left:20%;background-color:white;color:red;");
    el.innerHTML = msg;
    setTimeout(function(){
      el.parentNode.removeChild(el);
    }, duration);
    document.body.appendChild(el);
  }

</script>

</body>
</div>
</html>
)rawliteral";
