//**************************************************************************************************************************************************
// This procedure reads the serial port - SerialBT - for bluetooth commands being sent from the Android device

void bluetooth()
{
 while (SerialBT.available())                                    // Read bluetooth commands over SerialBT // Warning: If an error with SerialBT occurs, make sure Arduino Mega 2560 is Selected
 {  
  {  
      str = SerialBT.readStringUntil('\n');                      // str is the temporary variable for storing the last sring sent over bluetooth from Android device
      //Serial.print(str);                                      // for testing purposes
  } 
    
    blueToothVal = (str.toInt());                               //  convert the string 'str' into an integer and assign it to blueToothVal
    Serial.print("BlueTooth Value ");
    Serial.println(blueToothVal);    

// **************************************************************************************************************************************************

 switch (blueToothVal) 
 {
      case 1:                                
        SerialBT.println("Forward");
        Forward();
        break;

      case 2:                 
        SerialBT.println("Reverse");
        Reverse();
        break;

      case 3:         
        SerialBT.println("Left");
        LeftTurn();
        StopCar();
        break;
        
      case 4:                     
        SerialBT.println("Right");
        RightTurn();
        StopCar();
        break;
        
      case 5:                                            
        SerialBT.println("Stop Car ");
        StopCar();
        break; 

      case 6:                 
        setWaypoint();
        break;
      
      case 7:        
        goWaypoint();
        break;  
      
      case 8:        
        SerialBT.println("Turn Around");
        turnAround();
        break;
      
      case 9:        
        SerialBT.println("Compass Forward");
        setHeading();
        Compass_Forward();
        break;
      
      case 10:
        setHeading();
        break; 

      case 11:
         gpsInfo();
        break;
      
      case 12:  
        SerialBT.println("Compass Turn Right");
        CompassTurnRight();
        break;
      
      case 13:  
        SerialBT.println("Compass Turn Left");
        CompassTurnLeft();
        break;
        
      case 14:  
        SerialBT.println("Calibrate Compass");
        calibrateCompass();
        break;

      case 15:  
        pingToggle();
        break;  

      case 16:
        clearWaypoints();
        break;  

      case 17:                    // finish with waypoints
        ac = 0;
        SerialBT.print("Waypoints Complete");
        break;
      case 18:                    // check soil quality
        if(chSoil == 0){
          checkSoil();
          SerialBT.print("Checking Soil");
          chSoil = 1;
        }else {
          uncheckSoil();
          SerialBT.print("Unchecking Soil");
          chSoil = 0;
        }
        break;
      

 } // end of switch case

// **************************************************************************************************************************************************  
// Slider Value for Speed

if (blueToothVal)                                    
  {    
   //Serial.println(blueToothVal);
  if (blueToothVal >= 1000)
{
    SerialBT.print("Speed set To:  ");
    SerialBT.println(blueToothVal - 1000);
    turn_Speed = (blueToothVal - 1000); 
    Serial.println();
    Serial.print("Turn Speed ");
    Serial.println(turn_Speed);
 } 
  }  

/*

// **************************************************************************************************************************************************
// Detect for Mines - Sweep Not Used

 else if (blueToothVal== 15)                                   
  {    
    SerialBT.println("Detecting");
    sweep();
  }

// **************************************************************************************************************************************************  
*/
 }                                                              // end of while loop SerialBT read
 
                                                               // if no data from Bluetooth 
   if (SerialBT.available() < 0)                                 // if an error occurs, confirm that the arduino mega board is selected in the Tools Menu
    {
     SerialBT.println("No Bluetooth Data ");          
    }
  
} // end of bluetooth procedure
