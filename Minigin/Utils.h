#pragma once
namespace dae 
{
	#define BUTTONA = "BUTTON_A";
	#define BUTTONB = "BUTTON_B";
	#define BUTTONX = "BUTTON_X";
	#define BUTTONY = "BUTTON_Y";

	#define BUTTONUP = "BUTTON_UP";
	#define BUTTONDOWN = "BUTTON_DOWN";
	#define BUTTONLEFT = "BUTTON_LEFT";
	#define BUTTONRIGHT = "BUTTON_RIGHT";

	#define LEFTSHOULDER = "L_SHOULDER";
	#define RIGHTSHOULDER = "R_SHOULDER";
	#define LEFTTHUNB = "L_THUMB";
	#define RIGHTTHUMB = "R_THUMB";

	#define BUTTONSTART = "BUTTON_START";
	#define BUTTONOPTIONS = "BUTTON_BACK";

	template<class T, class U> struct Pair
	{
		Pair(T cfirst, U csecond)
			: first{ cfirst }
			, second{ csecond }
		{}			

		T first;
		U second;
	};
}