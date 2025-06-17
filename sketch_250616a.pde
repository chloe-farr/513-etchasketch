import processing.serial.*;

Serial myPort;
PImage frame;
PGraphics drawingLayer;

int x = 0;
int y = 0;
int x_prev = 0;
int y_prev = 0;

int canvasX = 85;
int canvasY = 140;
int canvasW = 470;
int canvasH = 335;

void setup() {
  size(640, 640); 
  frame = loadImage("frame.png");
  drawingLayer = createGraphics(canvasW, canvasH);
  
  String portName = "/dev/cu.usbmodem1101";     
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  background(255);
  image(frame,0,0);
  image(drawingLayer,canvasX,canvasY);
}

void serialEvent(Serial myPort) {
  String input = myPort.readStringUntil('\n');

  if (input != null) {
    input = trim(input);
    println("from arduino: " + input);

    if (input.equals("clear")) {
      drawingLayer.beginDraw();
      drawingLayer.background(255);
      drawingLayer.endDraw();
      x = y = x_prev = y_prev = 0;
    } else if (input.contains(",")) {
      String[] coords = split(input, ",");
      if (coords.length == 2) {
        try {
          x_prev = x;
          y_prev = y;

          x = constrain(Integer.parseInt(coords[0]) * canvasW/350, 0, canvasW);
          y = constrain(Integer.parseInt(coords[1]) * canvasH/350, 0, canvasH);

          drawingLayer.beginDraw();
          drawingLayer.stroke(0);
          drawingLayer.strokeWeight(2);
          drawingLayer.line(x_prev, y_prev, x, y);
          drawingLayer.endDraw();
          
        } catch (NumberFormatException e) {
          println("Invalid coordinate data: " + input);
        }
      }
    }
  }
}
