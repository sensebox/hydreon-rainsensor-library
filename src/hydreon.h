/*
  Hydreon Rainsensor Library for senseBox
*/



#ifndef HYDREON_h
#define HYDREON_h

class HYDREON
{
  public:
    HYDREON(Uart& serial);
    void begin();
    void setContinuousMode(bool cmd);
    void pollContinuousData();
    float getAcc(); //get Accumulation data as float
    float getEventAcc(); //get Accumulation since last boot data as float
    //float getTotalAcc(); //get total Accumulation data as float
    void getAllData(); // ge
    void setHighResolution(bool cmd); // set to HighResolution mode if true 
    void sendCmd(char cmd); // helper function to send serial commands to the sensor
    void recvWithEndMarker(char cmd);
    void showNewData();
    float CharToFloat(const char *str); // convert char to float
    float Rg15Parse(char* buffer, const char* item); //parse data from the buffer to convert to float

  private:
    boolean newData = false;
    const byte numChars = 32;
    char rain_sensitivity[64]; // an array to store the received data
    float acc = 0.0f;
    Uart& sensor;

};

#endif
