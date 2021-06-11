import processing.serial.*;

int port = 0;      // If you get a grey screen check console and change to your required port

int lf = 10;  
String myString = null;
Serial myPort; 

int p1x = 1023;
int p1y = 1023;
int p2x = 1023;
int p2y = 1023;
int p3x = 1023;
int p3y = 1023;
int p4x = 1023;
int p4y = 1023;
int mx = 1023;
int my = 1023;
int dx = 1023;
int dy = 1023;

color p1color = color( 102, 255, 102 ); // TL
color p2color = color( 51, 204, 51 ); // TR
color p3color = color( 0, 153, 255 ); // BL
color p4color = color( 0, 102, 255 ); // BR
color p5color = color( 89, 89, 89 ); // MEDIAN
color p6color = color( 255, 0, 0 ); // FINAL
 
void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[port], 9600);
  delay (100);
  myPort.clear();

  myString = myPort.readStringUntil('\n');
  myString = null;
  myPort.bufferUntil('\n');
  
  size(1223,968);
  frameRate(30);
  
}

void draw() {
  // preparation stage
  while( myPort.available() > 0 ) {
     myString = myPort.readStringUntil('\n');
     if( myString != null) {
       convertmyStringToCoordinates();
  
      // drawing stage   
      background(10, 10, 10); // repaint the whole drawing area with dark grey color (77,77,77), making the whole window clear and clean
      // immediately draw the circles after clearing, we've done the time-consuming preparation beforehand in convertmyStringToCoordinates() so this will give us minimal lag ( hopefully no flickering ).
      fill(77, 77, 77);
      rect(100, 100, 1023, 768);
  
      line(p1x, p1y, p2x, p2y);
      line(p1x, p1y, p3x, p3y);
      line(p4x, p4y, p3x, p3y);
      line(p4x, p4y, p2x, p2y);

      drawCircle( p1x, p1y, p1color );
      drawCircle( p2x, p2y, p2color );
      drawCircle( p3x, p3y, p3color );
      drawCircle( p4x, p4y, p4color );
      drawCircle( dx, dy, p5color );
      drawCircle( mx, my, p6color );

      }  
   }
}

    
void drawCircle( int xval, int yval, color c ){ 
  if( xval != 1023 && yval != 1023 ){ 
    ellipseMode(RADIUS);  
    fill( c );  
    ellipse(xval, yval, 15, 15);
  }
}

 
void convertmyStringToCoordinates() {
  
   try {
  
  println(myString); 
  int[] output = int (split(trim(myString), ',')); 
 
  p1x = output[0];
  p1y = output[1];  
  p2x = output[2];
  p2y = output[3];   
  p3x = output[4];
  p3y = output[5];  
  p4x = output[6];
  p4y = output[7];
  
  mx = output[8];
  my = output[9];
  
  dx = output[10];
  dy = output[11];
  
}
  
  catch(ArrayIndexOutOfBoundsException exception) {
    println("612,484,612,484,612,484,612,484,612,484,612,484,"); 
}
 
      
}
