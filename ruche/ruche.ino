//bibliotèques pour les sondes de température
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <dht11.h>

//bibliotèque pour l'écran LCD
#include <LiquidCrystal.h>

dht11 DHT11; // Création du capteur humidité+température
#define DHT11PIN 7 // Définition de la pin utilisée

#define ONE_WIRE_BUS_1 9
OneWire oneWire_1(ONE_WIRE_BUS_1);
DallasTemperature sensors_1(&oneWire_1);
DeviceAddress insideThermometer = { 0x28, 0x31, 0x9D, 0xD3, 0x5, 0x0, 0x0, 0x23 }; //Cette adresse correspond au "nom" du capteur

#define ONE_WIRE_BUS_2 8
OneWire oneWire_2(ONE_WIRE_BUS_2);
DallasTemperature sensors_2(&oneWire_2);
DeviceAddress outsideThermometer = { 0x28, 0x38, 0x4C, 0xD2, 0x5, 0x0, 0x0, 0xA9 };//Cette adresse correspond au "nom" du capteur

LiquidCrystal lcd(11, 10, 6, 5, 4, 3);

const int detecteurMouvementPin = 2;

void setup() {

  pinMode(detecteurMouvementPin, INPUT);
  digitalWrite(detecteurMouvementPin, LOW);

  sensors_1.begin();
  sensors_2.begin();
	//Paramétrage de la précision de température (en bit) : 9 bit (0.5°C), 10 bit (0.25°C), 11 bit (0.125°C), 12 bit (0.0625°C),
  sensors_1.setResolution(insideThermometer, 10);
  sensors_2.setResolution(outsideThermometer, 10);
  
  
  lcd.begin(16, 2);
  lcd.print("  Bonjour les ");
  lcd.setCursor(0, 1);
  lcd.print("    Abeilles!   ");

  delay(2000);

  attachInterrupt(0, presence, HIGH); //Cette interruption permet d'activer le détecteur de mouvement en priorité par rapport aux autres sondes.
}

void loop()
{

  int chk = DHT11.read(DHT11PIN); // Lecture du capteur
  lcd.clear();
  lcd.print("Humid (%): ");
  lcd.println((float)DHT11.humidity, 2); //On affiche l'humidité à l'intérieur de la ruche
	
	delay(3000);//Pause avant la prochaine lecture
   
	sensors_1.requestTemperatures();
  float int1= (float)DHT11.temperature;
  float tempC = sensors_1.getTempC(insideThermometer);
  float result = (tempC + int1)/2; //On fait une moyenne des deux sondes intérieurs afin d'avoir un résultat plus homogène.
 	
	lcd.clear();
  lcd.print("Temp int");
  lcd.setCursor(0, 1);
  lcd.print("(C):");
  lcd.print(result); //Affichage du résultat

  delay(3000); //Pause avant la prochaine lecture     
  lcd.clear();
  lcd.print("Temp ext");
  lcd.setCursor(0, 1);
  lcd.print("(C) :");
  printTemperature(sensors_2,outsideThermometer); //Affichage du résultat
  
	delay(3000); 
}

/*
* Cette fonction affiche un message de détection de présence sur l'écran LCD
*/
void presence() {
	lcd.clear();
  lcd.setCursor(0, 1);
  lcd.println("detection          ");
}

/*
* Cette fonction affiche la température récupéré sur la sonde et l'affiche sur l'écran LCD
*/
void printTemperature(DallasTemperature sensor, DeviceAddress deviceAddress)
{
  sensor.requestTemperatures();
  float tempC = sensor.getTempC(deviceAddress);
  if (tempC == -127.00) {
    lcd.print("Impossible de récuperer la température");
  } else {
    lcd.print(tempC);
  }
}

