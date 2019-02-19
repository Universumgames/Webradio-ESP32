#ifndef MP3Decoder_h
#define MP3Decoder_h

#include <Arduino.h>
#include <SPI.h>

const uint8_t VS_WRITE = 0b00000010;
const uint8_t VS_READ = 0b00000011;

const uint8_t VS_ADDRESS_MODE = 0x0;
const uint8_t VS_ADDRESS_STATUS = 0x1;
const uint8_t VS_ADDRESS_BASS = 0x2;
const uint8_t VS_ADDRESS_CLOCKFREQ = 0x3;
const uint8_t VS_ADDRESS_DECODE_TIME = 0x4;
const uint8_t VS_ADDRESS_AUDIOSAMPLERATE = 0x5;
//const uint8_t VS_ADDRESS_WRAM = 0x6;
//const uint8_t VS_ADDRESS_WRAMADDR = 0x7;
//const uint8_t VS_ADDRESS_HDAT0 = 0x8;
//const uint8_t VS_ADDRESS_HDAT1 = 0x9;
//const uint8_t VS_ADDRESS_AIADDR = 0xA;
const uint8_t VS_ADDRESS_VOL = 0xB;
//const uint8_t VS_ADDRESS_AICTRL0 = 0xC;
//const uint8_t VS_ADDRESS_AICTRL1 = 0xD;
//const uint8_t VS_ADDRESS_AICTRL2 = 0xE;
//const uint8_t VS_ADDRESS_AICTRL3 = 0xF;

class MP3Decoder
{
  public:
    MP3Decoder(uint8_t _miso = MISO, uint8_t _mosi = MOSI, uint8_t _xcs = SS, uint8_t _sck = SCK, uint8_t _xdcs = -1, uint8_t _dreq = -1, uint8_t _xrst = -1)
    {
        cs = _xcs;
        dcs = _xdcs;
        dreq = _dreq;
        rst = _xrst;
        miso = _miso;
        mosi = _mosi;
        sck = _sck;
    }

    void begin()
    {
        settings = SPISettings(2000000, LSBFIRST, SPI_MODE1);
        //define GPIO´s
        pinMode(dreq, INPUT_PULLUP);
        pinMode(rst, OUTPUT);
        pinMode(cs, OUTPUT);

        digitalWrite(rst, HIGH);
        digitalWrite(cs, HIGH);

        hardReset();

        //begin SPI
        SPI.begin(sck, miso, mosi, cs);
        SPI.setBitOrder(settings._bitOrder);
        SPI.setFrequency(settings._clock);
        SPI.setDataMode(settings._dataMode);
        SPI.setClockDivider(SPI_CLOCK_DIV4);
        //SPI.setHwCs(false);
    }

    void hardReset()
    {
        digitalWrite(rst, LOW);
        delay(10);
        digitalWrite(rst, HIGH);
    }

    bool connected()
    {
        return digitalRead(dreq);
    }

    void playChunk(uint8_t *data, uint32_t length)
    {
        Chunk c;
        c.data = data;
        c.length = length;
        chunks->enqueue(c);
    }

    void loop()
    {
        play();
    }

    /*void initMP3Stream(){
        uint16_t *data;
        readMode(data);
        writeMode(data[0], data[1], data[2], 0, data[4], data[5], 1, data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14]);
    }*/

    void testConnection(){
        hardReset();
        SPI.beginTransaction(settings);
        digitalWrite(cs, LOW);
        uint8_t byte1 = VS_WRITE;
        uint8_t byte2 = 00000100;
        uint8_t byte3 = 00000000;
        //uint8_t byte4 = 00000000;
        SPI.transfer(byte1);
        SPI.transfer(byte2);
        SPI.transfer(byte3);
        //uint32_t bytes = 000001000000000000000000;
        //SPI.transfer32(bytes);
        /*SPI.write(VS_WRITE);
        SPI.write(0x0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(1);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);*/
        digitalWrite(cs, HIGH);
        SPI.endTransaction();

        SPI.beginTransaction(settings);
        digitalWrite(cs, LOW);
        uint8_t byte11 = 0x53;
        uint8_t byte21 = 0xEF;
        uint8_t byte31 = 0x6E;
        uint8_t byte41 = 0b01111110;
        uint8_t byte51 = 0;
        uint8_t byte61 = 0;
        uint8_t byte71 = 0;
        uint8_t byte81 = 0;
        SPI.transfer(byte11);
        SPI.transfer(byte21);
        SPI.transfer(byte31);
        SPI.transfer(byte41);
        SPI.transfer(byte51);
        SPI.transfer(byte61);
        SPI.transfer(byte71);
        SPI.transfer(byte81);
        /*SPI.write(0x53);
        SPI.write(0xEF);
        SPI.write(0x6E);
        SPI.write(0b01111110);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);
        SPI.write(0);*/
        digitalWrite(cs, HIGH);
        SPI.endTransaction();
    }

  private:
    struct Chunk
    {
        uint8_t *data;
        uint32_t length = 15;
    };
    uint8_t cs;
    uint8_t dcs;
    uint8_t dreq;
    uint8_t rst;
    uint8_t miso;
    uint8_t mosi;
    uint8_t sck;
    SPISettings settings;
    Queue<Chunk> *chunks;

    bool DREQ()
    {
        return digitalRead(dreq);
    }

    void waitForRequest()
    {
        while (!DREQ())
        {
            ;
        }
    }

    void play()
    {
        Chunk front = chunks->front();
        chunks->dequeue();
        writeBytes(0x5, front.data, front.length);
    }

    void writeBytes(uint8_t adress, uint8_t *data, uint32_t length = 16)
    {
        waitForRequest();
        digitalWrite(cs, LOW);
        SPI.beginTransaction(settings);
        //Write writeIntro
        SPI.write(VS_WRITE);
        //Write adress
        SPI.write(adress);
        //Write data
        SPI.writeBytes(data, length);
        SPI.endTransaction();
        digitalWrite(cs, HIGH);
    }

    uint16_t readByte(uint8_t adress)
    {
        uint8_t data;
        uint32_t size = 16;
        waitForRequest();
        digitalWrite(cs, LOW);
        SPI.beginTransaction(settings);
        //write readIntro
        SPI.write(VS_READ);
        //write adress
        SPI.write(adress);
        //read result
        SPI.transfer(&data, size);
        waitForRequest();
        SPI.endTransaction();
        digitalWrite(cs, HIGH);
        return data;
    }

    void readMode(uint16_t *data){
        uint16_t d = readByte(VS_ADDRESS_MODE);
        data = &d;
    }

    void writeMode(bool diff, bool settozero, bool reset, bool outofwav, bool powerdown, bool tests, bool stream, bool settozero2, bool dact, bool sdiord, bool sdishare, bool sdinew, bool adpcm, bool adpcm_hp, bool line_in){
        uint8_t *data;// = {diff, settozero, reset, outofwav, powerdown, tests, stream, settozero2, dact, sdiord, sdishare, sdinew, adpcm, adpcm_hp, line_in};
        data[0] = diff;
        data[1] = settozero;
        data[2] = reset;
        data[3] = outofwav;
        data[4] = powerdown;
        data[5] = tests;
        data[6] = stream;
        data[7] = settozero2;
        data[8] = dact;
        data[9] = sdiord;
        data[10] = sdishare;
        data[11] = sdinew;
        data[12] = adpcm;
        data[13] = adpcm_hp;
        data[14] = line_in;
        writeBytes(VS_ADDRESS_MODE, data);
    }
};
#endif