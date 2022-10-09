#pragma once
#include <cstdint>

namespace Mouse
{
	enum MouseCode: uint16_t
	{
		Button1 = 0,
		Button2 = 1,
		Button3 = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,

		ButtonLast = Button8,
		LeftClick = Button1,
		RightClick = Button2,
		ButtonMiddle = Button3
	};
}