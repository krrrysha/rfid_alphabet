#include "LCD_1602_RUS.h"
#include <SPI.h>
#include <MFRC522.h>
#include <avr/pgmspace.h>
#include <Fongen.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

LCD_1602_RUS lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN); 
   Fongen myFongen;

wchar_t* bukva1[] = {L"А", L"Б", L"В", L"Г", L"Д", L"Е", L"Ё", L"Ж", L"З", L"И", L"Й", L"К", L"Л", L"М", L"Н", L"О", L"П", L"Р", L"С", L"Т", L"У", L"Ф", L"Х", L"Ц", L"Ч", L"Ш", L"Щ", L"Ъ", L"Ы", L"Ь", L"Э", L"Ю", L"Я"};



const byte PROGMEM serNum0[] = {0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,  0x34, 0x34, 0x34, 0x34, 0x34, 0x34};            
const byte PROGMEM serNum1[] = {0x78, 0x3D, 0x31, 0xE7, 0x31, 0xC0, 0x41, 0x31, 0x41, 0x78, 0x3D, 0x60, 0xE7, 0xE7, 0x78, 0xE7, 0x3D, 0x41, 0x39, 0xE7, 0x78, 0xE7, 0x3D, 0x6C, 0x78, 0x06, 0x6C,  0xE7, 0x6C, 0x31, 0x8B, 0x39, 0xE7};             
const byte PROGMEM serNum2[] = {0x4B, 0x6D, 0xB4, 0x80, 0xB7, 0x78, 0x55, 0xB7, 0x55, 0x4B, 0x6C, 0xF0, 0x7C, 0x90, 0x4B, 0x92, 0x6E, 0x56, 0x86, 0x91, 0x4B, 0x7C, 0x6F, 0x96, 0x4B, 0xBB, 0x96,  0x92, 0x94, 0xB7, 0xDE, 0x83, 0x92};             
const byte PROGMEM serNum3[] = {0x41, 0x99, 0x91, 0x79, 0x41, 0x31, 0x71, 0x91, 0x71, 0x51, 0x61, 0x21, 0x61, 0x51, 0x41, 0x51, 0x11, 0x51, 0x09, 0x21, 0x41, 0x61, 0x21, 0x91, 0x41, 0xA1, 0xC9,  0x59, 0x61, 0x49, 0x69, 0xB1, 0x51};             
const byte PROGMEM serNum4[] = {0x5D, 0x2C, 0x1A, 0x6A, 0x91, 0x22, 0x5A, 0x73, 0x48, 0xF9, 0x09, 0x5C, 0x8E, 0xC3, 0x43, 0x61, 0x4F, 0x2F, 0x5B, 0x46, 0xEA, 0xDE, 0x0E, 0xDB, 0x02, 0x23, 0xCD,  0x3C, 0xE8, 0xBA, 0x6B, 0x88, 0x47};            
const byte PROGMEM serNum5[] = {0x8B, 0x0C, 0xAA, 0xAF, 0x93, 0xA7, 0x01, 0x8B, 0xCF, 0xE8, 0xE1, 0xBC, 0xB0, 0x91, 0x0B, 0xC2, 0xF3, 0x2B, 0x23, 0xF2, 0x02, 0x10, 0xB7, 0xBE, 0x01, 0xFB, 0xC3,  0x87, 0x14, 0x1B, 0x1A, 0xAB, 0x69};             
const byte PROGMEM serNum6[] = {0x67, 0x86, 0x87, 0x76, 0x86, 0x76, 0x86, 0x86, 0x86, 0x66, 0x86, 0x67, 0x76, 0x76, 0x67, 0x77, 0x86, 0x87, 0x87, 0x77, 0x66, 0x76, 0x87, 0x66, 0x67, 0x87, 0x66,  0x77, 0x66, 0x86, 0x66, 0x86, 0x77};             

const byte COUNT_BUKV = 33;

byte now_state;
byte bukva_idx;

wchar_t* bukva; 

byte round_i;

// music Winx?

