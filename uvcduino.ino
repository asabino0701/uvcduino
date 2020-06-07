// Programa : Relay and display for secure lamp UV-C. 
// Autor : Aleksander Sabino da Silva
// Data: 29/05/2020

// So even if we compare SARS CoV-2 to highest D90 dose of SARS coronavirus (Urbani),
// Desired direct exposure time in seconds is:
// Exposure_time = Desired_UV_dose (multiplied by) 4 (multiplied by) pi (multiplied by) (UV_bulb_distance)^2 )/ UV_bulb_power
// Units

// So, Exposure time= 241 J/m2 (x) 4 (x) 3.14 (x) 1.8 /80 = 68 seconds.

// Lâmpada UV-C 25W = 241 J/m2 (x) 4 (x) 3.14 (x) 2.5 / 25 = 303 Segundos - 5,03 minutos.
#include "IRremote.h"
#include "LedControl.h"
#include "binary.h"

int pinorelay = 3;  
int t_delay = 1000, imax = 30, seg = 0;
int t1 = 0, t2 = 0, t_total = 0;
boolean relay = false;
  
// IR Remote
const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

// LCD 8x8 
LedControl lc = LedControl(2,6,5,1);

// Matriz representando minutos no display 8x8.
byte trinta[8]= {B00000000,B11111111,B00011001,B11111001,B00011001,B00011001,B11111111,B0000000};
byte vintenove[8]= {B00000000,B11111111,B00011001,B11111111,B10000001,B10000001,B11110001,B0000000};
byte vinteoito[8]= {B00000000,B11111111,B00011001,B11111111,B10001001,B10001001,B11111111,B0000000};
byte vintesete[8]= {B00000000,B11111111,B00010001,B11110010,B10000100,B10001000,B11110000,B0000000};
byte vinteseis[8]= {B00000000,B11111111,B00011000,B11111000,B10001111,B10001001,B11111111,B0000000};
byte vintecinco[8]= {B00000000,B11111111,B00011000,B11111111,B10000001,B10000001,B11111111,B0000000};
byte vintequatro[8]= {B00000000,B11111001,B00011001,B11111111,B10000001,B10000001,B11110001,B0000000};
byte vintetres[8]= {B00000000,B11111111,B00010001,B11111111,B10000001,B10000001,B11111111,B0000000};
byte vintedois[8]= {B00000000,B11111111,B00010001,B11111111,B10001000,B10001000,B11111111,B0000000};
byte vinteum[8]= {B00000000,B11110001,B00010001,B11110001,B10000001,B10000001,B11110001,B0000000};
byte vinte[8]= {B00000000,B11111111,B00011001,B11111001,B10001001,B10001001,B11111111,B0000000};
byte dezenove[8]= {B00000000,B10001111,B10001001,B10001111,B10000001,B10000001,B10000001,B0000000};
byte dezoito[8]= {B00000000,B10001111,B10001001,B10001111,B10001001,B10001001,B10001111,B0000000};
byte dezessete[8]= {B00000000,B10001111,B10000001,B10000010,B10000100,B10001000,B10000000,B0000000};
byte dezesseis[8]= {B00000000,B10001111,B10001000,B10001000,B10001111,B10001001,B10001111,B0000000};
byte quinze[8]= {B00000000,B10001111,B10001000,B10001111,B10000001,B10000001,B10001111,B0000000};
byte quatorze[8]= {B00000000,B10001001,B10001001,B10001111,B10000001,B10000001,B10000001,B0000000};
byte treze[8]= {B00000000,B10001111,B10000001,B10001111,B10000001,B10000001,B10001111,B0000000};
byte doze[8]= {B00000000,B10001111,B10000001,B10000001,B10001111,B10001000,B10001111,B0000000};
byte onze[8]= {B00000000,B10000001,B10000001,B10000001,B10000001,B10000001,B10000001,B0000000};
byte dez[8]= {B00000000,B10001111,B10001001,B10001001,B10001001,B10001001,B10001111,B0000000};
byte nove[8]= {B00000000,B00001111,B00001001,B00001111,B00000001,B00000001,B00000001,B0000000};
byte oito[8]= {B00000000,B00001111,B00001001,B00001111,B00001001,B00001001,B00001111,B0000000};
byte sete[8]= {B00000000,B00001111,B00000001,B00000010,B00000100,B00001000,B00000000,B0000000};
byte seis[8]= {B00000000,B00001000,B00001000,B00001000,B00001111,B00001001,B00001111,B0000000};
byte cinco[8]= {B00000000,B00001111,B00001000,B00001111,B00000001,B0000001,B0001111,B0000000};
byte quatro[8]= {B00000000,B00001001,B00001001,B00001111,B00000001,B00000001,B00000001,B0000000};
byte tres[8]= {B00000000,B00001111,B00000001,B00001111,B00000001,B00000001,B00001111,B0000000};
byte dois[8]= {B00000000,B00001111,B00000001,B00001111,B00001000,B00001000,B00001111,B0000000};
byte um[8]= {B00000000,B0000001,B00000001,B00000001,B00000001,B00000001,B00000001,B0000000};

