///
// class Rope
// this class represents one single rope holding a ball
// it stores its own length
///

class Rope {
 
 int lengthy; // the length of the rope
 int value;   // the happiness of the rope
 int x;
 int y1;
 int y2;
 color col;
 
 // just an arbitrary value to start (rope starts up at ceiling)
  public Rope(int p_x) {
   lengthy = 0;
    x = p_x;
    y1 = 10;
    y2 = 10;
    col = color(50,0,100);
  } 
  
  ///
  // @peram: value  a 1 - 5 number that represents the average
  //         level of happiness to calculate rope length
  // @peram: topH is the height of the top of the graph
  // @peram: bottomH is the height of the bottom of the graph
  // Purpose: This function converts the happiness value into
  //          a the length that the rope should be
  // @return: nothing
  ///
  public void calcLength(int p_value, int topH, int bottomH) {
    value = p_value;
    int distance = topH - bottomH;
    int increment = distance/5;
    lengthy = topH - value * increment;
  }
  
  public void drawRope() {
    noFill();
    stroke(col);
    strokeWeight(2);
    line(x, y1, x, y2);
    y2 = y1 + lengthy;
  }
  
  
}