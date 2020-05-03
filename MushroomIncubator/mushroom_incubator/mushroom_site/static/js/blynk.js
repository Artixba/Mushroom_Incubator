const AUTH_TOKEN = 'jWL3x5xRTSLnDnXZyuXxtNiznJ8ikLAm';
const TEMP_PIN = 'D2';
const HUM_PIN = 'D4';
const CO2_PIN = 'D2';
const LIGHT_PIN = 'D4';
const TIME_PIN = 'V1';


// http://blynk-cloud.com/auth_token/get/pin

function queryAndSet(pin, elementId){
  var request = new XMLHttpRequest();
  request.open('GET', `http://blynk-cloud.com/${AUTH_TOKEN}/get/${pin}`);
  request.onreadystatechange = function () {
    if (this.readyState === 4) {
      console.log('Status:', this.status);
      console.log('Headers:', this.getAllResponseHeaders());
      console.log('Body:', this.responseText);
      document.getElementById(elementId).innerHTML = JSON.parse(this.responseText)[0];
    }
  };
  request.send();
}
queryAndSet(TEMP_PIN, "temp");
queryAndSet(HUM_PIN, "hum");  
document.getElementById("temp_hum_button").onclick = function() { 
  queryAndSet(TEMP_PIN, "temp");
  queryAndSet(HUM_PIN, "hum");  
}
queryAndSet(CO2_PIN, "co2");
document.getElementById("co2_button").onclick = function() { 
  queryAndSet(CO2_PIN, "co2");
}
queryAndSet(LIGHT_PIN, "light");
  queryAndSet(TIME_PIN, "time");
document.getElementById("light_button").onclick = function() { 
  queryAndSet(LIGHT_PIN, "light");
  queryAndSet(TIME_PIN, "time");  
}
