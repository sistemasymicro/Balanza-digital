/* Arduino pin 
 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND */
 
#include "HX711.h"
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);

byte cuenta = 0;

float calibration_factor = 10500  ; //-7050 worked for my 440lb max scale setup

float peso_gramos = 0.0;
float peso_libras = 0.0;
float peso_kilos=0.0;

float precio_libras = 0.00;
float precio;
float precio_total=0.00;

char precio_producto[5]; //
String precio_productoS = String(); //Almacena las 4 cifras del jugador

String v1 = String();  //primera cifra del número jugador
String v2 = String(); //segunda cifra del número jugador
String v3 = String(); //tercera cifra del número jugador
String v4 = String(); //cuarta cifra del número jugador
String v5 = String(); //cuarta cifra del número jugador

char tecla;

const byte ROWS=4;
const byte COLS=4;
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};

byte rowPins[ROWS] = {4,5,6,7}; //Filas(pines del 4 al 7)
byte colPins[COLS] = {8,9,10,11}; //Columnas (pines del 8 al 11)
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Crear el objeto lcd  dirección  0x27 y 20 columnas x 4 filas
LiquidCrystal_I2C lcd(0x27,20,4);  //

void setup() {

  // Inicializar el LCD 
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();

  scale.set_scale(calibration_factor);  //  El obtenido en la calibracion

  scale.tare(); //Reset the scale to 0

}


void loop() {

  tecla = keypad.getKey();

   v1=String(precio_producto[0]);
   v2=String(precio_producto[1]);
   v3=String(precio_producto[2]);
   v4=String(precio_producto[3]);
   v5=String(precio_producto[4]);

   peso_libras= abs(((int) (scale.get_units()*100))/100.0);
   peso_kilos= abs(peso_libras/2);
   peso_gramos = abs(peso_libras*500);
   
   precio_productoS = v1+v2+v3+v4+v5;
   
   precio = precio_productoS.toFloat();

    //  Visualiza en peso en gramos
    lcd.setCursor(0, 0);
    lcd.print("Peso gramos:");
    lcd.print(peso_gramos,1);   

    //  Visualiza en peso en libras y kilos
    lcd.setCursor(0, 1);
    lcd.print("Lbs:");
    lcd.print(peso_libras,1);     

    lcd.setCursor(12, 1);
    lcd.print("Kls:");
    lcd.print(peso_kilos,1);     

    precio_total = peso_kilos*precio;
    
    lcd.setCursor(0, 3);
    lcd.print("Precio ttl: $");
    lcd.print(precio_total,1);  

    if(tecla=='A'){
        scale.tare(); //Reset the scale to 0
    }
   
    
    if(tecla=='C')  {
    lcd.clear();
    precio_producto[0] = ' ';
    precio_producto[1] = ' ';
    precio_producto[2] = ' ';
    precio_producto[3] = ' ';
    precio_producto[4] = ' ';    
    precio = 0.00;
    precio_productoS = '0'; 
    cuenta=0;  
    precio_total = 0.00;     
    lcd.setCursor(0, 2);
    lcd.print("Precio kilo: $ ");     
    lcd.setCursor(14+cuenta,2); 
    lcd.print(precio_producto[cuenta]);
    
  }

     if(tecla){
            
     if(tecla!='A' && tecla!='B' && tecla!='C' && tecla!='D')
     {
      precio_producto[cuenta] = tecla;
      //  Visualiza el precio del producto            
      lcd.setCursor(14+cuenta,2); 
      lcd.print(precio_producto[cuenta]);
      cuenta++;       

     }

   delay(100);
        
    }     
  
}
