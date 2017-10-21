#include <SD.h> //SD lib
#include <dht.h> //DHT sensor lib
#define DHT11_PIN 7 //pin for sensor
#define SECONDS_BETWEEN_LOGS 60
#define NUM_READS 100 //number of readings (size of arrays) reduce to increase memory available

//sensors
dht DHT;
//files
File myFile;

//temperature tracking variables
int avgTmp;
int readingsT[NUM_READS];
int totalT = 0;
   
//humidity tracking variables
int avgHum;
int readingsH[NUM_READS];   
int totalH = 0;                  
//
//add new sensors/variables whatever here
//
//indexes and counters
int counter=0;
int readIndex = 0; 
int readingCount=0;

void setup(){
  //the SD card is on pin 4
  SD.begin(4);
  myFile = SD.open("THLOG.txt", FILE_WRITE);
  }
  
void loop(){
  counter++;
  if (counter==SECONDS_BETWEEN_LOGS) {
    print();
    counter=0;     
  }
  measure();
  delay(1000);
}

//____________________________________________________________________
// TITLE
//        print
// DESCRIPTION
//        prints the data/delim/newline using printData()
// SCOPE
//        PRIVATE
// INPUTS
//        none
// ASSUMES
//        avgTemp,AvgHum,readingcount
//        myFile
// RETURNS  
//        N/A Data printed to file
//_____________________________________________________________________
void print() {
  printData(avgTmp, false, false);
  printData(avgHum, false, true);
  printData(readingCount, true, true);
  
  //flush to file so we can pull out the SD card whenever
  myFile.flush();
}
//____________________________________________________________________
// TITLE
//        updateArray
// DESCRIPTION
//        updates the arrays and keeps the totals 
// SCOPE
//        PRIVATE
// INPUTS
//        measurement   : reading from the sensor to place in arrays
//        totalReadings : current running total
//        readdingArray : array corresponding to the measurement
// ASSUMES
//        nothing
// RETURNS  
//        updated totalReadings (int)
//_____________________________________________________________________
int updateArray(int measurement,int totalReadings,int readingArray[]) {
  //remove old measurement, add new to history, and cacluate new total
  totalReadings-=readingArray[readIndex];
  readingArray[readIndex] = measurement;
  totalReadings+=readingArray[readIndex];
  return totalReadings;
} 

//____________________________________________________________________
// TITLE
//        measure
// DESCRIPTION
//        pulls the measurements off the sensor and updates averages
//        and indexes 
// SCOPE
//        PRIVATE
// INPUTS
//        none
// ASSUMES
//        avgTemp,avgHum,numReadings
//        readingsT,readingsH
//        DHT
// RETURNS  
//        none
//_____________________________________________________________________
void measure() {
  //update sensor
  DHT.read11(DHT11_PIN);
  //measure temp and calc the average
  totalT=updateArray(DHT.temperature, totalT,readingsT);
  avgTmp=totalT/NUM_READS;
  //measure humidity and calc the average
  totalH=updateArray(DHT.humidity,totalH,readingsH);
  avgHum=totalH/NUM_READS;
  //
  //Can add more measurements here
  //

  //index updates
  readIndex = (readIndex + 1)%NUM_READS;
  readingCount+=1;
}

//____________________________________________________________________
// TITLE
//        printData(int)
// DESCRIPTION
//        writes input to new file and/or delimters and/or asterisks
// SCOPE
//        PRIVATE
// INPUTS
//        data     : the int to print
//        newline  : boolean to put a newline or not
//        asterisk : boolean to delimit with asterisk
// ASSUMES
//        myFile
// RETURNS  
//        N/A data printed to file
//_____________________________________________________________________
void printData(int data, boolean newline, boolean asterisk) {
  if (asterisk) {
    myFile.print("*");
  }
  myFile.print(data);
  if (newline) {
    myFile.println("");
  }
}
//____________________________________________________________________
// TITLE
//        printData(string)
// DESCRIPTION
//        writes input to new file and/or delimters and/or asterisks
// SCOPE
//        PRIVATE
// INPUTS
//        data     : the string to print
//        newline  : boolean to put a newline or not
//        asterisk : boolean to delimit with asterisk
// ASSUMES
//        myFile
// RETURNS  
//        N/A data printed to file
//_____________________________________________________________________
void printData(String data, boolean newline, boolean asterisk) {
  if (asterisk) {
    myFile.print("*");
  }
  myFile.print(data);
  if (newline) {
    myFile.println("");
  }
}

