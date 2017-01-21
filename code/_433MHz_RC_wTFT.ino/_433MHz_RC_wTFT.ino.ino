#include <SPI.h>
#include <TFT.h>
//#include "comlib.h"

#define cs   10
#define dc   9
#define rst  8  

//TFT TFTscreen = TFT(cs, dc, rst);


class button{
  private:
    static char buffer[50];
    String name;
    button* next=NULL;
    button* last=NULL;
    button* up=NULL;
    
    static void drawBasics(const uint8_t pos);
    static void setBackgroundColour();
    static void setButtonColours();
    static void setActiveColour();
    static void setStroke2ButtonFillColours();
    static void setStroke2TextColours();
    static void buttonText(const String text, const uint8_t pos, const bool big);

  public:
    //static TFT TFTscreen;
    button(int _type);
    virtual button* push();
    virtual void longPush();
    virtual void left(const bool fast);
    virtual void right(const bool fast);
    virtual void draw(const uint8_t pos, const bool active);
    void setNext(const button*);
    static void DeActivate(const uint8_t pos, const bool active);
};


class pushbutton: button{
  private:
    char* message="";
    void send();
    
  public:
    pushbutton(const String name_, const char* message_);
    button* push();
    void left(const bool fast);
    void right(const bool fast);
    void draw(const uint8_t pos, const bool active);
};

class linselectbutton: button{
  private:
    char* message="";
    void send();
    uint8_t maxVal;
    uint8_t minVal=0;
    uint8_t curVal=0;
    uint8_t pos;
    const uint8_t sliderBarWidth=99;
    void setSliderColours();
    void setSliderBarColours();
    
  public:
    linselectbutton(const String name_, const char* message_, const uint8_t maximum);
    button* push();
    void left(const bool fast);
    void right(const bool fast);
    void draw(const uint8_t pos, const bool active);
};

class arrayselectbutton: button{
  private:
    char* message="";
    void send();
    uint8_t arrayLength;
    String *values;
    uint8_t curPos=0;
    uint8_t pos;
    
  public:
    arrayselectbutton(const String name_, const char* message_, String* vals, uint8_t nvals);
    button* push();
    void left(const bool fast);
    void right(const bool fast);
    void draw(const uint8_t pos, const bool active);
};


class submenubutton: button{
  private:
    button* down;

  public:
    submenubutton(const String name_, const button* down_);
    button* push();
    void left(const bool fast);
    void right(const bool fast);
    void draw(const uint8_t pos, const bool active);
};

class menu{
  private:
    button* currentButton;
    uint8_t curButtonPos=1;
    void draw();
    
  public:
    menu();
    void left(const bool fast);
    void right(const bool fast);
    void down();
    void up();
    void pushLong();
    void pushShort();
};


int a = 0;
// char array to print to the screen
char sensorPrintout[4];
TFT TFTscreen = TFT(cs, dc, rst);
//TFT TFTscreen=button::TFTscreen;

void setup() {

}


void loop() {

  String sensorVal = String(a++);

  sensorVal.toCharArray(sensorPrintout, 4);

/*
 * determine with millis how long a button has been pressed if >250ms do something, if stick is on the far left do more
 * also use millis for sleep timer on the lamps (unsigned long) one counter plus states on and off and other counter overwrite/reset
 */

//basic shape
  TFTscreen.stroke(128,128,128);
  TFTscreen.fill(64,64,64);
  TFTscreen.rect(2, 20, 124, 45);  
  TFTscreen.rect(2, 67, 124, 45);
  TFTscreen.rect(2, 114, 124, 45);


//slider
  //text (has to be centered
  TFTscreen.stroke(0,0,0);
  TFTscreen.setTextSize(1);
  TFTscreen.text("Test", 45, 24);//15 High continue 16



//selector
  //text (has to be centered
  TFTscreen.stroke(0,0,0);
  TFTscreen.setTextSize(1);
  TFTscreen.text("Test2", 45, 71);//15 High continue 16
  TFTscreen.stroke(0,0,128);
  TFTscreen.fill(64,64,64);
  TFTscreen.rect(40, 87, 40, 20);
  TFTscreen.stroke(0,0,0);
  TFTscreen.setTextSize(1);
  TFTscreen.text("val1", 8, 94);//15 High continue 16
  TFTscreen.text("val2", 43, 94);//15 High continue 16
  TFTscreen.text("val3", 90, 94);//15 High continue 16


//Submenu/pushbutton
  TFTscreen.stroke(0,0,0);
  TFTscreen.setTextSize(3);
  TFTscreen.text("On/Off", 4, 118);//15 High continue 16

  //TFTscreen.circle(100,100,4);
  //TFTscreen.text(sensorPrintout, 0, 20);
  button::DeActivate(1,true);
  delay(1000);
  //TFTscreen.stroke(0,0,255);
  //TFTscreen.text(sensorPrintout, 0, 20);

  //TFTscreen.stroke(255,0,0); // B, G,R
  //TFTscreen.point(5,159);//x(lang), y(kurz)

  //TFTscreen.stroke(255,255,0); // B,G,R
  //int xPos = TFTscreen.width()/2;
  //int yPos = TFTscreen.height()/2;

  //TFTscreen.line(xPos, TFTscreen.height() - yPos, xPos, TFTscreen.height()); // vertical
  //TFTscreen.line(xPos-4, TFTscreen.height() - yPos+4, xPos-2, TFTscreen.height()); // angled


  //TFTscreen.noStroke(); // don't draw a line around the next rectangle
  //TFTscreen.fill(0,255,0); // set the fill color to green
  
//20 -45
}




