const AUTH_TOKEN = 'jWL3x5xRTSLnDnXZyuXxtNiznJ8ikLAm';
const TEMP_PIN = 'V1';
const TEMP_TARGET ='V2';
const HUM_PIN = 'V3';
const HUM_TARGET = 'V4'
const CO2_PIN = 'V8';
const CO2_TARGET = 'V6';
const LIGHT_PIN = 'V7';
const TIME_PIN = 'V12';

// http://blynk-cloud.com/auth_token/get/pin

function queryTarget(pin, elementId){
  const request = new XMLHttpRequest();
  request.open('GET', `http://blynk-cloud.com/${AUTH_TOKEN}/get/${pin}`);
  request.onreadystatechange = function () {
    if (this.readyState === 4) {
      document.getElementById(elementId).innerHTML = JSON.parse(this.responseText)[0];
    }
  };
  request.send();
}

function queryBlynk(pin, callback){
  const request = new XMLHttpRequest();
  request.open('GET', `http://blynk-cloud.com/${AUTH_TOKEN}/get/${pin}`);
  request.onreadystatechange = function () {
    if (this.readyState === 4) {
      const data = JSON.parse(this.responseText)[0];
      callback(data);
    }
  };
  request.send();
}

function queryAndSet(pin, elementId, targetVal){
  queryBlynk(pin, (data) =>{
    document.getElementById(elementId).innerHTML = data;
    document.getElementById(elementId).style.color = 'black';
    if(targetVal){
      targetMeasurement(data, elementId, targetVal);    
    }
  });
}

function targetMeasurement(actualVal, actualValID, targetVal){
  if (actualVal === targetVal){
    // green
    document.getElementById(actualValID).style.backgroundColor = 'rgba(48, 187, 43, 0.85)'; 
  }
  else if (Math.abs(actualVal - targetVal) <= 5){ 
    // yellow
    document.getElementById(actualValID).style.backgroundColor = 'rgba(206, 211, 69, 0.85)';
  } else {
    // red
    document.getElementById(actualValID).style.backgroundColor = 'rgba(228, 60, 60, 0.85)';
  }
};

queryBlynk(TEMP_TARGET, (data) => queryAndSet(TEMP_PIN, "temp", data));
queryTarget(TEMP_TARGET,"temptarget");
queryBlynk(HUM_TARGET, (data) => queryAndSet(HUM_PIN, "hum", data));  
queryTarget(HUM_TARGET,"humtarget");
document.getElementById("temp_hum_button").onclick = function() { 
  queryBlynk(TEMP_TARGET, (data) => queryAndSet(TEMP_PIN, "temp", data));
  queryTarget(TEMP_TARGET,"temptarget");
  queryBlynk(HUM_TARGET, (data) => queryAndSet(HUM_PIN, "hum", data));  
  queryTarget(HUM_TARGET,"humtarget");
}

queryBlynk(CO2_TARGET, (data) => queryAndSet(CO2_PIN, "co2", data));  
queryTarget(CO2_TARGET,"co2target");
document.getElementById("co2_button").onclick = function() { 
  queryBlynk(CO2_TARGET, (data) => queryAndSet(CO2_PIN, "co2", data));  
  queryTarget(CO2_TARGET,"co2target");
}

queryAndSet(LIGHT_PIN, "light", LIGHT_PIN);
queryAndSet(TIME_PIN, "time", TIME_PIN);
document.getElementById("light_button").onclick = function() { 
  queryAndSet(LIGHT_PIN, "light", LIGHT_PIN); 
  queryAndSet(TIME_PIN, "time", TIME_PIN);  
}

