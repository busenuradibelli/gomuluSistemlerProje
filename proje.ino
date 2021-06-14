//=======================================================================================================//|
//                                                                                                       //|
//                          ONDOKUZ MAYIS ÜNİVERSİTESİ SAMSUN MESLEK YÜKSEKOKULU                         //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                  BİLGİSAYAR PROGRAMCILIĞI PROGRAMI                                    //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                               GÖMÜLÜ SİSTEMLER DERSİ FİNAL PROJE ÖDEVİ                                //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                  PROJE ADI: AKVARYUM KONTROL SİSTEMİ                                  //|
//                                                                                                       //|
//=======================================================================================================//|
//    HAZIRLAYAN: BUSENUR ADIBELLİ                                                                       //|
//    ÖĞRENCİ NO: 20480098                                                                               //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                               15/06/2021                                              //|
//                                                                                                       //|
//=======================================================================================================//|

//-------------------------------------------KÜTÜPHANELER-------------------------------------------------//
#include <Servo.h> // Servo kütüphane dosyası çağırıldı.
#include <OneWire.h> // OneWire kütüphane dosyası çağrıldı
#include <DallasTemperature.h>// // DallasTemperature kütüphane dosyası çağrıldı
#include <Wire.h> // wire kütüphanesi çağrıldı
#include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C kütüphanesi çağrıldı
#include <virtuabotixRTC.h> // RTC kütüphane dosyası çağırıldı

//---------------------------------------------SERVO--------------------------------------------------------//
Servo motor;  // servoyu tanımladım.
 int pos = 0;   

 
//--------------------------------------------LEDLER VE BUTON-----------------------------------------------//


#define yled 3  //Yeşil led
#define mled 2 //Mavi led
#define sled 4 //şerit led


//---------------------------------------------------RTC-------------------------------------------------------//

int CLK_PIN = 6;                                        //6. pini clock pini olarak tanımladım.
int DAT_PIN = 7;                                        //7. pini data pini olarak tanımladım.
int RST_PIN = 8;                                        //8. pini reset pini olarak tanımladım.
virtuabotixRTC myRTC(CLK_PIN, DAT_PIN, RST_PIN);        // Kütüphaneyi pinlere atadım.


//------------------------------------------------------LCD---------------------------------------------------//
int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;   //LCD'nin pin değişkenlerini tanımladım.

LiquidCrystal_I2C lcd(0x27,16,2); // 0x27 adresli 16x2 lcd'yi tanımladım.

//---------------------------------------------------SENSÖRLER------------------------------------------------//

#define ONE_WIRE_BUS 13 // ds18b20 dijital ısı sensörünü 13. pine atadım.
//Herhangi bir OneWire cihazıyla iletişim kurmak için.
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

//------------------------------------------------START-------------------------------------------------------//
void setup() 
  {  
 Serial.begin(9600); // Seri haberleşmeyi başlattım.
  motor.attach(10);// 10. pine servo bağlandı.

  pinMode(A0, INPUT);
  pinMode(yled, OUTPUT); // yeşil led çıkış olarak tanımlandı.
  pinMode(mled, OUTPUT); // mavi led çıkış olarak tanımlandı.
  pinMode(sled, OUTPUT); // şerit led çıkış olarak tanımlandı.
  pinMode(buton, INPUT); // buton 1 giriş olarak tanımlandı.

  lcd.begin(); // lcd başlatıldı.
  lcd.backlight(); // lcd arka ışığı 
  sensors.begin(); //sensörler başlatıldı

   lcd.setCursor(0, 0);
   lcd.print("Proje Sahibi:");
   lcd.setCursor(0, 1);
   lcd.print("Busenur Adibelli");
   delay(2000);
   lcd.clear();
   
   lcd.setCursor(0, 0);
   lcd.print("Ogrenci No:");
   lcd.setCursor(0, 1);
   lcd.print("20480098");
   delay(2000);
   lcd.clear();
}


void loop() 
{
  digitalWrite(yled , HIGH); // sisteme güç verildiğinde yeşil led yanar.

  //IŞIKLANDIRMA
  int isik = analogRead(A0); //Işık değişkenini A0 pinindeki LDR ile okuttum.
  Serial.println(isik); //Okunan değeri seri iletişim ekranına yansıttım.
  delay(50);

  if (isik > 450) { //Okunan ışık değeri 450'den büyük ise
    digitalWrite(sled, HIGH); //LED yansın
  }

  if (isik < 350) { //Okunan ışık değeri 350'den küçük ise
    digitalWrite(sled, LOW); //LED yanmasın
  }

  
   //SICAKLIK
  sensors.requestTemperatures(); //Bu fonksiyon ile sensöre sıcaklık okumak istediğimize ait komutu yollarız ve sensör sıcaklığı ölçer ve okunması için hazır da bekletir. Her sıcaklık okunmadan önce bu fonksiyon kullanılmalıdır.
  lcd.setCursor(9,1);
  lcd.print(  sensors.getTempCByIndex(0)); //
   lcd.print((char)223);
   lcd.print("C");

  //RTC
   myRTC.updateTime();                                   //RTC'den zamanı okuyoruz
  lcd.setCursor(4,0);                                   //Ekranın ilk satır ilk sütunundan yazmaya başlıyoruz.
  lcd.print(myRTC.dayofmonth);                          //Günü ekrana bastırıyoruz.
  lcd.print("/");
  lcd.print(myRTC.month);                               //Ayı ekrana bastırıyoruz.
  lcd.print("/");
  lcd.print(myRTC.year);                                //Yılı ekrana bastırıyoruz
  lcd.setCursor(0,1);                                  //LCD'nin 2. satır 1. sütunundan yazmaya başlıyoruz.
  lcd.print(myRTC.hours);                               //Saati ekrana bastırıyoruz.
  lcd.print(":");
  lcd.print(myRTC.minutes);                             //Dakikayi ekrana bastırıyoruz.   
  lcd.print(":");
  lcd.print(myRTC.seconds);                             //Saniyeyi ekrana bastırıyoruz.


   //SERVO
 if (/*myRTC.hours==20  && myRTC.minutes==37&&*/ myRTC.seconds==30) // Yemleme saatini rtc modül ile belirledim. 
      { //normalde günde 1 defa yemlemek için tasarlandı fakat proje sunumu için dakikada 1 yemleme motoru çalışacak.
      
     lcd.clear();
     lcd.print("Yemleniyor");
     digitalWrite(mled , HIGH); //yemlenirken mavi led yanar.
     delay(3000);
     
      motor.write(pos);              // Servo açı değeri olarak belirlediğimiz pos değişkenini servoya yazdırdım.
    delay(1);                       // servonun hedeflenen açıya gidebilmesi için 1 ms bekleme ekledim.
  for (pos = 0; pos <= 180; pos += 1) { // For döngüsü ile 0 ile 180 derece arası gitmesini sağladım.
                                        // her bir adımda 1 derece artacak şekilde ayarladım.
    motor.write(pos);              // Servo açı değeri olarak belirlediğimiz pos değişkenini servoya yazdırdık.
    delay(1);                       // servonun hedeflenen açıya gidebilmesi için 1 ms bekleme ekledim.
  } 
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Yemleme");
  lcd.setCursor(0,1);
  lcd.print("Tamamlandi.");
   digitalWrite(mled , LOW); //yemleme tamamlandığında mavi led söner.
  delay(2000);
  lcd.clear();
        }  

}
  
 
 
  
  