// Faces
byte ff[8]= {B00111100,B01000010,B10100101,B10000001,B10100101,B10011001,B01000010,B00111100};
byte fn[8]={B00111100, B01000010,B10100101,B10000001,B10111101,B10000001,B01000010,B00111100};
byte ft[8]= {B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B01000010,B00111100};

// Cov-Sars-2
byte covok[8]={B10011001, B01100110,B01000010,B10000001,B10000001,B01000010,B01100110,B10011001};
byte covolho[8]={B10011001, B01100110,B01010110,B10000001,B10000001,B01000010,B01100110,B10011001};
byte covx[8]={B10011001, B01100110,B01100110,B10011001,B10011001,B01100110,B01100110,B10011001};
byte covx2[8]={B10011001, B01100110,B01100110,B11111111,B11111111,B01100110,B01100110,B10011001};
byte covx3[8]={B10011001, B01111110,B01111110,B11111111,B11111111,B01111110,B01111110,B10011001};
byte covx4[8]={B11111111, B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};

void setup()  
{  
  // IR remote
  Serial.begin(9600);
  irrecv.enableIRIn(); 
  irrecv.blink13(true);

  // Relay
  pinMode(pinorelay, OUTPUT);  
  
  // Display LCD 8x8
  lc.shutdown(0,false);
 
  // seta o brilho para um valor mediano:
  //-------------------------------------
  lc.setIntensity(0,5);
 
  // limpa o display:
  //-----------------
  lc.clearDisplay(0);

  lcd_display(covok);
}  
  
void loop()  
  {  
    if (irrecv.decode(&results))
      {
      if (results.value == 0XFFFFFFFF) 
        {
        results.value = key_value;
        //lc.clearDisplay(0);
        }
               
        switch(results.value)
          {
          case 0xFFA25D:          
          //lcd.print("CH-");
          if (relay == true)
            {
            reset();
            }
          break;
          case 0xFF629D:
          //lcd.print("CH");
          if (relay == true)
            {
            reset();
            }
          break;
          case 0xFFE21D:
          //lcd.print("CH+");
          if (relay == true)
            {
            reset();
            }
          break;
          case 0xFF22DD:
          //lcd.print("|<<");
          if (relay == true and t_total >= 1) // pause 
            {
            relay = false;
            digitalWrite(pinorelay, LOW);
            number_lcd(t_total);  
            }
          break;
          case 0xFF02FD:
          //lcd.print(">>|");
          if (relay == true and t_total >= 1) // pause 
            {
            relay = false;
            digitalWrite(pinorelay, LOW);
            number_lcd(t_total);  
            }
          break ;  
          case 0xFFC23D:
          //lcd.print(">|");
          if (t1 != 0 and t_total == 0 and relay == false)
            {
            t_total = t1;
            }
          acende_uvc();
          break ;               
          case 0xFFE01F:
          //lcd.print("-");
          if (t1 != 0 and t_total == 0 and t1 <= 9 and t1 > 1)
            {
            t1--;
            number_lcd(t1);
            }
          else if (t_total <= 30 and t_total > 1)
            {
            t_total--;
            number_lcd(t_total);
            }
          break ;  
          case 0xFFA857:
          //lcd.print("+");
          if (t1 != 0 and t_total == 0 and t1 < 9 and t1 >= 1)
            {
            t1++;
            number_lcd(t1);
            }
          else if (t_total < 30  and t_total >= 0)
            {
            t_total++;
            number_lcd(t_total);
            }          
          break ;  
          case 0xFF906F:
          //lcd.print("EQ");
          break ;  
          case 0xFF6897:
          //lcd.print("0");
          calcula_tempo(0);
          break ;  
          case 0xFF9867:
          //lcd.print("100+");
          break ;
          case 0xFFB04F:
          //lcd.print("200+");
          break ;
          case 0xFF30CF:
          //lcd.print("1");
          calcula_tempo(1);
          break ;
          case 0xFF18E7:
          //lcd.print("2");
          calcula_tempo(2);
          break ;
          case 0xFF7A85:
          //lcd.print("3");
          calcula_tempo(3);
          break ;
          case 0xFF10EF:
          //lcd.print("4");
          calcula_tempo(4);
          break ;
          case 0xFF38C7:
          //lcd.print("5");
          calcula_tempo(5);
          break ;
          case 0xFF5AA5:
          //lcd.print("6");
          calcula_tempo(6);
          break ;
          case 0xFF42BD:
          //lcd.print("7");
          calcula_tempo(7);
          break ;
          case 0xFF4AB5:
          //lcd.print("8");
          calcula_tempo(8);
          break ;
          case 0xFF52AD:
          //lcd.print("9");
          calcula_tempo(9);
          break ;                   
          }
        key_value = results.value;
        irrecv.resume(); 
      }


      if (relay == true)
        {
        acende_uvc();  
        }
      
      delay(100);
      
  }  

