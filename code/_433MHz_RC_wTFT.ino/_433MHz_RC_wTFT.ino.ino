#include <SPI.h>
#include <TFT.h>
//#include "comlib.h"

#define cs   10
#define dc   9
#define rst  8  

//TFT TFTscreen = TFT(cs, dc, rst);


class button{
  protected:
    String name;
    
    static void setBackgroundColour();
    static void setButtonColours();
    static void setActiveColour();
    static void setStroke2ButtonFillColours();
    static void setStroke2TextColours();
    static void buttonText(const String text, const uint8_t pos, const bool big);

  public:

    button* next=NULL;
    button* last=NULL;
    button* up=NULL;
    
    //static TFT TFTscreen;

    button();
    
    static void DeActivate(const uint8_t pos, const bool active);
    static void drawBasics(const uint8_t pos);
    
    virtual button* push() {};
    virtual void left(const bool fast) {};
    virtual void right(const bool fast){};
    virtual void draw(const uint8_t pos, const bool active) {};
    
    void setNext(const button*);
    void setnext(button* next_);
    void setlast(button* last_);
    void setup(button* up_);
};


class pushbutton: public button{
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

class linselectbutton: public button{
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

class arrayselectbutton: public button{
  private:
    char* message="";
    void send();
    uint8_t arrayLength;
    String *values;
    uint8_t curPos=0;
    uint8_t pos;
    