static void button::drawBasics(const uint8_t pos){  TFTscreen.begin();
  TFTscreen.setRotation(0);
  TFTscreen.background(0, 0, 0);

  TFTscreen.stroke(0,255,255);
  TFTscreen.setTextSize(2);
  TFTscreen.text("Dustins RC: ",0,0);//15 High continue 16
  TFTscreen.setTextSize(5);
  button::setButtonColours();
  if(pos==3){
    for(int i=0;i<3;i++) {
      TFTscreen.rect(2, 20+i*47, 124, 45);
    }
    return;
  }
  TFTscreen.rect(2, 20+pos*47, 124, 45);
}

static void button::setBackgroundColour(){
  TFTscreen.background(0, 0, 0);
}
static void button::setButtonColours(){
  TFTscreen.stroke(128,128,128);
  TFTscreen.fill(64,64,64);
}
static void button::setActiveColour(){
  TFTscreen.stroke(0,0,255);
}
static void button::setStroke2ButtonFillColours(){
  TFTscreen.stroke(64,64,64);
}    
static void button::setStroke2TextColours(){
  TFTscreen.stroke(0,0,0);
}
static void button::DeActivate(const uint8_t pos, const bool active){
  if(active){
    button::setActiveColour();   
  }else{
    button::setButtonColours();
  }
  TFTscreen.line(2, 20+47*pos, 125, 20+47*pos);
  TFTscreen.line(125, 20+47*pos, 125, 65+47*pos); 
  TFTscreen.line(126, 64+47*pos, 2, 64+47*pos);
  TFTscreen.line(2, 64+47*pos, 2, 20+47*pos);
}


static void button::buttonText(const String text,const uint8_t pos,const bool big){
  button::setStroke2TextColours();
  text.toCharArray(buffer, 50);
  const uint8_t txtlength=text.length();
  if(big){
    uint8_t txtsize=1;
    if(txtlength<7){
      txtsize=3;
    }else if(txtlength<10){
      txtsize=2;
    }
    TFTscreen.text(buffer, 64-6*txtsize*txtlength/2 , 36+pos*47);//15 High continue 16
  }else{
    TFTscreen.setTextSize(1);
    TFTscreen.text(buffer, 64-6*txtlength/2, 24+pos*47);//15 High continue 16
  }
}

void button::setNext(const button* next_){
  self.next=next_;
  next_->last=&self;
}


void pushbutton::send(){
  //send self.message
}
    
pushbutton::pushbutton(const String name_, const char* message_){
  self.message=message_;
  self.name=name_;
}

button* pushbutton::push(){
  self.send();
  return self;
}

void pushbutton::left(const bool fast){
  return;
}
    
void pushbutton::right(bool fast){
  return;
}

void pushbutton::draw(const uint8_t pos, const bool active){
  if(active){
    self.DeActivate(pos,active);
  }
  button::buttonText(self.name, pos, true);
}




linselectbutton::linselectbutton(const String name_, const char* message_, const uint8_t maximum){
  self.message=message_;
  self.name=name_;
  self.maxVal=maximum;
}