void calcula_tempo(int nuentrada)
  {
  char strdig1[5], strdig2[5], strfull[5];
   
  if ((t1 == 3 and t2 > 0) or (t_total != 0))
    {
    t1 = 0;
    t2 = 0;
    t_total = 0;
    lcd_display(ft);
    delay(t_delay);
    lcd_display(covok);
    return;
    }

  if (t1 != 0 and t2 == 0)
    {
    t2 = nuentrada;
    
    itoa(t1, strdig1, 10);
    itoa(t2, strdig2, 10);
    strcpy(strfull, strdig1);
    strncat(strfull, strdig2, 2);
    t_total = atoi(strfull);
    
    number_lcd(t_total);
    }
  else
    {
    t1 = nuentrada; 
    number_lcd(t1);
    }
  
  }

void reset(void)
  {
  t1 = 0;
  t2 = 0;
  t_total = 0;
  seg = 0;
  digitalWrite(pinorelay, LOW);
  relay = false;
  lcd_display(covok);
  }

void contagem_regressiva(int nmax)
  {
  int k;
  
  for (k=nmax;k>=1;k--)
    {
    number_lcd(k);
    delay(t_delay);
    }
  }
  
void acende_uvc(void)
  {
  int i;
  
  if (t_total == 0 and relay == true)
    {
    reset();
    lcd_display(ff);
    delay(t_delay * 10);
    return;
    }

  if (relay == false and t_total > 0)
    {
    contagem_regressiva(30);  
    t_total += 1;
    digitalWrite(pinorelay, HIGH);
    relay = true;
    }

  if (relay == true and seg == 0 and t_total > 0)
    {
    t_total -= 1;
    seg = 50;
    }

  anima_covid();
  
  number_lcd(t_total);

  for (i=5;i>=3;i--)
    {
    lc.setIntensity(0,i);
    delay(t_delay);
    }
    
  for (i=3;i<=5;i++)
    {
    lc.setIntensity(0,i);
    delay(t_delay);
    }

  seg -= 10;

  }

void anima_covid(void)
  {
    lcd_display(covolho);
    delay(t_delay);
    lcd_display(covx);
    delay(t_delay);
    lcd_display(covx2);
    delay(t_delay);
    lcd_display(covx3);
    delay(t_delay);
    lcd_display(covx4);
    delay(t_delay);
    lc.clearDisplay(0);
  }
  
void lcd_display(byte led[])
  {
  int j;
  
  for (j=0;j<=7;j++)
    {
    lc.setRow(0,j,led[j]);
    }
  }

void number_lcd(int number)
  {
    if (number == 30)
      {
        lcd_display(trinta);
        return;
      }
    
    if (number == 29)
      {
        lcd_display(vintenove);
        return;
      }
    
    if (number == 28)
      {
        lcd_display(vinteoito);
        return;
      }
    
    if (number == 27)
      {
        lcd_display(vintesete);
        return;
      }
    
    if (number == 26)
      {
        lcd_display(vinteseis);
        return;
      }
    
    if (number == 25)
      {
        lcd_display(vintecinco);
        return;
      }
    
    if (number == 24)
      {
        lcd_display(vintequatro);
        return;
      }
    
    if (number == 23)
      {
        lcd_display(vintetres);
        return;
      }
    
    if (number == 22)
      {
        lcd_display(vintedois);
        return;
      }
    
    if (number == 21)
      {
        lcd_display(vinteum);
        return;
      }
    
    if (number == 20)
      {
        lcd_display(vinte);
        return;
      }
    
    if (number == 19)
      {
        lcd_display(dezenove);
        return;
      }
    
    if (number == 18)
      {
        lcd_display(dezoito);
        return;
      }
    
    if (number == 17)
      {
        lcd_display(dezessete);
        return;
      }
    
    if (number == 16)
      {
        lcd_display(dezesseis);
        return;
      }
    
    if (number == 15)
      {
        lcd_display(quinze);
        return;
      }


    if (number == 14)
      {
        lcd_display(quatorze);
        return;
      }


    if (number == 13)
      {
        lcd_display(treze);
        return;
      }


    if (number == 12)
      {
        lcd_display(doze);
        return;
      }


    if (number == 11)
      {
        lcd_display(onze);
        return;
      }


    if (number == 10)
      {
        lcd_display(dez);
        return;
      }


    if (number == 9)
      {
        lcd_display(nove);
        return;
      }


    if (number == 8)
      {
        lcd_display(oito);
        return;
      }


    if (number == 7)
      {
        lcd_display(sete);
        return;
      }


    if (number == 6)
      {
        lcd_display(seis);
        return;
      }

 
    if (number == 5)
      {
        lcd_display(cinco);
        return;
      }


    if (number == 4)
      {
        lcd_display(quatro);
        return;
      }


    if (number == 3)
      {
        lcd_display(tres);
        return;
      }


    if (number == 2)
      {
        lcd_display(dois);
        return;
      }


    if (number == 1)
      {
        lcd_display(um);
        return;
      }
  }

 