  public:
    arrayselectbutton(const String name_, const char* message_, const String* vals, const uint8_t nvals);
    button* push();
    void left(const bool fast);
    void right(const bool fast);
    void draw(const uint8_t pos, const bool active);
};


class submenubutton: public button{
  private:
    const button* down;

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
    void start();
    void left(const bool fast);
    void right(const bool fast);
    void down();
    void up();
    void pushLong();
    void pushShort();
};


TFT TFTscreen = TFT(cs, dc, rst);
//TFT TFTscreen=button::TFTscreen;
menu MENU= menu();
char buffer[50];


void setup() {
  MENU.start();
}


void loop() {


  delay(1000);
  MENU.up();
  
  delay(1000);
  MENU.up();
  
  delay(1000);
  MENU.pushShort();

  delay(1000);
  MENU.down();

  while(true){
    delay(100);
    MENU.right(false);
  }
}

void button::setnext(button* next_){
  this->next =next_;
}
void button::setlast(button* last_){
  this->last=last_;
}
void button::setup(button* up_){
  this->up=up_;
}

static void button::drawBasics(const uint8_t pos){
  TFTscreen.setRotation(0);
  TFTscreen.background(0, 0, 0);

  TFTscreen.stroke(0,255,255);
  TFTscreen.setTextSize(2);
  TFTscreen.text("Dustins RC: ",0,0);//15 High continue 16
  TFTscreen.setTextSize(5);
  button::setButtonColours();
  if(pos>=3){
    for(int i=0;i<3;i++) {
      TFTscreen.rect(2, 20+i*47, 124, 45);
    }
    return;
  }else{
    TFTscreen.rect(2, 20+pos*47, 124, 45);
  }
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
    TFTscreen.setTextSize(txtsize);
    TFTscreen.text(buffer, 64-6*txtsize*txtlength/2 , 36+pos*47);//15 High continue 16
  }else{
    TFTscreen.setTextSize(1);
    TFTscreen.text(buffer, 64-6*txtlength/2, 24+pos*47);//15 High continue 16
  }
}

void button::setNext(const button* next_){
  this->next=next_;
  next_->setlast(this);
}


void pushbutton::send(){
  //send this->message
}
    
pushbutton::pushbutton(const String name_, const char* message_){
  this->message=message_;
  this->name=name_;
}

button* pushbutton::push(){
  this->send();
  return this;
}

void pushbutton::left(const bool fast){
  return;
}
    
void pushbutton::right(bool fast){
  return;
}

void pushbutton::draw(const uint8_t pos, const bool active){
  if(active){
    this->DeActivate(pos,active);
  }
  button::buttonText(this->name, pos, true);
}




linselectbutton::linselectbutton(const String name_, const char* message_, const uint8_t maximum){
  this->message=message_;
  this->name=name_;
  this->maxVal=maximum;
}

void linselectbutton::send(){
  uint8_t offset=0;
  uint8_t i=0;
  for(i=0;this->message[i]!='\0';i++){
    buffer[i+offset]=this->message[i];
    if(this->message[i]=='%'){
      String Value = String(this->curVal);
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
  this->send();
}

button::button(){
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
    this->DeActivate(pos,active);
  }
  button::buttonText(this->name, pos, false);
  this->pos=pos;
  uint8_t sliderPos=10 + this->sliderBarWidth * (this->curVal-this->minVal)/(double) (this->maxVal-this->minVal);
  this->setSliderBarColours();
  TFTscreen.rect(10, 47+pos*47, this->sliderBarWidth, 5);
  this->setSliderColours();
  TFTscreen.circle(sliderPos ,49+pos*47, 4);
}

void linselectbutton::left(const bool fast){
  uint8_t sliderPos=10 + this->sliderBarWidth * (this->curVal-this->minVal)/(double) (this->maxVal-this->minVal);
  button::setButtonColours();
  TFTscreen.noStroke();
  TFTscreen.circle(sliderPos ,49+this->pos*47, 4);
  this->setSliderBarColours();
  TFTscreen.rect(10, 47+this->pos*47, this->sliderBarWidth, 5);
  if(fast){
    uint8_t offset=this->maxVal-this->minVal/10;
    if(this->curVal-this->minVal>offset){
      this->curVal=-offset;
    }else{
      this->curVal=this->minVal;
    }
  }else{
    this->curVal--;
  }
  sliderPos=10 + this->sliderBarWidth * (this->curVal-this->minVal)/(double) (this->maxVal-this->minVal);
  this->setSliderColours();
  TFTscreen.circle(sliderPos ,49+this->pos*47, 4);
}

void linselectbutton::right(const bool fast){
  uint8_t sliderPos=10 + this->sliderBarWidth * (this->curVal-this->minVal)/(double) (this->maxVal-this->minVal);
  button::setButtonColours();
  TFTscreen.noStroke();
  TFTscreen.circle(sliderPos ,49+this->pos*47, 4);
  this->setSliderBarColours();
  TFTscreen.rect(10, 47+this->pos*47, this->sliderBarWidth, 5);
  if(fast){
    uint8_t offset=this->maxVal-this->minVal/10;
    if(this->curVal+offset<this->maxVal){
      this->curVal=+offset;
    }else{
      this->curVal=this->maxVal;
    }
  }else{
    if(this->curVal<this->maxVal){
      this->curVal++;
    }
  }
  sliderPos=10 + this->sliderBarWidth * (this->curVal-this->minVal)/(double) (this->maxVal-this->minVal);
  this->setSliderColours();
  TFTscreen.circle(sliderPos ,49+this->pos*47, 4);
}


void arrayselectbutton::send(){
  uint8_t offset=0;
  uint8_t i=0;
  for(i=0;this->message[i]!='\0';i++){
    buffer[i+offset]=this->message[i];
    if(this->message[i]=='%'){
      this->values[this->curPos].toCharArray(&(buffer[i]), 6);
      while(offset<6){
        if(buffer[i+offset]=='\0'){
          break;
        }
        offset++;
      }
    }
  }
  //send buffer with length i+offset
}
    

arrayselectbutton::arrayselectbutton(const String name_, const char* message_, const String* vals, const uint8_t nvals){
  this->arrayLength=nvals;
  this->values=vals;
  this->message=message_;
  this->name=name_;
}

button* arrayselectbutton::push(){
  this->send();
}

void arrayselectbutton::draw(const uint8_t pos, bool active){
  if(active){
    this->DeActivate(pos,active);
  }
  button::buttonText(this->name, pos, false);
  this->pos=pos;
  button::setButtonColours();
  TFTscreen.stroke(0,0,128);
  TFTscreen.rect(40, 87 +this->pos*47, 40, 20);
  button::setStroke2TextColours();
  TFTscreen.setTextSize(1);
  this->values[this->curPos].toCharArray(buffer, 6);
  TFTscreen.text(buffer, 43, 47+this->pos*47);//15 High continue 16
  if(this->curPos>0){
    this->values[this->curPos-1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 8, 47+this->pos*47);//15 High continue 16
  }
  if(this->arrayLength>this->curPos+1){
    this->values[this->curPos+1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 90, 47+this->pos*47);//15 High continue 16
  }
}

void arrayselectbutton::left(const bool fast){
  button::setStroke2ButtonFillColours();
  TFTscreen.setTextSize(1);
  this->values[this->curPos].toCharArray(buffer, 6);
  TFTscreen.text(buffer, 43, 47+this->pos*47);//15 High continue 16
  if(this->arrayLength>this->curPos+1){
    this->values[this->curPos+1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 90, 47+this->pos*47);//15 High continue 16
  }
  if(this->curPos>0){
    this->values[this->curPos-1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 8, 47+this->pos*47);//15 High continue 16
    this->curPos--;
  }
  if(fast){
    this->curPos=0;
  }
  button::setStroke2TextColours();
  TFTscreen.setTextSize(1);
  this->values[this->curPos].toCharArray(buffer, 6);
  TFTscreen.text(buffer, 43, 47+this->pos*47);//15 High continue 16
  if(this->curPos>0){
    this->values[this->curPos-1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 8, 47+this->pos*47);//15 High continue 16
  }
  if(this->arrayLength>this->curPos+1){
    this->values[this->curPos+1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 90, 47+this->pos*47);//15 High continue 16
  }
}

void arrayselectbutton::right(const bool fast){
  button::setStroke2ButtonFillColours();
  TFTscreen.setTextSize(1);
  this->values[this->curPos].toCharArray(buffer, 6);
  TFTscreen.text(buffer, 43, 47+this->pos*47);//15 High continue 16
  if(this->curPos>0){
    this->values[this->curPos-1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 8, 47+this->pos*47);//15 High continue 16
  }
  if(this->arrayLength>this->curPos+1){
    this->values[this->curPos+1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 90, 47+this->pos*47);//15 High continue 16
    this->curPos++;
  }
  if(fast){
    this->curPos=this->arrayLength-1;
  }
  button::setStroke2TextColours();
  TFTscreen.setTextSize(1);
  this->values[this->curPos].toCharArray(buffer, 6);
  TFTscreen.text(buffer, 43, 47+this->pos*47);//15 High continue 16
  if(this->curPos>0){
    this->values[this->curPos-1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 8, 47+this->pos*47);//15 High continue 16
  }
  if(this->arrayLength>this->curPos+1){
    this->values[this->curPos+1].toCharArray(buffer, 6);
    TFTscreen.text(buffer, 90, 47+this->pos*47);//15 High continue 16
  }
}



submenubutton::submenubutton(const String name_, const button* down_){
  this->name=name_;
  this->down=down_;
  this->down->setup(this);
  button* next=this->down->next;
  while(this->down != next){
    next->setup(this);
    next=next->next;
  }
}
    
button* submenubutton::push(){
  return this->down;
}
    
void submenubutton::draw(const uint8_t pos,bool active){
  if(active){
    this->DeActivate(pos,active);
  }
  button::buttonText(this->name, pos, true);
}
    
void submenubutton::left(const bool fast){
  return;
}
    
void submenubutton::right(const bool fast){
  return;
}


menu::menu(){
}


void menu::start(){

  TFTscreen.begin();


  //bedroomLamp
  static pushbutton bedroomlampoff("Off","");  
  static pushbutton bedroomlampon("On","");
  bedroomlampoff.setNext(&bedroomlampon);
  static pushbutton bedroomlampmusicon("MOn","");
  bedroomlampon.setNext(&bedroomlampmusicon);
  static pushbutton bedroomlampmusicoff("MOff","");
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
  const String sleeptimes[]={String("1"),String("2"),String("5"),String("10"),String("15"),String("20"),String("30"),String("45"),String("60"),String("90"),String("120"),String("180")};
  static arrayselectbutton Livingroomsleep(String("Sleep Timer"), "sleep%", sleeptimes, 12);
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
  static submenubutton Mediapi("MediaPI", &MediapiMusicon);
  static submenubutton Livingroomlamp("Livingroom", &Livingroomlampon);
  Mediapi.setNext(&Livingroomlamp);
  static submenubutton cupboardlamp("Cupboard", &cupboardlampon);
  Livingroomlamp.setNext(&cupboardlamp);
  static submenubutton bedroomlamp("Bedroom", &bedroomlampon);
  cupboardlamp.setNext(&bedroomlamp);
  bedroomlamp.setNext(& Mediapi);

  //set current and display it
  this->currentButton=&Mediapi;
  this->curButtonPos=0;
  
  //draw
  this->draw();
}

void menu::draw(){
  //button::setBackgroundColour();
  button::drawBasics(3);
  switch(this->curButtonPos){
    case 0:
      this->currentButton->draw(0, true);
      this->currentButton->next->draw(1, false);
      this->currentButton->next->next->draw(2, false);
      break;
    case 1:
      this->currentButton->last->draw(0, false);
      this->currentButton->draw(1, true);
      this->currentButton->next->draw(2, false);
      break;
    case 2:
    default:
      this->curButtonPos=2;
      this->currentButton->last->last->draw(0, false);
      this->currentButton->last->draw(1, false);
      this->currentButton->draw(2, true);
  }
  
}


void menu::left(const bool fast){
  this->currentButton->left(fast);
}

void menu::right(const bool fast){
  this->currentButton->right(fast);
}

void menu::down(){
  switch(this->curButtonPos){
    case 0:
    case 1:
      button::DeActivate(this->curButtonPos++, false);
      this->currentButton=this->currentButton->next;
      button::DeActivate(this->curButtonPos, true);
      break;
    case 2:
    default:
      this->curButtonPos=2;
      this->currentButton=this->currentButton->next;
      this->draw();
  }
}

void menu::up(){
  switch(this->curButtonPos){
    case 0:
    default:
      this->curButtonPos=0;
      this->currentButton=this->currentButton->last;
      this->draw();
      break;
    case 1:
    case 2:
      button::DeActivate(this->curButtonPos--, false);
      this->currentButton=this->currentButton->last;
      button::DeActivate(this->curButtonPos, true);
  }
}

void menu::pushLong(){
  this->curButtonPos=0;
  this->currentButton=this->currentButton->up;
  this->draw();
}

void menu::pushShort(){
  button* temp=this->currentButton->push();
  if(temp!=NULL && temp!=this->currentButton){
    this->currentButton=temp;
    this->curButtonPos=0;
    this->draw();
  }
}


