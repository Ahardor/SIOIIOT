#include "OneWire.h"

OneWire oneWire(15); 
 
void setup(void) {
    Serial.begin(115200);
}
 
void loop(void) {
    int i;
    byte addr[8];
 
    if (!oneWire.search(addr)) {
        Serial.println(" No more addresses.");
        Serial.println();
        oneWire.reset_search();
        delay(250);
        return;
    }
    
    Serial.print(" ROM =");
    
    for (i = 0; i < 8; i++) {
        Serial.write(' ');
        Serial.print(addr[i], HEX);
    }
}