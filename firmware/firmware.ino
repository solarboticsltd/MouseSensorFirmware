

const int EXPERIMENT_DURATION = 20; //The amount of time taken before the experiement is concluded (Time in minutes)
const int DIP_SEPERATION = 1000;    //The amount of time taken before a subsiquent dip can be detected (Time in milliseconds)
const int DIP_HYSTERESIS = 100;     //The amount of time a sensor must be active/inactive before a dip is cosidered "dipping" or "stopped dipping" (Time in milliseconds)
const int RECORD_ROWS = 16;
const int RECORD_COLUMNS = 50;

unsigned long experimentStart = 0;      //When the Experiment began
unsigned long dipStart = 0;             //When a dip began
unsigned long dipEnd = 0;               //When a dip ended
unsigned long hysteresisStart = 0;      //when a hysteresis check began

int triggeredHole = 0;        //The pin that sensed a dip
int state = 0;                //The current state of the state machine

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

  //Begin serial communication with computer
  Serial.begin(115200);

  //Initialize the sensor pins as inputs
  for(int i = 0; i < sizeof(hole); i++)
  {
    pinMode(hole[i], INPUT);
  }


  //Splash text for the serial connection
  Serial.println("Nicole Larsen - Hole Poke Experiment");
  Serial.println("");
  Serial.println("Beginning Experiment");
  delay(2000);

  experimentStart = millis();
}

void loop() {

  if((millis() - experimentStart) > (EXPERIMENT_DURATION * 60000))
  {
    state = 6;
  }
  
  if(state == 1) //Scanning for an dip on one of the hole sensors
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

  if(state == 6)
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
  }

}
