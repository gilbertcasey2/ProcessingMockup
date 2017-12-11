///
// Casey E Gilbert
///

int hoursTracked = 720;
int[] data = new int[hoursTracked];
Rope[] ropes;
int ropeNum;
Timeframe timeWindow;
int[] currWindow; // the current window being shown
int top; // height of top of graph;
int bottom; // height of bottom of graph
int startLine;
int endLine;
int start;
int end;

void setup() {
  size(500,500);
  // CURRENTLY ARBITRARY VALUES
  top = 120;
  bottom = 36;
  startLine = 55;
  endLine = 445;
  
  
  // ARBITRARY generate random data 
  for (int i = 0; i < hoursTracked; i++) {
    int num = i + (int)random(0,300);
    data[i] = (num*5)/720;
    println(data[i]);
  }
  // AGAIN COULD CHANGE THIS
  ropeNum = 65;
  // make rope array and generate ropes
  ropes = new Rope[ropeNum];
  for (int i = 0; i < ropeNum; i++) {
   ropes[i] = new Rope(10 + i*(500/ropeNum)); 
  }
  // build the time window
  timeWindow = new Timeframe(ropeNum, hoursTracked, data);
  /*newWindow(1, 720);
  printRopes();
  newWindow(50,150);
  print("\n---------- AFTER CHANGE -----------\n");
  printRopes();*/
  
}

void draw() {
  background(255,255,255);
  if (startLine > endLine - 1) {
    startLine = 55;
    endLine = 445;
  }
  if (startLine < 55) {
    startLine = 55;
  }
  if (endLine > 445) {
    endLine = 445;
  }
  calcWind();
  drawControl();
  for (int i = 0; i < ropeNum; i++) {
    ropes[i].drawRope();
  }
  newWindow();
  
}

///
// printRopes: a function to loop through ropes and print them
///
void printRopes() {
  for (int i = 0; i < ropeNum; i ++) {
    println("Rope " + i + ": value: " + ropes[i].value + 
      ", length: " + ropes[i].lengthy);
  }
}

///
// newWindow: a function to generate the new window and store it,
//            and then apply those values to the ropes
// @peram: the start and end hours of the window
// @return: none
///
void newWindow() {
  // get the rope data for the new window
  currWindow = new int[ropes.length];
  currWindow = timeWindow.changeWindow(start,end);
  // put that data into the ropes
  for (int i = 0; i < ropeNum; i++) {
    ropes[i].calcLength(currWindow[i], top, bottom);
  }
}

void drawControl() {
  stroke(0,0,0);
  strokeWeight(2);
  line(50,300,50,400);
  line(450,300,450,400);
  line(50,350,450,350);
  strokeWeight(3);
  stroke(100,100,100);
  line(startLine, 310, startLine,  390);
  line(endLine, 310, endLine, 390);
  noStroke();
  fill(0,0,0,50);
  rect(startLine, 320, endLine-startLine, 60);
}

void mouseDragged() {
  if (mouseX >= startLine - 40 && mouseX < startLine + 40 && mouseY > 300 && mouseY < 400) {
    startLine = mouseX;
  }
  if (mouseX >= endLine - 40 && mouseX < endLine + 40 && mouseY > 300 && mouseY < 400) {
    endLine = mouseX;
  }
  
}

void calcWind() {
  start = ((startLine-55)*720)/400;
  end = ((endLine-45)*720)/400;
}