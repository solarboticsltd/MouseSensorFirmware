
//Variables you can change
//*********************************
const int EXPERIMENT_DURATION = 1;  //The amount of time taken before the experiement is concluded (Time in minutes)
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


//An array containing the pins used for the hole sensors
int hole[16] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 24, 26, 28, 30};


//Run once to setup the arduino
void setup() {

  for(int i = 0; i < RECORD_ROWS; i++)
  { 
    for(int j = 0; j < RECORD_COLUMNS; j++)
    {
      if(j == 0)
      {
        dipRecords[i][j] = 1;
      }
      else
      {
        dipRecords[i][j] = 0;
      }
    }
  }
    
  //Begin serial communication with computer
  Serial.begin(115200);
  
  //Initialize the sensor pins as inputs
  for(int i = 0; i < RECORD_ROWS; i++)
  {
    pinMode(hole[i], INPUT_PULLUP);
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
  //Serial.println("State: " + String(state));
}





void loop() {

  if((millis() - experimentStart) > (EXPERIMENT_DURATION * 60000) && state != 7)
  {
    state = 6;
    //Serial.println("State: " + String(state));
  }


  
  if(state == 1) //Scanning for a dip on one of the hole sensors
  { 
    for(int i = 0; i < RECORD_ROWS; i++)
    {
      if(digitalRead(hole[i]) == LOW)
      {
        triggeredHole = i;
        hysteresisStart = millis();
        state = 2;
        Serial.println("Dip detected on pin " + String(hole[triggeredHole]) + ", sensor " + String(triggeredHole));
        Serial.println("State: " + String(state));
        break;
      }
    }
  }


  
  if(state == 2) //Hysteresis timer on positive edge of dip
  {
    if((digitalRead(hole[triggeredHole]) == LOW) && (millis() - hysteresisStart > DIP_HYSTERESIS))
    {
      dipStart = millis() + DIP_HYSTERESIS;
      state = 3;
      Serial.println("State: " + String(state));
    }
    if(digitalRead(hole[triggeredHole]) == HIGH)
    {
      state = 1;
      Serial.println("State: " + String(state));
    }
  }
  
  if(state == 3) //"Counting" the dip time aka doing nothing until a negative edge of dip is detected.
  {
    if(digitalRead(hole[triggeredHole]) == HIGH)
    {
      state = 4;
      Serial.println("State: " + String(state));
      hysteresisStart = millis();
    }
  }



  if(state == 4) //Hysteresis timer on a negative edge of dip
  {
    if((digitalRead(hole[triggeredHole]) == HIGH) && (millis() - hysteresisStart > DIP_HYSTERESIS))
    {
      dipEnd = millis() - DIP_HYSTERESIS;
      state = 5;
      Serial.println("State: " + String(state));
    }
    if(digitalRead(hole[triggeredHole]) == LOW)
    {
      state = 3;
      Serial.println("State: " + String(state));
    }
  }



  if(state == 5) //Save dip duration in dipRecords and start checking holes for dips again
  {
    unsigned long dipDuration = dipEnd - dipStart;

    if(dipDuration < (EXPERIMENT_DURATION * 60000))
    {
      int recordSlot = dipRecords[triggeredHole][0];
      
      dipRecords[triggeredHole][recordSlot] = dipDuration;
      dipRecords[triggeredHole][0] += 1;
      
      Serial.println("Dip on pin: " + String(hole[triggeredHole]) + ", was " + String(dipDuration) + "mS long and saved in slot " + String(triggeredHole) + ", " + String(recordSlot));
  
      delay(DIP_SEPERATION);
      Serial.println("State: " + String(state));
    }
    else
    {
      Serial.println("Error: measurement of " + String(dipDuration) + "mS rejected");
    }

    state = 1;
    Serial.println("State: " + String(state));
  }



  if(state == 6) //Display the results of the experiment in the serial window.
  {
    delay(3000);
    Serial.println("");
    Serial.println("The experiment has been completed!");
    Serial.println("");
    Serial.println("Results:");
    Serial.println("");

    for(int i = 0; i < RECORD_ROWS; i++)
    {
      Serial.print("Sensor " + String(hole[i]) + ", ");
      
      for(int j = 1; j < RECORD_COLUMNS; j++)
      {
        Serial.print(String(dipRecords[i][j]));
        Serial.print(", ");
      }
      Serial.println("");
    }
    state = 7;
    Serial.println("State: " + String(state));
  }



  if(state == 7) //Idle until reset (blink LED)
  {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
  }

}