const int Pin_tone = 8; // номер порта зуммера
const byte winx_count = 24; // Колличество нот
//частоты ноты
const word PROGMEM winx_freqs[winx_count] = 
{329, 329, 329, 261, 261, 329, 293, 329, 329, 329, 329, 349, 329, 261, 329, 329, 329, 261, 261, 329, 293, 392, 440, 392};
//длительность нот
const word PROGMEM winx_dur[winx_count] = 
{125, 125, 250, 250, 250, 250, 750, 125, 125, 250, 250, 250, 250, 750, 125, 125, 250, 250, 250, 250, 500, 500, 500, 500};

const byte sad_count = 13; // Колличество нот
//частоты ноты

const word PROGMEM sad_freqs[sad_count] = 
{440,415,392,370,349,329,311,293,277,261,247,233,220};
//длительность нот
const word PROGMEM sad_dur[sad_count] = 
{125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};


const byte milaya_count = 22; // Колличество нот
const word PROGMEM milaya_freqs[milaya_count] = 
{329, 261, 220, 261, 174, 293, 246, 220, 246, 220, 196, 261, 246, 220, 261, 174, 164, 164, 184, 207, 246, 220};
const word PROGMEM milaya_dur[milaya_count] = 
{250, 125, 250, 125, 750, 250, 125, 250, 125, 250, 500, 250, 125, 250, 125, 750, 250, 125, 250, 125, 250, 500};

const byte edem_count = 26; // Колличество нот
const word PROGMEM edem_freqs[edem_count] = 
{392, 329, 329, 293, 293, 261, 261, 246, 220, 246, 261, 293, 196, 261, 246, 220, 246, 261, 293, 293, 440, 392, 349, 329, 293, 261};
const word PROGMEM edem_dur[edem_count] = 
{125, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 375, 125, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 375};

const byte kuznec_count = 27; // Колличество нот
const word PROGMEM kuznec_freqs[kuznec_count] = 
{440, 329, 440, 329, 440, 415, 415, 415, 329, 415, 329, 415, 440, 440, 440, 329, 440, 329, 440, 415, 415, 415, 329, 415, 329, 415, 440};
const word PROGMEM kuznec_dur[kuznec_count] = 
{125, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 375};

long randNumber;

unsigned long timerStart;
unsigned long timerStartSnd;

const char rus_string1[] PROGMEM="A ";
const char rus_string2[] PROGMEM="Be ";
const char rus_string3[] PROGMEM="Ve ";
const char rus_string4[] PROGMEM="Ge ";
const char rus_string5[] PROGMEM="De ";
const char rus_string6[] PROGMEM="E "; //не раборает
const char rus_string7[] PROGMEM="%O ";
const char rus_string8[] PROGMEM="Je ";
const char rus_string9[] PROGMEM="Ze "; //
const char rus_string10[] PROGMEM="I ";
const char rus_string11[] PROGMEM="I KRATKAE ";
const char rus_string12[] PROGMEM="KA ";
const char rus_string13[] PROGMEM="eL ";
const char rus_string14[] PROGMEM="eM ";
const char rus_string15[] PROGMEM="eN ";
const char rus_string16[] PROGMEM="O ";
const char rus_string17[] PROGMEM="Pe "; //не раборает
const char rus_string18[] PROGMEM="eR ";
const char rus_string19[] PROGMEM="eS ";
const char rus_string20[] PROGMEM="Te ";
const char rus_string21[] PROGMEM="y ";
const char rus_string22[] PROGMEM="eF ";
const char rus_string23[] PROGMEM="XA ";
const char rus_string24[] PROGMEM="Ce ";
const char rus_string25[] PROGMEM="He ";
const char rus_string26[] PROGMEM="WA ";
const char rus_string27[] PROGMEM="wA ";
const char rus_string28[] PROGMEM="TV%ORDYIZNAK ";
const char rus_string29[] PROGMEM="Y ";
const char rus_string30[] PROGMEM="MQGKIZNAK ";
const char rus_string31[] PROGMEM="e ";
const char rus_string32[] PROGMEM="U ";
const char rus_string33[] PROGMEM="Q ";


