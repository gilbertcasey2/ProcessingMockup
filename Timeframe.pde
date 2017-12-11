///
// class Timeframe
// This class holds the total timeframe of data collected
// It allows you to select windows of time from within this data

class Timeframe {
   int totalHour;  // the total number of hours currently in the window
   int windowHour;
   int start;      // the start of the current window
   int end;        // the end of the current window
   int ropeNum;    // the number of ropes in the graph
   int segLength;  // the number of hours that each rope represents
   int[] data;     // the data set ( an avg happiness/ hour ) 
  
  ///
  // Constructor: takes in rope number, total hours for data, 
  //              and the array of data
  //              Also calculates the first segLength by dividing 
  //              total hours by rope number
  ///
  public Timeframe(int p_rope, int p_hour, int[] p_data) {
    totalHour = p_hour;
    ropeNum = p_rope;
    segLength = totalHour/ropeNum;
    data = new int[totalHour];
    this.data = p_data;
  }
  
  ///
  // changeWindow
  // Purpose: the purpose of this function is to 
  //          change the window in time that is displayed
  // @peram: p_start: the starting hour in the total time
  // @param: p_end: the ending hour in the total time ( will 
  //                include end hour in display )
  // @return: an array of data the size of the number of ropes
  //           each element in the array is the avg happiness
  //           data that tells the length of one rope
  ///
  public int[] changeWindow(int p_start, int p_end) {
    start = p_start;
    end = p_end;
    windowHour = end - start;
    // create the array to hold the data sending out
    int[] window = new int[ropeNum];
    // get the section of data that we want
    int[] newData = subset(data, start, windowHour);
    for (int i = 0; i < newData.length; i++) {
    }
    segLength = windowHour/ropeNum; // calculate new segLength for window
    int count = 0;
    int temp = 0;
    // Loop through the new segment of data by segLength
    
    for (int i = 0; i < ropeNum; i ++) {
      // loop through each hour in the subset 
      for (int j = 0; j < segLength; j++) {
        // add that hour to the last
        temp = temp + newData[i*segLength +j]; 
      }
      // divide the total added hours by segLength to get avg happiness
      // for that segment. Then put it in the window array
      window[count] = temp/segLength;
      temp = 0;
      // increase our location in the window array
      count = count + 1;
    }
    // return the window array with data for each rope
    return window;
  }
}