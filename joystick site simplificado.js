<!DOCTYPE html>\
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
    </script>\
  </head>\
  <body>\
    <div id=\"joy1Div\" style=\"width:300px;height:300px;margin:50px\"></div>\
    <input id=\"r\" type=\"text\" /><br/>\
    <input id=\"g\" type=\"text\" /><br/>\
    <script type=\"text/javascript\">\
      var Joy1 = new JoyStick('joy1Div');\
      var joy1X = document.getElementById(\"r\");\
      var joy1Y = document.getElementById(\"g\");\
      setInterval(function(){ joy1X.value=Joy1.GetX();}, 50);\
      setInterval(function(){ joy1Y.value=Joy1.GetY();}, 50);\
    </script>\
  </body>\
</html>