const char* const  rus_string[] PROGMEM = {rus_string1,rus_string2,rus_string3,rus_string4,rus_string5,rus_string6,rus_string7,rus_string8,rus_string9,rus_string10,rus_string11,rus_string12,rus_string13,rus_string14,rus_string15,rus_string16,rus_string17,rus_string18,rus_string19,rus_string20,rus_string21,rus_string22,rus_string23,rus_string24,rus_string25,rus_string26,rus_string27,rus_string28,rus_string29,rus_string30,rus_string31,rus_string32,rus_string33};



const char rus_word1[] PROGMEM="ARBUZ";
const char rus_word2[] PROGMEM="BARABAN";
const char rus_word3[] PROGMEM="VINOGRAD";
const char rus_word4[] PROGMEM="GRIBY";
const char rus_word5[] PROGMEM="DOM";
const char rus_word6[] PROGMEM="ENOT"; //не раборает
const char rus_word7[] PROGMEM="%OJ";
const char rus_word8[] PROGMEM="JyK";
const char rus_word9[] PROGMEM="ZMEEQ"; //
const char rus_word10[] PROGMEM="INDUK";
const char rus_word11[] PROGMEM="%OGURT";
const char rus_word12[] PROGMEM="KENGyRy";
const char rus_word13[] PROGMEM="LIV%NOK";
const char rus_word14[] PROGMEM="MAWYNA";
const char rus_word15[] PROGMEM="NOJNICY";
const char rus_word16[] PROGMEM="OSMINOG";
const char rus_word17[] PROGMEM="PARAVOZ"; //не раборает
const char rus_word18[] PROGMEM="RYBKA";
const char rus_word19[] PROGMEM="SAMAL%T";
const char rus_word20[] PROGMEM="TILIFON";
const char rus_word21[] PROGMEM="yLITKA";
const char rus_word22[] PROGMEM="FLAG";
const char rus_word23[] PROGMEM="XLEB";
const char rus_word24[] PROGMEM="CYPL%NAK";
const char rus_word25[] PROGMEM="HEREPAXA";
const char rus_word26[] PROGMEM="WARIK";
const char rus_word27[] PROGMEM="wOTKA";
const char rus_word28[] PROGMEM=" ";
const char rus_word29[] PROGMEM=" ";
const char rus_word30[] PROGMEM=" ";
const char rus_word31[] PROGMEM="eKSKAVATOR";
const char rus_word32[] PROGMEM="ULA";
const char rus_word33[] PROGMEM="QBLOKO";

const char* const  rus_word[] PROGMEM = {rus_word1,rus_word2,rus_word3,rus_word4,rus_word5,rus_word6,rus_word7,rus_word8,rus_word9,rus_word10,rus_word11,rus_word12,rus_word13,rus_word14,rus_word15,rus_word16,rus_word17,rus_word18,rus_word19,rus_word20,rus_word21,rus_word22,rus_word23,rus_word24,rus_word25,rus_word26,rus_word27,rus_word28,rus_word29,rus_word30,rus_word31,rus_word32,rus_word33};


char char_string[30];

// wchar_t* rus_string;



//byte ascii_start_code=42;



//const int pin_tone=8;
const byte pin_tone=B00000001;
//const byte port_shift=0;


void nfc_init() {
    SPI.begin(); 
  rfid.PCD_Init(); // Init MFRC522 
}


void setup()
{

 lcd.init();    
 lcd.backlight();
 now_state=1; // буква не выбрана
 Serial.begin(9600);
 timerStart=millis();
 timerStartSnd=millis();
 pinMode(Pin_tone, OUTPUT);  // Настраиваем контакт на выход 
//DDRB=B0000001;
   round_i = 0;
nfc_init();

}




void music_play(int mus_pin, word *mus_frequences, word *mus_durations, int mus_count, byte mus_temp) {
word musdur;
word musfreq;

  
  for (int i = 0; i <= mus_count-1; i++  ) { // Цикл от 0 по количеству нот
    musdur=pgm_read_word_near (mus_durations+i);
    musfreq=pgm_read_word_near (mus_frequences+i);
    tone(mus_pin, musfreq, musdur * 2); // Включаем звук, определенной частоты
    delay(musdur * 2/mus_temp);  // Пауза для заданой ноты
    noTone(mus_pin); // Останавливаем звук
  }
}

