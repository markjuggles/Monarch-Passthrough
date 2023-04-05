// Program: Monarch-Passthrough.ino
// Purpose: Allow access to the GM02S via the external UART pins (Serial2).  
//
// This is derived from Microchip AVR-IoT Cell Mini examples.  
// Microchip license restrictions apply.
//
// This implements a psuedo-command to do a hardware reset:
// GM02S^RESET
// It will respond immediately with an "OK\r\n" and later with a "+SYSSTART\r\n"

// Compile+Upload with Ctrl-Shift-U

#include <Arduino.h>
#include <log.h>
#include <sequans_controller.h>

void debugBridgeUpdate(void);

// ------------------------------ DEBUG BRIDGE ----------------------------- //

#ifdef __AVR_AVR128DB48__ // MINI

#define SerialDebug Serial3   // USB Console

#else
#ifdef __AVR_AVR128DB64__ // Non-Mini

#define SerialDebug Serial5

#else
#error "INCOMPATIBLE_DEVICE_SELECTED"
#endif
#endif

#define RESET_PIN   PIN_PC5


void setup()
{
    Log.begin(115200);
    SerialDebug.println("The external UART is connected to the GM02S");

    Serial2.swap(1);
    Serial2.begin(115200);

    SequansController.begin();
}

void loop()
{
    static uint8_t data;
    static char ResetCommand[] = "GM02S^RESET\n";
    static int  match = 0;

    // Read input from the external facing UART (Serial2).
    if (Serial2.available())
    {
        data = Serial2.read();

        if(data == ResetCommand[match++])
        {
          // If we reached the end of the string, then the command was received.
          if(ResetCommand[match] == '\0')
          {
            Serial2.write('O');
            Serial2.write('K');
            Serial2.write('\r');
            Serial2.write('\n');
            match = 0;
            // Reset the GM02S
            pinConfigure(RESET_PIN, PIN_DIR_OUTPUT | PIN_INPUT_ENABLE);
            digitalWrite(RESET_PIN, HIGH);
            delay(10);
            digitalWrite(RESET_PIN, LOW);
          }
        }
        else
        {
          match = 0;          
        }
        
        SequansController.writeBytes((const uint8_t*)&data, 1);
    }

    if (SequansController.isRxReady()) 
    {
        // Display data from the GM02S to the external UART and the USB UART.
        data = SequansController.readByte();
        //SerialDebug.write(data);
        Serial2.write(data);
    }
}
