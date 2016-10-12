#include "Input.h"
#include <iostream>
#include <Windows.h>


using namespace std;


int main()
{
	Keyboard keyboard;
	Mouse mouse;
	int pos =0;

	if(!mouse)
	{
		cout <<"error" <<endl;
	}
	else
	{
		while(!keyboard.Key(KeyCode::LALT) || !keyboard.Key(KeyCode::F4))
		{
			keyboard.Update();
			mouse.Update();

			pos +=mouse.GetState().m_X/5;

			if(pos < 0) {pos =0;}
			else if(77 <= pos) {pos =76;}

			for(int i=0; i<pos; ++i)
			{
				cout <<" ";
			}
			cout <<"¡" <<endl;

			Sleep(16);
		}
	}

	return 0;
}