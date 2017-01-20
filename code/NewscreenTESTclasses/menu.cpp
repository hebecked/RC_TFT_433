


enum Tvpe { submenu, pushbutton, selectorlin, selectorarray, selectorlog };

class button{

private:
	int type;
	char* send;

public:

button(int type_);

virtual void push();
virtual void left();
virtual void right();
};


class submenubutton (button){



};
