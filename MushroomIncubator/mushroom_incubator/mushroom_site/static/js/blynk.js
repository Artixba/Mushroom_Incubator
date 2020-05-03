const auth_token = 'jWL3x5xRTSLnDnXZyuXxtNiznJ8ikLAm';
const pin = 'D2';

// http://blynk-cloud.com/auth_token/get/pin

function Bbuttonhandler(){
  var request = new XMLHttpRequest();
  request.open('GET', 'http://blynk-cloud.com/jWL3x5xRTSLnDnXZyuXxtNiznJ8ikLAm/get/D2');

  request.onreadystatechange = function () {
    if (this.readyState === 4) {
      console.log('Status:', this.status);
      console.log('Headers:', this.getAllResponseHeaders());
      console.log('Body:', this.responseText);
      document.getElementById("Blynk1").innerHTML = this.responseText;
    }
  };
  request.send();
}
document.getElementById("Bbutton").onclick = Bbuttonhandler;
 
function Abuttonhandler(){
  var request = new XMLHttpRequest();
  request.open('GET', 'http://blynk-cloud.com/jWL3x5xRTSLnDnXZyuXxtNiznJ8ikLAm/get/D4');

  request.onreadystatechange = function () {
    if (this.readyState === 4) {
      console.log('Status:', this.status);
      console.log('Headers:', this.getAllResponseHeaders());
      console.log('Body:', this.responseText);
      document.getElementById("Blynk2").innerHTML = this.responseText;
    }
  };
  request.send();
}
document.getElementById("Abutton").onclick = Abuttonhandler;

document.getElementById("Cbutton").onclick = function() { 
  Abuttonhandler();
  Bbuttonhandler();  
}
