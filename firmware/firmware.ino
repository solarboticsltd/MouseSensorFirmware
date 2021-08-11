
//Variables you can change
//*********************************
const int EXPERIMENT_DURATION = 2;  //The amount of time taken before the experiement is concluded (Time in minutes)
const int DIP_SEPERATION = 1000;    //The amount of time taken before a subsiquent dip can be detected (Time in milliseconds)
const int DIP_HYSTERESIS = 500;     //The amount of time a sensor must be active/inactive before a dip is cosidered "dipping" or "stopped dipping" (Time in milliseconds)
//*********************************





const int RECORD_ROWS = 16;         //The number of rows of the data table
const int RECORD_COLUMNS = 50;      //The number of columns of the data table (to add more record slots, you will need to change this number and also add the appropriate amount of zeros to "dipRecords")

unsigned long experimentStart = 0;  //When the Experiment began
unsigned long dipStart = 0;         //When a dip began
unsigned long dipEnd = 0;           //When a dip ended
unsigned long hysteresisStart = 0;  //when a hysteresis check began

int triggeredHole = 0;              //The pin that sensed a dip
int state = 0;                      //The current state of the state machine

//An array containing the pins used for the hole sensors
int hole[16] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 24, 26, 28, 30};

//An array to track dip duration of each individual dip
int dipRecords[RECORD_ROWS][RECORD_COLUMNS] = {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}; 





//Run once to setup the arduino
void setup() {

  //Begin serial communication with computer
  Serial.begin(115200);

  //Initialize the sensor pins as inputs
  for(int i = 0; i < sizeof(hole); i++)
  {
    pinMode(hole[i], INPUT);
  }

  //Set the pin assigned to the onboard LED as an output
  pinMode(13, OUTPUT);
  
  //Splash text for the serial connection
  Serial.println("Nicole Larsen - Hole Poke Experiment");
  Serial.println("");
  Serial.println("Beginning Experiment");
  Serial.println("");
  delay(2000);

  experimentStart = millis();
  state = 1;
}





void loop() {

  if((millis() - experimentStart) > (EXPERIMENT_DURATION * 60000))
  {
    state = 6;
  }


  
  if(state == 1) //Scanning for a dip on one of the hole sensors
  { 
    for(int i = 0; i < sizeof(hole); i++)
    {
      if(digitalRead(hole[i]) == HIGH)
      {
        triggeredHole = i;
        hysteresisStart = millis();
        state = 2;
        Serial.println("Dip detected on pin: " + String(hole[triggeredHole]));
        break;
      }
    }
  }


  
  if(state == 2) //Hysteresis timer on positive edge of dip
  {
    if((digitalRead(hole[triggeredHole]) == HIGH) && (millis() - hysteresisStart > DIP_HYSTERESIS))
    {
      dipStart = millis();
      state = 3;
    }
    if(digitalRead(hole[triggeredHole]) == LOW)
    {
      state = 1;
    }
  }
  
  if(state == 3) //"Counting" the dip time aka doing nothing until a negative edge of dip is detected.
  {
    if(digitalRead(hole[triggeredHole]) == LOW)
    {
      state = 4;
      hysteresisStart = millis();
    }
  }



  if(state == 4) //Hysteresis timer on a negative edge of dip
  {
    if((digitalRead(hole[triggeredHole]) == LOW) && (millis() - hysteresisStart > DIP_HYSTERESIS))
    {
      dipEnd = millis();
      state = 5;
    }
    if(digitalRead(hole[triggeredHole]) == HIGH)
    {
      state = 3;
    }
  }



  if(state == 5) //Save dip duration in dipRecords and start checking holes for dips again
  {
    unsigned long dipDuration = dipEnd - dipStart;
    int recordSlot = dipRecords[triggeredHole][0];
    
    dipRecords[triggeredHole][recordSlot] = dipDuration;
    dipRecords[triggeredHole][0] += 1;
    
    Serial.println("Dip on pin: " + String(triggeredHole) + ", was " + String(dipDuration) + "mS long and saved in slot " + String(triggeredHole) + ", " + String(recordSlot));

    delay(DIP_SEPERATION);
    state = 1;
  }



  if(state == 6) //Display the results of the experiment in the serial window.
  {
    Serial.println("The experiment has been completed!");
    Serial.println("");

    for(int i = 0; i < RECORD_ROWS; i++)
    {
      Serial.print("Sensor " + String(hole[triggeredHole]) + ", ");
      
      for(int j = 0; j < RECORD_COLUMNS; j++)
      {
        Serial.print(String(dipRecords[i][j]));
        if(j < RECORD_COLUMNS - 1)
        {
           Serial.print(", ");
        }
      }
      Serial.println("");
    }
    state = 7;
  }



  if(state == 7) //Idle until reset (blink LED)
  {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
  }

}
