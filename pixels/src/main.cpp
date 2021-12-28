
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

int main()
{
	// main loop
	while (true)
	{
		DrawPoint(RandU16Max(WIDTH-1), RandU16Max(HEIGHT-1), RandU8Max(15));
	}
}