void linselectbutton::send(){
  uint8_t offset=0;
  uint8_t i=0;
  for(i=0;self.message[i]!='\0';i++){
    button::buffer[i+offset]=self.message[i];
    if(self.message[i]=='%'){
      String Value = String(self.curVal);
      Value.toCharArray(&(buffer[i]), 5);
      while(offset<5){
        if(buffer[i+offset]=='\0'){
          break;
        }
        offset++;
      }
    }
  }
  //send buffer with length i+offset
}

button* linselectbutton::push(){
  self.send();
}

void linselectbutton::setSliderColours(){
  TFTscreen.noStroke();
  TFTscreen.fill(0,0,0);
}

void linselectbutton::setSliderBarColours(){
  TFTscreen.stroke(16,16,16);
  TFTscreen.fill(192,192,192);
}

void linselectbutton::draw(const uint8_t pos,bool active){
  if(active){
    self.DeActivate(pos,active);
  }
  button::buttonText(self.name, pos, false);
  self.pos=pos;
  uint8_t sliderPos=10 + self.sliderBarWidth * (self.curVal-self.minVal)/(double) (self.maxVal-self.minVal);
  self.setSliderBarColours()
  TFTscreen.rect(10, 47+pos*47, self.sliderBarWidth, 5);
  self.setSliderColours()
  TFTscreen.circle(sliderPos ,49+pos*47, 4);
}

