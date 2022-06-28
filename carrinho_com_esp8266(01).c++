// esse é um projeto caseiro fiquem a vontade para copiar 
// a parte do joystick em javascript eu ja peguei pronta de resto foram varias adaptações de codigos desculpa se ficou bagunçado 
// qualquer coisa estou a disposição para ensinar

#define _WEBSOCKETS_LOGLEVEL_     2
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer_Generic.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <DNSServer.h>
DNSServer dnsServer;
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);
void setup() {
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 1),IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP("Controle","12341234");
  dnsServer.start(53, "*", IPAddress(192, 168, 1, 1));
  //WiFi.disconnect();
  //WiFiMulti.addAP("HGVA", "JoaoEric1234@@");
  //check_status();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  MDNS.begin("esp8266");
  //webServer.on("/", []() {
    //controle();
  //});
  webServer.onNotFound([]() {
    controle();
  });
  webServer.begin();
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
  OTAinicialization();
}
void loop() {
  //check_status();
  webSocket.loop();
  dnsServer.processNextRequest();
  webServer.handleClient();
  ArduinoOTA.handle();
}
void controle() {
  webServer.send(200, "text/html",F("<!DOCTYPE html>\
  <html>\
  <head>\
  <meta charset='utf-8'>\
    <meta name='viewport' content='width=device-width'>\
    <title>controle</title>\
    <script >\
        var JoyStick = (function(container, parameters){\
          parameters = parameters || {};\
          var title = (typeof parameters.title === \"undefined\" ? \"joystick\" : parameters.title),\
          width = (typeof parameters.width === \"undefined\" ? 0 : parameters.width),\
          height = (typeof parameters.height === \"undefined\" ? 0 : parameters.height),\
          internalFillColor = (typeof parameters.internalFillColor === \"undefined\" ? \"#00AA00\" : parameters.internalFillColor),\
          internalLineWidth = (typeof parameters.internalLineWidth === \"undefined\" ? 2 : parameters.internalLineWidth),\
          internalStrokeColor = (typeof parameters.internalStrokeColor === \"undefined\" ? \"#003300\" : parameters.internalStrokeColor),\
          externalLineWidth = (typeof parameters.externalLineWidth === \"undefined\" ? 2 : parameters.externalLineWidth),\
          externalStrokeColor = (typeof parameters.externalStrokeColor ===  \"undefined\" ? \"#008000\" : parameters.externalStrokeColor),\
          autoReturnToCenter = (typeof parameters.autoReturnToCenter === \"undefined\" ? true : parameters.autoReturnToCenter);\
          var objContainer = document.getElementById(container);\
          var canvas = document.createElement(\"canvas\");\
          canvas.id = title;\
          if(width === 0) { \
            width = objContainer.clientWidth; \
          }\
            if(height === 0) { \
            height = objContainer.clientHeight; \
          }\
          canvas.width = width;\
          canvas.height = height;\
          objContainer.appendChild(canvas);\
          var context=canvas.getContext(\"2d\");\
          var pressed = 0;\
          var circumference = 2 * Math.PI;\
          var internalRadius = (canvas.width-((canvas.width/2)+10))/2;\
          var maxMoveStick = internalRadius + 5;\
          var externalRadius = internalRadius + 30;\
          var centerX = canvas.width / 2;\
          var centerY = canvas.height / 2;\
          var directionHorizontalLimitPos = canvas.width / 10;\
          var directionHorizontalLimitNeg = directionHorizontalLimitPos * -1;\
          var directionVerticalLimitPos = canvas.height / 10;\
          var directionVerticalLimitNeg = directionVerticalLimitPos * -1;\
          var movedX=centerX;\
          var movedY=centerY;\
          if(\"ontouchstart\" in document.documentElement){\
            canvas.addEventListener(\"touchstart\", onTouchStart, false);\
            canvas.addEventListener(\"touchmove\", onTouchMove, false);\
            canvas.addEventListener(\"touchend\", onTouchEnd, false);\
          }else{\
            canvas.addEventListener(\"mousedown\", onMouseDown, false);\
            canvas.addEventListener(\"mousemove\", onMouseMove, false);\
            canvas.addEventListener(\"mouseup\", onMouseUp, false);\
          }\
          drawExternal();\
          drawInternal();\
          function drawExternal(){\
            context.beginPath();\
            context.arc(centerX, centerY, externalRadius, 0, circumference, false);\
            context.lineWidth = externalLineWidth;\
            context.strokeStyle = externalStrokeColor;\
            context.stroke();\
          }\
          function drawInternal(){\
            context.beginPath();\
            if(movedX<internalRadius) { \
              movedX=maxMoveStick; \
            }\
            if((movedX+internalRadius) > canvas.width) { \
              movedX = canvas.width-(maxMoveStick); \
            }\
            if(movedY<internalRadius) { \
              movedY=maxMoveStick; \
            }\
            if((movedY+internalRadius) > canvas.height) {\
              movedY = canvas.height-(maxMoveStick);\
            }\
            context.arc(movedX, movedY, internalRadius, 0, circumference, false);\
            var grd = context.createRadialGradient(centerX, centerY, 5, centerX, centerY, 200);\
            grd.addColorStop(0, internalFillColor);\
            grd.addColorStop(1, internalStrokeColor);\
            context.fillStyle = grd;\
            context.fill();\
            context.lineWidth = internalLineWidth;\
            context.strokeStyle = internalStrokeColor;\
            context.stroke();\
          }\
          function onTouchStart(event) {\
            pressed = 1;\
          }\
          function onTouchMove(event){\
            event.preventDefault();\
            if(pressed === 1 && event.targetTouches[0].target === canvas){\
              movedX = event.targetTouches[0].pageX;\
              movedY = event.targetTouches[0].pageY;\
              if(canvas.offsetParent.tagName.toUpperCase() === \"BODY\"){\
                movedX -= canvas.offsetLeft;\
                movedY -= canvas.offsetTop;\
              }else{\
                movedX -= canvas.offsetParent.offsetLeft;\
                movedY -= canvas.offsetParent.offsetTop;\
              }\
              context.clearRect(0, 0, canvas.width, canvas.height);\
              drawExternal();\
              drawInternal();\
            }\
          }\
          function onTouchEnd(event){\
            pressed = 0;\
            if(autoReturnToCenter){\
              movedX = centerX;\
              movedY = centerY;\
            }\
            context.clearRect(0, 0, canvas.width, canvas.height);\
            drawExternal();\
            drawInternal();\
          }\
          function onMouseDown(event){\
            pressed = 1;\
          }\
          function onMouseMove(event){\
            if(pressed === 1){\
              movedX = event.pageX;\
              movedY = event.pageY;\
              if(canvas.offsetParent.tagName.toUpperCase() === \"BODY\"){\
                movedX -= canvas.offsetLeft;\
                movedY -= canvas.offsetTop;\
              }else{\
                movedX -= canvas.offsetParent.offsetLeft;\
                movedY -= canvas.offsetParent.offsetTop;\
              }\
              context.clearRect(0, 0, canvas.width, canvas.height);\
              drawExternal();\
              drawInternal();\
            }\
          }\
          function onMouseUp(event) {\
            pressed = 0;\
            if(autoReturnToCenter){\
              movedX = centerX;\
              movedY = centerY;\
            }\
            context.clearRect(0, 0, canvas.width, canvas.height);\
            drawExternal();\
            drawInternal();\
          }\
          this.GetWidth = function () {\
            return canvas.width;\
          };\
          this.GetHeight = function () {\
            return canvas.height;\
          };\
          this.GetX = function (){\
            return (100*((movedX - centerX)/maxMoveStick)).toFixed();\
          };\
          this.GetY = function (){\
           return ((100*((movedY - centerY)/maxMoveStick))*-1).toFixed();\
          };\
        });\
        var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);\
        connection.onopen = function () {\
          connection.send('Connect ' + new Date());\
        };\
        connection.onerror = function (error) {\
          console.log('WebSocket Error ', error);\
        };\
        connection.onmessage = function (e) {\
          console.log('Server: ', e.data);\
        };\
        function sendRGB() {\
          var r = parseInt(document.getElementById('r').value );\
          var g = parseInt(document.getElementById('g').value );\
          var b = parseInt(document.getElementById('V').value ).toString(16);\
          console.log(r+128);\
          console.log(g+128);\
          r = (r+128).toString(16);\
          g = (g+128).toString(16);\
          if(r.length < 2){\
            r = '0' + r;\
          }\
          if(g.length < 2){\
            g = '0' + g;\
          }   \
          if(b.length < 2){\
            b = '0' + b;\
          }\
          var rgb = '#'+r+g+b;\
          console.log('RGB: ' + rgb);\
          connection.send(rgb);\
        }\
    </script>\
  </head>\
  <body>\
    <div id=\"joy1Div\" style=\"width:300px;height:300px;margin:50px\"></div>\
    <input id=\"r\" type=\"text\" /><br/>\
    <input id=\"g\" type=\"text\" /><br/>\
    velocidade:<input id=\"V\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>\
    <script type=\"text/javascript\">\
      var Joy1 = new JoyStick('joy1Div');\
      var joy1X = document.getElementById(\"r\");\
      var joy1Y = document.getElementById(\"g\");\
      setInterval(function(){ joy1X.value=Joy1.GetX();}, 50);\
      setInterval(function(){ joy1Y.value=Joy1.GetY();}, 50);\
      setInterval(function(){ sendRGB();},200);\
    </script>\
  </body>\
</html>"));
}
//void check_status() {
//  while (WiFiMulti.run() != WL_CONNECTED) {
//  WiFi.disconnect();
//  WiFiMulti.addAP("HGVA", "JoaoEric1234@@");
// while (WiFiMulti.run() != WL_CONNECTED) {
//  delay(500);
//   }
// }
//}
void webSocketEvent(const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length) {
  (void) length;
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      {
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      if (payload[0] == '#') {
        int x = 0;
        int y = 0;
        int r = 0;
        int l = 0;
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        x = (((rgb >> 16) & 0xFF) - 128) * 2;
        y = (((rgb >> 8) & 0xFF) - 128) * 2;
        int b = ((rgb >> 0) & 0xFF);
        if (x >= 0) {
          r = x;
        } else {
          l = (-1 * x);
        }
        if (y > 0) {
          analogWrite(1, (y - r + 50));
          analogWrite(3, 0);
          analogWrite(2, 0);
          analogWrite(0, (y - l + 50));
        } else if (y < 0) {
          analogWrite(1, 0);
          analogWrite(3, ((-y) - r));
          analogWrite(2, ((-y) - l));
          analogWrite(0, 0);
        } else {
          analogWrite(1, 0);
          analogWrite(3, 0);
          analogWrite(2, 0);
          analogWrite(0, 0);
        }
      }
      break;
    default:
      break;
  }
}
void OTAinicialization() {
  ArduinoOTA.setHostname("ESP_Carrinho");
  ArduinoOTA.onStart([]() {
  });
  ArduinoOTA.onEnd([]() {
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  });
  ArduinoOTA.onError([](ota_error_t error) {
  });
  ArduinoOTA.begin();
}
