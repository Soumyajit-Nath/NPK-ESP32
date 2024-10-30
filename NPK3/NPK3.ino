// #include <Arduino.h>
// #include <SoftwareSerial.h>
// #include <Wire.h>

// #define RE 4
// #define DE 2

// //const byte code[]= {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};
// const byte nitro[] = { 0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c };
// const byte phos[] = { 0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc };
// const byte pota[] = { 0x01, 0x0   3, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0 };

// byte values[11];
// //SoftwareSerial mod(2, 3); //FOR UNO
// SoftwareSerial mod(22, 23);

// void setup() {
//   Serial.begin(9600);
//   mod.begin(9600);
//   pinMode(RE, OUTPUT);
//   pinMode(DE, OUTPUT);
// }

// void loop() {
//   byte val1, val2, val3;
//   val1 = nitrogen();
//   delay(250);
//   val2 = phosphorous();
//   delay(250);
//   val3 = potassium();
//   delay(250);


//   Serial.print("Nitrogen: ");
//   Serial.print(val1);
//   Serial.println(" mg/kg");
//   Serial.print("Phosphorous: ");
//   Serial.print(val2);
//   Serial.println(" mg/kg");
//   Serial.print("Potassium: ");
//   Serial.print(val3);
//   Serial.println(" mg/kg");
//   delay(2000);
// }

// byte nitrogen() {
//   digitalWrite(DE, HIGH);
//   digitalWrite(RE, HIGH);
//   delay(50);
//   if (mod.write(nitro, sizeof(nitro)) == 8) {
//     digitalWrite(DE, LOW);
//     digitalWrite(RE, LOW);
//     delay(100);

//     for (byte i = 0; i < 7; i++) {
//       //Serial.print(mod.read(),HEX);
//       if (mod.available()) {
//         values[i] = mod.read();
//         Serial.print(values[i], HEX);
//       }
//     }
//     Serial.println();
//   }
//   return values[4];
// }

// byte phosphorous() {
//   digitalWrite(DE, HIGH);
//   digitalWrite(RE, HIGH);
//   delay(10);
//   if (mod.write(phos, sizeof(phos)) == 8) {
//     digitalWrite(DE, LOW);
//     digitalWrite(RE, LOW);
//     delay(100);

//     for (byte i = 0; i < 7; i++) {
//       //Serial.print(mod.read(),HEX);
//       if (mod.available()) {
//         values[i] = mod.read();
//         Serial.print(values[i], HEX);
//       }
//     }
//     Serial.println();
//   }
//   return values[4];
// }

// byte potassium() {
//   digitalWrite(DE, HIGH);
//   digitalWrite(RE, HIGH);
//   delay(10);
//   if (mod.write(pota, sizeof(pota)) == 8) {
//     digitalWrite(DE, LOW);
//     digitalWrite(RE, LOW);
//     delay(100);

//     for (byte i = 0; i < 7; i++) {
//       //Serial.print(mod.read(),HEX);
//       if (mod.available()) {
//         values[i] = mod.read();
//         Serial.print(values[i], HEX);
//       }
//     }
//     Serial.println();
//   }
//   return values[4];
// }






#include <SoftwareSerial.h>
#include <Wire.h>
#include <pins_arduino.h>

#define RE 4
#define DE 2

const uint8_t code[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};
uint8_t values[11];
SoftwareSerial mod(22, 23);

int nitrogen;
int phosphorous;
int potassium;

struct MyVariable {
    int nitrogen;
    int phosphorous;
    int potassium;
};
MyVariable variable;

void setup() {
    Serial.begin(9600);
    mod.begin(9600);
    pinMode(RE, OUTPUT);
    pinMode(DE, OUTPUT);
    delay(10000);
}

void loop() {
    // Prepare to send the Modbus command to the NPK sensor
    digitalWrite(DE, HIGH);
    digitalWrite(RE, HIGH);
    delay(10);

    // Send the query command to the NPK sensor
    if(mod.write(code, sizeof(code)) == 8) {
        digitalWrite(DE, LOW);
        digitalWrite(RE, LOW);
        delay(100); // Give time for the response to arrive

        // Check if we have enough data to read (11 bytes expected)
        if (mod.available() >= 11) {
            for(int i = 0; i < 11; i++) {
                values[i] = mod.read();
                Serial.print(values[i], HEX);
                Serial.print(" ");
            }
            Serial.println();

            // Combine two bytes for each value (16-bit data)
            nitrogen = (values[3] << 8) | values[4];
            phosphorous = (values[5] << 8) | values[6];
            potassium = (values[7] << 8) | values[8];

            // Store the results in the struct
            variable.nitrogen = nitrogen;
            variable.phosphorous = phosphorous;
            variable.potassium = potassium;

            // Output the results
            Serial.print("Nitrogen: ");
            Serial.print(variable.nitrogen);
            Serial.println(" mg/kg");
            Serial.print("Phosphorous: ");
            Serial.print(variable.phosphorous);
            Serial.println(" mg/kg");
            Serial.print("Potassium: ");
            Serial.print(variable.potassium);
            Serial.println(" mg/kg");
        } else {
            Serial.println("No data received");
        }
    }

    // Delay before the next query
    delay(2000);
}