void linselectbutton::left(const bool fast){
  uint8_t sliderPos=10 + self.sliderBarWidth * (self.curVal-self.minVal)/(double) (self.maxVal-self.minVal);
  button::setButtonColours();
  TFTscreen.noStroke();
  TFTscreen.circle(sliderPos ,49+self.pos*47, 4);
  self.setSliderBarColours()
  TFTscreen.rect(10, 47+self.pos*47, self.sliderBarWidth, 5);
  if(fast){
    uint8_t offset=self.maxVal-self.minVal/10;
    if(self.curVal-self.minVal>offset){
      self.curVal=-offset;
    else{
      self.curVal=self.minVal;
    }
  }else{
    self.curVal--;
  }
  sliderPos=10 + self.sliderBarWidth * (self.curVal-self.minVal)/(double) (self.maxVal-self.minVal);
  self.setSliderColours()
  TFTscreen.circle(sliderPos ,49+self.pos*47, 4);
}

void linselectbutton::right(const bool fast){
  uint8_t sliderPos=10 + self.sliderBarWidth * (self.curVal-self.minVal)/(double) (self.maxVal-self.minVal);
  button::setButtonColours();
  TFTscreen.noStroke();
  TFTscreen.circle(sliderPos ,49+self.pos*47, 4);
  self.setSliderBarColours()
  TFTscreen.rect(10, 47+self.pos*47, self.sliderBarWidth, 5);
  if(fast){
    uint8_t offset=self.maxVal-self.minVal/10;
    if(self.curVal+offset<self.maxVal){
      self.curVal=+offset;
    else{
      self.curVal=self.maxVal;
    }
  }else{
    self.curVal++;
  }
  sliderPos=10 + self.sliderBarWidth * (self.curVal-self.minVal)/(double) (self.maxVal-self.minVal);
  self.setSliderColours()
  TFTscreen.circle(sliderPos ,49+self.pos*47, 4);
}


void arrayselectbutton::send(){
  uint8_t offset=0;
  uint8_t i=0;
  for(i=0;self.message[i]!='\0';i++){
    button::buffer[i+offset]=self.message[i];
    if(self.message[i]=='%'){
      String Value = String(self.curVal);
      self.values[self.curPos].toCharArray(&(self.buffer[i]), 6);
      while(offset<6){
        if(self.buffer[i+offset]=='\0'){
          break;
        }
        offset++;
      }
    }
  }
  //send buffer with length i+offset
}
    

arrayselectbutton::arrayselectbutton(const char* name_, const char* message_, const String* vals, const uint8_t nvals){
  self.arrayLength=nvals;
  self.values=vals;
  self.message=message_;
  self.name=name_;
}

button* arrayselectbutton::push(){
  self.send();
}

void arrayselectbutton::draw(const uint8_t pos, bool active){
  if(active){
    self.DeActivate(pos,active);
  }
  button::buttonText(self.name, pos, false);
  self.pos=pos;
  button::setButtonColours();
  TFTscreen.stroke(0,0,128);
  TFTscreen.rect(40, 87 +self.pos*47, 40, 20);
  button::setStroke2TextColours();
  TFTscreen.setTextSize(1);
  self.values[self.curPos].toCharArray(self.buffer, 6);
  TFTscreen.text(buffer, 43, 47+self.pos*47);//15 High continue 16
  if(self.curPos>0){
    self.values[self.curPos-1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 8, 47+self.pos*47);//15 High continue 16
  }
  if(self.arrayLength>self.curPos+1){
    self.values[self.curPos+1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 90, 47+self.pos*47);//15 High continue 16
  }
}

void arrayselectbutton::left(const bool fast){
  button::setStroke2ButtonFillColours();
  TFTscreen.setTextSize(1);
  self.values[self.curPos].toCharArray(self.buffer, 6);
  TFTscreen.text(buffer, 43, 47+self.pos*47);//15 High continue 16
  if(self.arrayLength>self.curPos+1){
    self.values[self.curPos+1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 90, 47+self.pos*47);//15 High continue 16
  }
  if(self.curPos>0){
    self.values[self.curPos-1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 8, 47+self.pos*47);//15 High continue 16
    self.curPos--;
  }
  if(fast){
    self.curPos=0;
  }
  button::setStroke2TextColours();
  TFTscreen.setTextSize(1);
  self.values[self.curPos].toCharArray(self.buffer, 6);
  TFTscreen.text(buffer, 43, 47+self.pos*47);//15 High continue 16
  if(self.curPos>0){
    self.values[self.curPos-1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 8, 47+self.pos*47);//15 High continue 16
  }
  if(self.arrayLength>self.curPos+1){
    self.values[self.curPos+1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 90, 47+self.pos*47);//15 High continue 16
  }
}

void arrayselectbutton::right(const bool fast){
  button::setStroke2ButtonFillColours();
  TFTscreen.setTextSize(1);
  self.values[self.curPos].toCharArray(self.buffer, 6);
  TFTscreen.text(buffer, 43, 47+self.pos*47);//15 High continue 16
  if(self.curPos>0){
    self.values[self.curPos-1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 8, 47+self.pos*47);//15 High continue 16
  }
  if(self.arrayLength>self.curPos+1){
    self.values[self.curPos+1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 90, 47+self.pos*47);//15 High continue 16
    self.curPos++;
  }
  if(fast){
    self.curPos=self.arrayLength-1;
  }
  button::setStroke2TextColours();
  TFTscreen.setTextSize(1);
  self.values[self.curPos].toCharArray(self.buffer, 6);
  TFTscreen.text(self.buffer, 43, 47+self.pos*47);//15 High continue 16
  if(self.curPos>0){
    self.values[self.curPos-1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 8, 47+self.pos*47);//15 High continue 16
  }
  if(self.arrayLength>self.curPos+1){
    self.values[self.curPos+1].toCharArray(self.buffer, 6);
    TFTscreen.text(self.buffer, 90, 47+self.pos*47);//15 High continue 16
  }
}



submenubutton::submenubutton(const char* name_, const button* down_){
  self.name=name_;
  self.down=down_;
  self.down->up=&self;
  button* next=self.down.next;
  while(self.down != next){
    next->up=&self;
    next=self.down.next;
  }
}
    
button* submenubutton::push(){
  return self.down;
}
    
void submenubutton::draw(const uint8_t pos,bool active){
  if(active){
    self.DeActivate(pos,active);
  }
  button::buttonText(self.name, pos, true);
}
    
void submenubutton::left(const bool fast){
  return;
}
    
void submenubutton::right(const bool fast){
  return;
}



menu::menu(){

  TFTscreen.begin();
  TFTscreen.setRotation(0);
  TFTscreen.background(0, 0, 0);

  TFTscreen.stroke(0,255,255);
  TFTscreen.setTextSize(2);
  TFTscreen.text("Dustins RC: ",0,0);

  //bedroomLamp
  static pushbutton bedroomlampoff("Off","");  
  static pushbutton bedroomlampon("On","");
  bedroomlampoff.setNext(&bedroomlampon);
  static pushbutton bedroomlampmusicon("MOn","");
  bedroomlampon.setNext(&bedroomlampmusicon);
  static pushbutton bedroomlampmusicff("MOff","");
  bedroomlampmusicon.setNext(&bedroomlampmusicoff);
  bedroomlampmusicoff.setNext(&bedroomlampoff); //add sleep light, wakeup light, timer light, same for music, music volume, different/all colours "volume"
  
  //CupboardLEDs
  static pushbutton cupboardlampoff("Off","");  
  static pushbutton cupboardlampon("On","");
  cupboardlampoff.setNext(&cupboardlampon);
  static linselectbutton cupboardoverallbrightness("Brightness", "Brightness%", 100);
  cupboardlampon.setNext(&cupboardoverallbrightness);
  cupboardoverallbrightness.setNext(&cupboardlampoff);
  //on, off, volume(r,g,b,all), sleep, sleeprampdown, rampup

  //LivingroomLamp
  static pushbutton Livingroomlampoff("Off","");  
  static pushbutton Livingroomlampon("On","");
  Livingroomlampoff.setNext(&Livingroomlampon);
  static linselectbutton Livingroomoverallbrightness("Brightness", "Brightness%", 100);
  Livingroomlampon.setNext(&Livingroomoverallbrightness);
  static arrayselectbutton Livingroomsleep("Sleep Timer, "sleep%, {"1","2","5","10","15","20","30","45","60","90","120","180"}, 12);
  Livingroomoverallbrightness.setNext(&Livingroomsleep);
  Livingroomsleep.setNext(&Livingroomlampoff);
  //on, off, volume(ww,kw), sleep, sleeprampdown, rampup

  //Mediapi
  static pushbutton MediapiMusicoff("Off","");  
  static pushbutton MediapiMusicon("On","");
  MediapiMusicoff.setNext(&MediapiMusicon);
  MediapiMusicon.setNext(&MediapiMusicoff);
  //musicon, off, volume, sleep, 


  //MainMenu
  static submenubutton Mediapi(const char* name_, &MediapiMusicon);
  static submenubutton Livingroomlamp(const char* name_, &Livingroomlampon);
  Mediapi.setNext(&Livingroomlamp);
  static submenubutton cupboardlamp(const char* name_, &cupboardlamp);
  Livingroomlamp.setNext(&cupboardlamp);
  static submenubutton bedroomlamp(const char* name_, &bedroomlampon);
  cupboardlamp.setNext(&bedroomlamp);
  bedroomlamp.setNext(& Mediapi);

  //set current and display it
  self.currentButton=&Mediapi;
  self.curButtonPos=0;
  
  //draw
  self.draw();
}
next last

void menu::draw(){
  //button::setBackgroundColour();
  button::drawBasics(3);
  switch(self.curButtonPos){
    case 0:
      self.currentButton->draw(0, true);
      self.currentButton->next->draw(1, false);
      self.currentButton->next->next->draw(2, false);
      break;
    case 1:
      self.currentButton->last->draw(0, false);
      self.currentButton->draw(1, true);
      self.currentButton->next->draw(2, false);
      break;
    case 2:
    default:
      self.curButtonPos=2;
      self.currentButton->last->last->draw(0, false);
      self.currentButton->last->draw(1, false);
      self.currentButton->draw(2, true);
  }
  
}


void menu::left(const bool fast){
  self.currentButton->left(fast);
}

void menu::right(const bool fast){
  self.currentButton->right(fast);
}

void menu::down(){
  switch(self.curButtonPos){
    case 0:
    case 1:
      button::DeActivate(self.curButtonPos++, false);
      self.currentButton=self.currentButton->next;
      button::DeActivate(self.curButtonPos, true);
      break;
    case 2:
    default:
      self.curButtonPos=2;
      self.currentButton=self.currentButton->next;
      self.draw();
  }
}

void menu::up(){
  switch(self.curButtonPos){
    case 0:
    default:
      self.curButtonPos=0;
      self.currentButton=self.currentButton->last;
      self.draw();
      break;
    case 1:
    case 2:
      button::DeActivate(self.curButtonPos--, false);
      self.currentButton=self.currentButton->last;
      button::DeActivate(self.curButtonPos, true);
  }
}

void menu::pushLong(){
  self.curButtonPos=0;
  self.currentButton=self.currentButton->up;
  self.draw();
}

void menu::pushShort(){
  self.currentButton->push();
}