void sleep_lcd (unsigned long timerStart_lcd,unsigned long timerLength_lcd) {
if (millis()-timerStart_lcd >= timerLength_lcd)
{
  lcd.off();
} else
 lcd.on();
}

void sleep_lamp (unsigned long timerStart_lamp, unsigned long timerLength_lamp) {
if (millis()-timerStart_lamp >= timerLength_lamp)
{
  lcd.noBacklight();
} else 
{
 lcd.backlight();
}
}

void sleep_sound (unsigned long timerStart_snd, unsigned long timerLength_snd) {
       
if (millis()-timerStart_snd >= timerLength_snd)
{
  music_play(Pin_tone, sad_freqs, sad_dur, sad_count, 2);
  timerStartSnd=millis();
}
}

void test_keys(){
if (digitalRead(7)==HIGH || digitalRead(6)==HIGH )
{
  timerStart=millis();
  timerStartSnd=timerStart;
  }

sleep_lcd (timerStart,240000);
sleep_lamp (timerStart,240000);
sleep_sound (timerStartSnd,600000);
}





void loop()
{

byte ser0;
byte ser1;
byte ser2;
byte ser3;
byte ser4;
byte ser5;
byte ser6;

test_keys();


switch (now_state) {
case 1:

  if (digitalRead(7)==LOW) {
  if (round_i <= COUNT_BUKV-1) {
   lcd.clear();

   lcd.setCursor(7, 1);
   bukva=bukva1[round_i];
   bukva_idx=round_i;
   lcd.print(bukva);
   delay (200);
    round_i++;}
    else
    {round_i = 0;}
    } else {
    now_state=2; //буква выбрана
    tone(Pin_tone, 261, 250);
    timerStart=millis();
    timerStartSnd=millis();   
delay(500);
strlcpy(char_string, "ByKVA", 30);

myFongen.string_speaker(char_string,pin_tone);
delay(1000);
//rus_to_fonem();
strcpy_P(char_string, (char*)pgm_read_word(&(rus_string[bukva_idx])));

myFongen.string_speaker(char_string,pin_tone);
delay(1000);
strcpy_P(char_string, (char*)pgm_read_word(&(rus_word[bukva_idx])));
myFongen.string_speaker(char_string,pin_tone);
Serial.print ("rus_string[bukva_idx] ");
Serial.print (bukva_idx);
Serial.println(char_string);
    }
   break;
case 2:   

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;  
   MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
       
     ser0=pgm_read_byte_near (serNum0+bukva_idx);
     ser1=pgm_read_byte_near (serNum1+bukva_idx);
     ser2=pgm_read_byte_near (serNum2+bukva_idx);
     ser3=pgm_read_byte_near (serNum3+bukva_idx);
     ser4=pgm_read_byte_near (serNum4+bukva_idx);
     ser5=pgm_read_byte_near (serNum5+bukva_idx);
     ser6=pgm_read_byte_near (serNum6+bukva_idx);
     
     if (rfid.uid.uidByte[0] == ser0 && rfid.uid.uidByte[1] == ser1 && rfid.uid.uidByte[2] == ser2 
   && rfid.uid.uidByte[3] == ser3 && rfid.uid.uidByte[4] == ser4 && rfid.uid.uidByte[5] == ser5 && rfid.uid.uidByte[6] == ser6 )
//    if (1==1)  
      {
         lcd.setCursor(6, 0);
         lcd.print(":-)");
         randNumber=random(4);

        switch (randNumber) {
         

         case 0:      
          music_play(Pin_tone, winx_freqs, winx_dur, winx_count, 2);
          break;
         case 1:
          music_play(Pin_tone, milaya_freqs, milaya_dur, milaya_count, 1);
          break;
         case 2: 
          music_play(Pin_tone, edem_freqs, edem_dur, edem_count, 1);
          break;
         case 3: 
          music_play(Pin_tone, kuznec_freqs, kuznec_dur, kuznec_count, 1);
          break;          
         }
      } else
      {
         lcd.setCursor(6, 0);
         lcd.print(":-(");  
         music_play(Pin_tone, sad_freqs, sad_dur, sad_count, 2);
      }

          delay (2000);
         now_state=1;
         break;
 //}
}
}


