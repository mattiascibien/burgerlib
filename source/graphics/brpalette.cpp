/***************************************

	Palette manager class

	Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#include "brpalette.h"
#include "brrenderer.h"

/*! ************************************

	\struct Burger::RGBWord8_t
	\brief Red, Green and Blue 8 bit values

	Three byte structure that contains the red, green and blue
	(in that order) color components where the value of
	0 mean absence of color and 255 is the maximum color
	
	\sa Burger::RGBAWord8_t, Burger::RGBWord16_t and Burger::RGBFloat_t

***************************************/

/*! ************************************

	\var Burger::RGBWord8_t::Aqua
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 0,255,255 :</td><td style="width:100%; background-color:#00FFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Black
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 0,0,0 :</td><td style="width:100%; background-color:#000000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Blue
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 0,0,255 :</td><td style="width:100%; background-color:#0000FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Fuchsia
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 255,0,255 :</td><td style="width:100%; background-color:#FF00FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Gray
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 128,128,128 :</td><td style="width:100%; background-color:#808080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Green
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 0,128,0 :</td><td style="width:100%; background-color:#008000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Lime
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 0,255,0 :</td><td style="width:100%; background-color:#00FF00">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Maroon
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 128,0,0 :</td><td style="width:100%; background-color:#800000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Navy
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 0,0,128 :</td><td style="width:100%; background-color:#000080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Olive
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 128,128,0 :</td><td style="width:100%; background-color:#808000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Orange
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 255,165,0 :</td><td style="width:100%; background-color:#FFA500">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Purple
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 128,0,128 :</td><td style="width:100%; background-color:#800080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Red
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 255,0,0 :</td><td style="width:100%; background-color:#FF0000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Silver
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 192,192,192 :</td><td style="width:100%; background-color:#C0C0C0">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Teal
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 0,128,128 :</td><td style="width:100%; background-color:#008080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::White
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 255,255,255 :</td><td style="width:100%; background-color:#FFFFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBWord8_t::Yellow
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGB 255,255,0 :</td><td style="width:100%; background-color:#FFFF00">&nbsp;</td></table> \endhtmlonly

***************************************/

const Burger::RGBWord8_t Burger::RGBWord8_t::Aqua = {0,255,255};
const Burger::RGBWord8_t Burger::RGBWord8_t::Black = {0,0,0};
const Burger::RGBWord8_t Burger::RGBWord8_t::Blue = {0,0,255};
const Burger::RGBWord8_t Burger::RGBWord8_t::Fuchsia = {255,0,255};
const Burger::RGBWord8_t Burger::RGBWord8_t::Gray = {128,128,128};
const Burger::RGBWord8_t Burger::RGBWord8_t::Green = {0,128,0};
const Burger::RGBWord8_t Burger::RGBWord8_t::Lime = {0,255,0};
const Burger::RGBWord8_t Burger::RGBWord8_t::Maroon = {128,0,0};
const Burger::RGBWord8_t Burger::RGBWord8_t::Navy = {0,0,128};
const Burger::RGBWord8_t Burger::RGBWord8_t::Olive = {128,128,0};
const Burger::RGBWord8_t Burger::RGBWord8_t::Orange = {255,165,0};
const Burger::RGBWord8_t Burger::RGBWord8_t::Purple = {128,0,128};
const Burger::RGBWord8_t Burger::RGBWord8_t::Red = {255,0,0};
const Burger::RGBWord8_t Burger::RGBWord8_t::Silver = {192,192,192};
const Burger::RGBWord8_t Burger::RGBWord8_t::Teal = {0,128,128};
const Burger::RGBWord8_t Burger::RGBWord8_t::White = {255,255,255};
const Burger::RGBWord8_t Burger::RGBWord8_t::Yellow = {255,255,0};

/*! ************************************

	\struct Burger::RGBAWord8_t
	\brief Red, Green,Blue and Alpha 8 bit values

	Four byte structure that contains the red, green, blue and alpha
	(in that order) color components where the value of
	0 mean absence of color and 255 is the maximum color
	
	\sa Burger::RGBWord8_t, Burger::RGBAWord16_t and Burger::RGBAFloat_t

***************************************/

/*! ************************************

	\var Burger::RGBAWord8_t::Aqua
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,255,255,255 :</td><td style="width:100%; background-color:#00FFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Black
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,0,255 :</td><td style="width:100%; background-color:#000000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Blue
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,255,255 :</td><td style="width:100%; background-color:#0000FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Fuchsia
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 255,0,255,255 :</td><td style="width:100%; background-color:#FF00FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Gray
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 128,128,128,255 :</td><td style="width:100%; background-color:#808080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Green
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,128,0,255 :</td><td style="width:100%; background-color:#008000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Lime
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,255,0,255 :</td><td style="width:100%; background-color:#00FF00">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Maroon
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 128,0,0,255 :</td><td style="width:100%; background-color:#800000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Navy
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,128,255 :</td><td style="width:100%; background-color:#000080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Olive
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 128,128,0,255 :</td><td style="width:100%; background-color:#808000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Orange
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 255,165,0,255 :</td><td style="width:100%; background-color:#FFA500">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Purple
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 128,0,128,255 :</td><td style="width:100%; background-color:#800080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Red
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 255,0,0,255 :</td><td style="width:100%; background-color:#FF0000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Silver
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 192,192,192,255 :</td><td style="width:100%; background-color:#C0C0C0">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Teal
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,128,128,255 :</td><td style="width:100%; background-color:#008080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::White
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 255,255,255,255 :</td><td style="width:100%; background-color:#FFFFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAWord8_t::Yellow
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 255,255,0,255 :</td><td style="width:100%; background-color:#FFFF00">&nbsp;</td></table> \endhtmlonly

***************************************/

const Burger::RGBAWord8_t Burger::RGBAWord8_t::Aqua = {0,255,255,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Black = {0,0,0,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Blue = {0,0,255,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Fuchsia = {255,0,255,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Gray = {128,128,128,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Green = {0,128,0,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Lime = {0,255,0,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Maroon = {128,0,0,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Navy = {0,0,128,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Olive = {128,128,0,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Orange = {255,165,0,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Purple = {128,0,128,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Red = {255,0,0,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Silver = {192,192,192,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Teal = {0,128,128,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::White = {255,255,255,255};
const Burger::RGBAWord8_t Burger::RGBAWord8_t::Yellow = {255,255,0,255};

/*! ************************************

	\struct Burger::RGBWord16_t
	\brief Red, Green and Blue 16 bit values

	Six byte structure that contains the red, green and blue
	(in that order) color components where the value of
	0 mean absence of color and 65535 is the maximum color
	
	\sa Burger::RGBAWord16_t, Burger::RGBWord8_t and Burger::RGBFloat_t

***************************************/

/*! ************************************

	\struct Burger::RGBAWord16_t
	\brief Red, Green,Blue and Alpha 16 bit values

	Eight byte structure that contains the red, green, blue and alpha
	(in that order) color components where the value of
	0 mean absence of color and 65535 is the maximum color
	
	\sa Burger::RGBWord16_t, Burger::RGBAWord8_t and Burger::RGBAFloat_t

***************************************/

/*! ************************************

	\struct Burger::RGBFloat_t
	\brief Red, Green and Blue 32 bit floating point values
	
	Structure that contains the red, green and blue
	(in that order) color components where the value of
	0.0f mean absence of color and 1.0f is the maximum color
	
	\sa Burger::RGBAFloat_t, Burger::RGBWord8_t or Burger::RGBWord16_t

***************************************/

/*! ************************************

	\var Burger::RGBFloat_t::Aqua
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,1.0f,1.0f :</td><td style="width:100%; background-color:#00FFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Black
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,0 :</td><td style="width:100%; background-color:#000000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Blue
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,1.0f :</td><td style="width:100%; background-color:#0000FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Fuchsia
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,0,1.0f :</td><td style="width:100%; background-color:#FF00FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Gray
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0.5f,0.5f :</td><td style="width:100%; background-color:#808080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Green
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0.5f,0 :</td><td style="width:100%; background-color:#008000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Lime
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,1.0f,0 :</td><td style="width:100%; background-color:#00FF00">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Maroon
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0,0 :</td><td style="width:100%; background-color:#800000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Navy
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,0.5f :</td><td style="width:100%; background-color:#000080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Olive
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0.5f,0 :</td><td style="width:100%; background-color:#808000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Orange
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,165.0f/255.0f,0 :</td><td style="width:100%; background-color:#FFA500">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Purple
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0,0.5f :</td><td style="width:100%; background-color:#800080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Red
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,0,0 :</td><td style="width:100%; background-color:#FF0000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Silver
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.75f,0.75f,0.75f :</td><td style="width:100%; background-color:#C0C0C0">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Teal
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0.5f,0.5f :</td><td style="width:100%; background-color:#008080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::White
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,1.0f,1.0f :</td><td style="width:100%; background-color:#FFFFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBFloat_t::Yellow
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,1.0f,0 :</td><td style="width:100%; background-color:#FFFF00">&nbsp;</td></table> \endhtmlonly

***************************************/

const Burger::RGBFloat_t Burger::RGBFloat_t::Aqua = {0,1.0f,1.0f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Black = {0,0,0};
const Burger::RGBFloat_t Burger::RGBFloat_t::Blue = {0,0,1.0f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Fuchsia = {1.0f,0,1.0f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Gray = {0.5f,0.5f,0.5f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Green = {0,0.5f,0};
const Burger::RGBFloat_t Burger::RGBFloat_t::Lime = {0,1.0f,0};
const Burger::RGBFloat_t Burger::RGBFloat_t::Maroon = {0.5f,0,0};
const Burger::RGBFloat_t Burger::RGBFloat_t::Navy = {0,0,0.5f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Olive = {0.5f,0.5f,0};
const Burger::RGBFloat_t Burger::RGBFloat_t::Orange = {1.0f,165.0f/255.0f,0};
const Burger::RGBFloat_t Burger::RGBFloat_t::Purple = {0.5f,0,0.5f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Red = {1.0f,0,0};
const Burger::RGBFloat_t Burger::RGBFloat_t::Silver = {0.75f,0.75f,0.75f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Teal = {0,0.5f,0.5f};
const Burger::RGBFloat_t Burger::RGBFloat_t::White = {1.0f,1.0f,1.0f};
const Burger::RGBFloat_t Burger::RGBFloat_t::Yellow = {1.0f,1.0f,0};


/*! ************************************

	\struct Burger::RGBAFloat_t
	\brief Red, Green,Blue and Alpha 32 bit floating point values
	
	Structure that contains the red, green, blue and alpha
	(in that order) color components where the value of
	0.0f mean absence of color and 1.0f is the maximum color
	
	\sa Burger::RGBFloat_t, Burger::RGBAWord8_t or Burger::RGBAWord16_t

***************************************/


/*! ************************************

	\var Burger::RGBAFloat_t::Aqua
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,1.0f,1.0f,1.0f :</td><td style="width:100%; background-color:#00FFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Black
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,0,1.0f :</td><td style="width:100%; background-color:#000000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Blue
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,1.0f,1.0f :</td><td style="width:100%; background-color:#0000FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Fuchsia
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,0,1.0f,1.0f :</td><td style="width:100%; background-color:#FF00FF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Gray
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0.5f,0.5f,1.0f :</td><td style="width:100%; background-color:#808080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Green
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0.5f,0,1.0f :</td><td style="width:100%; background-color:#008000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Lime
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,1.0f,0,1.0f :</td><td style="width:100%; background-color:#00FF00">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Maroon
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0,0,1.0f :</td><td style="width:100%; background-color:#800000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Navy
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0,0.5f,1.0f :</td><td style="width:100%; background-color:#000080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Olive
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0.5f,0,1.0f :</td><td style="width:100%; background-color:#808000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Orange
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,165.0f/255.0f,0,1.0f :</td><td style="width:100%; background-color:#FFA500">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Purple
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.5f,0,0.5f,1.0f :</td><td style="width:100%; background-color:#800080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Red
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,0,0,1.0f :</td><td style="width:100%; background-color:#FF0000">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Silver
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0.75f,0.75f,0.75f,1.0f :</td><td style="width:100%; background-color:#C0C0C0">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Teal
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 0,0.5f,0.5f,1.0f :</td><td style="width:100%; background-color:#008080">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::White
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,1.0f,1.0f,1.0f :</td><td style="width:100%; background-color:#FFFFFF">&nbsp;</td></table> \endhtmlonly

	\var Burger::RGBAFloat_t::Yellow
	\htmlonly <table border="0" style="width:100%"><td style="white-space:nowrap">RGBA 1.0f,1.0f,0,1.0f :</td><td style="width:100%; background-color:#FFFF00">&nbsp;</td></table> \endhtmlonly

***************************************/

const Burger::RGBAFloat_t Burger::RGBAFloat_t::Aqua = {0,1.0f,1.0f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Black = {0,0,0,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Blue = {0,0,1.0f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Fuchsia = {1.0f,0,1.0f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Gray = {0.5f,0.5f,0.5f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Green = {0,0.5f,0,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Lime = {0,1.0f,0,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Maroon = {0.5f,0,0,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Navy = {0,0,0.5f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Olive = {0.5f,0.5f,0,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Orange = {1.0f,165.0f/255.0f,0,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Purple = {0.5f,0,0.5f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Red = {1.0f,0,0,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Silver = {0.75f,0.75f,0.75f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Teal = {0,0.5f,0.5f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::White = {1.0f,1.0f,1.0f,1.0f};
const Burger::RGBAFloat_t Burger::RGBAFloat_t::Yellow = {1.0f,1.0f,0,1.0f};

/*! ************************************

	\class Burger::Palette
	\brief Static functions to manage palettes

	This is a set of subroutines that will take a 256 color, RGB triplet palette
	and manage the colors within.

***************************************/

/*! ************************************

	\brief Convert 15 bit color to 24 bit color

	Given a 15 bit RGB pixel value, convert it to a
	24 bit RGB triplet

	\param pOutput Pointer to the output buffer.
	\param uInput 15 bit RGB value in 5:5:5 format.

	\sa FromRGB15(RGBAWord8_t *,Word), FromRGB16(RGBWord8_t *,Word), Renderer::RGB5ToRGB8Table

***************************************/

void BURGER_API Burger::Palette::FromRGB15(RGBWord8_t *pOutput,Word uInput)
{
	pOutput->m_uRed = Renderer::RGB5ToRGB8Table[(uInput>>10U)&0x1FU];	// Red
	pOutput->m_uGreen = Renderer::RGB5ToRGB8Table[(uInput>>5U)&0x1FU];	// Green
	pOutput->m_uBlue = Renderer::RGB5ToRGB8Table[uInput&0x1FU];			// Blue
}

/*! ************************************

	\brief Convert 15 bit color to 32 bit color

	Given a 15 bit RGB pixel value, convert it to a
	32 bit RGB quad. Alpha will be set to 255

	\param pOutput Pointer to the output buffer.
	\param uInput 15 bit RGB value in 5:5:5 format.

	\sa FromRGB16(RGBAWord8_t *,Word), FromRGB15(RGBWord8_t *,Word), Renderer::RGB5ToRGB8Table

***************************************/

void BURGER_API Burger::Palette::FromRGB15(RGBAWord8_t *pOutput,Word uInput)
{
	pOutput->m_uRed = Renderer::RGB5ToRGB8Table[(uInput>>10U)&0x1FU];	// Red
	pOutput->m_uGreen = Renderer::RGB5ToRGB8Table[(uInput>>5U)&0x1FU];	// Green
	pOutput->m_uBlue = Renderer::RGB5ToRGB8Table[uInput&0x1FU];			// Blue
	pOutput->m_uAlpha = 255;											// Alpha
}

/*! ************************************

	\brief Convert 1:5:5:5 bit color to 32 bit color

	Given a 1:5:5:5 bit RGB pixel value, convert it to a
	32 bit RGB quad. Alpha will be set to 0 or 255

	\param pOutput Pointer to the output buffer.
	\param uInput 16 bit RGB value in 1:5:5:5 format.

	\sa FromRGB16(RGBAWord8_t *,Word), FromRGB15(RGBWord8_t *,Word), Renderer::RGB5ToRGB8Table

***************************************/

void BURGER_API Burger::Palette::FromRGB1555(RGBAWord8_t *pOutput,Word uInput)
{
	pOutput->m_uRed = Renderer::RGB5ToRGB8Table[(uInput>>10U)&0x1FU];	// Red
	pOutput->m_uGreen = Renderer::RGB5ToRGB8Table[(uInput>>5U)&0x1FU];	// Green
	pOutput->m_uBlue = Renderer::RGB5ToRGB8Table[uInput&0x1FU];			// Blue
	pOutput->m_uAlpha = static_cast<Word8>((uInput&0x8000) ? 0xFFU : 0);	// Alpha
}

/*! ************************************

	\brief Convert 16 bit color to 24 bit color

	Given a 16 bit RGB pixel value, convert it to a
	24 bit RGB triplet

	\param pOutput Pointer to the output buffer.
	\param uInput 16 bit RGB value in 5:6:5 format.

	\sa FromRGB15(RGBWord8_t *,Word), FromRGB16(RGBAWord8_t *,Word), Renderer::RGB5ToRGB8Table, Renderer::RGB6ToRGB8Table

***************************************/

void BURGER_API Burger::Palette::FromRGB16(RGBWord8_t *pOutput,Word uInput)
{
	pOutput->m_uRed = Renderer::RGB5ToRGB8Table[(uInput>>11U)&0x1FU];	// Red
	pOutput->m_uGreen = Renderer::RGB6ToRGB8Table[(uInput>>5U)&0x3FU];	// Green
	pOutput->m_uBlue = Renderer::RGB5ToRGB8Table[uInput&0x1F];			// Blue
}

/*! ************************************

	\brief Convert 16 bit color to 32 bit color

	Given a 16 bit RGB pixel value, convert it to a
	32 bit RGB quad. Alpha will be set to 255

	\param pOutput Pointer to the output buffer.
	\param uInput 16 bit RGB value in 5:6:5 format.

	\sa FromRGB15(RGBAWord8_t *,Word), FromRGB16(RGBWord8_t *,Word), Renderer::RGB5ToRGB8Table, Renderer::RGB6ToRGB8Table

***************************************/

void BURGER_API Burger::Palette::FromRGB16(RGBAWord8_t *pOutput,Word uInput)
{
	pOutput->m_uRed = Renderer::RGB5ToRGB8Table[(uInput>>11U)&0x1FU];	// Red
	pOutput->m_uGreen = Renderer::RGB6ToRGB8Table[(uInput>>5U)&0x3FU];	// Green
	pOutput->m_uBlue = Renderer::RGB5ToRGB8Table[uInput&0x1F];			// Blue
	pOutput->m_uAlpha = 255;											// Alpha
}

/*! ************************************

	\brief Convert 24 bit color to 15 bit color

	Given a 24 bit RGB triplet stored as an array of bytes
	organized as Red, Green and Blue, convert it to a
	15 bit RGB pixel value

	\param pInput Pointer to a 24 bit RGB triplet
	\return 24 bit color value as 5:5:5 color
	\sa FromRGB15() or ToRGB16()

***************************************/

Word BURGER_API Burger::Palette::ToRGB15(const RGBWord8_t *pInput)
{
	Word uColor = (pInput->m_uRed<<7U)&0x7C00U;	// Red
	uColor += (pInput->m_uGreen<<2U)&0x03E0U;	// Green
	uColor += (pInput->m_uBlue>>3U);			// Blue
	return uColor;
}

/*! ************************************

	\fn Word Burger::Palette::ToRGB15(const RGBAWord8_t *pInput)
	\brief Convert 32 bit color to 15 bit color

	Given a 32 bit RGBA quad stored as an array of bytes
	organized as Red, Green, Blue and Alpha convert it to a
	15 bit RGB pixel value. Alpha is ignored

	\param pInput Pointer to a 32 bit RGBA quad
	\return 32 bit color value as 5:5:5 color
	\sa FromRGB15() or ToRGB16()

***************************************/

/*! ************************************

	\brief Convert 24 bit color to 16 bit color

	Given a 24 bit RGB triplet stored as an array of bytes
	organized as Red, Green and Blue, convert it to a
	16 bit RGB pixel value

	\param pInput Pointer to a 24 bit RGB triplet
	\return 24 bit color value as 5:6:5 color
	\sa FromRGB16() or ToRGB15()

***************************************/

Word BURGER_API Burger::Palette::ToRGB16(const RGBWord8_t *pInput)
{
	Word uColor = (pInput->m_uRed<<8U)&0xF800U;	// Red
	uColor += (pInput->m_uGreen<<3U)&0x07E0;	// Green
	uColor += pInput->m_uBlue>>3U;				// Blue
	return uColor;
}

/*! ************************************

	\fn Word Burger::Palette::ToRGB16(const RGBAWord8_t *pInput)
	\brief Convert 32 bit color to 16 bit color

	Given a 32 bit RGBA quad stored as an array of bytes
	organized as Red, Green, Blue and Alpha convert it to a
	16 bit RGB pixel value. Alpha is ignored

	\param pInput Pointer to a 32 bit RGBA quad
	\return 32 bit color value as 5:6:5 color
	\sa FromRGB16() or ToRGB15()

***************************************/

/*! ************************************

	\brief Convert 24 bit color to current depth

	Given a 24 bit RGB triplet, convert it to a
	pixel value of the current depth. For 32 bit
	destination pixels depths, the alpha is set to 255

	\note This function can only return an 8, 15, 16, 24 or 32 bit quantity.
		Any other depth will force a return of 0.

	\param pInput Pointer to a 24 bit RGB triplet
	\param pDisplay Display context to determine the depth and base palette

	\return 24 bit color value as 15, 16 or 8 bit color

	\sa ToRGB16(), ToRGB15(), FindColorIndex(), ToDisplay(Word,Word,Word,const Display *)

***************************************/

Word BURGER_API Burger::Palette::ToDisplay(const RGBWord8_t *pInput,const Display *pDisplay)
{
	Word uResult;
	switch (pDisplay->GetDepth()) {
	default:
		uResult = 0;
		break;
	case 8:
		uResult = FindColorIndex(pDisplay->GetPalette()+3,pInput->m_uRed,pInput->m_uGreen,pInput->m_uBlue,254)+1;
		break;
	case 15:
		uResult = (pInput->m_uRed<<7U)&0x7C00U;		// Red
		uResult += (pInput->m_uGreen<<2U)&0x03E0U;	// Green
		uResult += (pInput->m_uBlue)>>3U;			// Blue
		break;
	case 16:
		uResult = (pInput->m_uRed<<8U)&0xF800U;
		uResult += (pInput->m_uGreen<<3U)&0x7E0U;
		uResult += (pInput->m_uBlue>>3U);
		break;
	case 24:
		uResult = static_cast<Word>((pInput->m_uRed<<16U)+(pInput->m_uGreen<<8U)+pInput->m_uBlue);
		break;
	case 32:
		uResult = static_cast<Word>((pInput->m_uRed<<16U)+(pInput->m_uGreen<<8U)+pInput->m_uBlue)+0xFF000000U;
		break;
	}
	return uResult;
}

/*! ************************************

	\brief Convert 24 bit color to current depth

	Given a red, green and blue value, convert it to a
	pixel value of the current depth. For 32 bit destinations the
	alpha is set to 255

	\note This function can only return an 8, 15, 16, 24 or 32 bit quantity.
		Any other value will force a return of 0.
	
	\param uRed Value of red (0-255)
	\param uGreen Value of green (0-255)
	\param uBlue Value of blue (0-255)
	\param pDisplay Display context to determine the depth and base palette

	\return Color value as 8, 15, 16, 24 or 32 bit color

	\sa ToRGB16(), ToRGB15(), FindColorIndex(), ToDisplay(Word32,const Display *)

***************************************/

Word BURGER_API Burger::Palette::ToDisplay(Word uRed,Word uGreen,Word uBlue,const Display *pDisplay)
{
	Word uResult;
	switch(pDisplay->GetDepth()) {
	default:
		uResult = 0;
		break;
	case 8:
		uResult = FindColorIndex(pDisplay->GetPalette()+3,uRed,uGreen,uBlue,254)+1;
		break;
	case 15:
		uResult = (uRed<<7U)&0x7C00U;		// Red
		uResult += (uGreen<<2U)&0x03E0U;	// Green
		uResult += uBlue>>3U;				// Blue
		break;
	case 16:
		uResult = (uRed<<8U)&0xF800U;
		uResult += (uGreen<<3U)&0x7E0U;
		uResult += uBlue>>3U;
		break;
	case 24:
		uResult = (uRed<<16U)+(uGreen<<8U)+uBlue;
		break;
	case 32:
		uResult = (uRed<<16U)+(uGreen<<8U)+uBlue+0xFF000000U;
		break;
	}
	return uResult;
}

/*! ************************************

	\brief Convert 24 bit packed color to current depth

	Given a red/green/blue Word32 value, convert it to a
	pixel value of the current depth.

	If the input value has an alpha component, it's ignored

	For 32 bit destinations the alpha is set to 255

	\note This function can only return an 8, 15, 16, 24 or 32 bit quantity.
		Any other value will force a return of 0.

	\param uRGBColor Value of red ((0-255)<<16) + Green ((0-255)<<8) + Blue (0-255)
	\param pDisplay Display context to determine the depth and base palette

	\return Color value as 8, 15, 16, 24 or 32 bit color

	\sa ToRGB16(), ToRGB15(), FindColorIndex(), ToDisplay(Word,Word,Word,const Display *)

***************************************/

Word BURGER_API Burger::Palette::ToDisplay(Word32 uRGBColor,const Display *pDisplay)
{
	Word uRed = (uRGBColor>>16U)&0xFFU;		// Extract Red/Green/Blue
	Word uGreen = (uRGBColor>>8U)&0xFFU;
	Word uBlue = uRGBColor&0xFFU;
	Word uResult;
	switch (pDisplay->GetDepth()) {
	default:
		uResult = 0;
		break;
	case 8:
		uResult = FindColorIndex(pDisplay->GetPalette()+3,uRed,uGreen,uBlue,254)+1;
		break;
	case 15:
		uResult = (uRed<<7U)&0x7C00U;		// Red
		uResult += (uGreen<<2U)&0x03E0U;	// Green
		uResult += uBlue>>3U;				// Blue
		break;	
	case 16:
		uResult = (uRed<<8U)&0xF800U;
		uResult += (uGreen<<3U)&0x7E0U;
		uResult += uBlue>>3U;
		break;
	case 24:
		uResult = uRGBColor&0xFFFFFFU;
		break;
	case 32:
		uResult = uRGBColor|0xFF000000U;
		break;
	}
	return uResult;
}


/*! ************************************

	\brief Lookup a color in an 8 bit palette

	Given an 8 bit red, green and blue value,
	return the pixel value that is the most closest match.

	The palette is an array of bytes that is sizeof(Word8)*uCount*3

	\param pPalette Pointer to an 8 bit palette to map to (768 bytes)
	\param uRed Red value (0-255)
	\param uGreen Green value (0-255)
	\param uBlue Blue value (0-255)
	\param uCount Number of colors in the palette (The array byte size is this value multiplied by 3)
	\return Color index that is the closest match. (0-(Count-1))
	\sa ToDisplay(Word32,const Display *) or ToDisplay(Word,Word,Word,const Display *)

***************************************/

Word BURGER_API Burger::Palette::FindColorIndex(const Word8 *pPalette,Word uRed,Word uGreen,Word uBlue,Word uCount)
{
	Word uClosestIndex = 0;					// Index found
	if (uCount) {
		Word32 uClosestDist = BURGER_MAXUINT;	// Use a bogus number
		uRed=uRed+255;						// Adjust so that the negative index is positive
		uGreen=uGreen+255;
		uBlue=uBlue+255;
		Word i = 0;
		do {
			Word uTemp = pPalette[0];		// Get the difference
			uTemp = uRed-uTemp;
			Word uNewDelta = Renderer::ByteSquareTable[uTemp];	// Get the square
			uTemp = pPalette[1];
			uTemp = uGreen-uTemp;
			uTemp = Renderer::ByteSquareTable[uTemp];
			uNewDelta = uNewDelta+uTemp;
			uTemp = pPalette[2];
			uTemp = uBlue-uTemp;
			uTemp = Renderer::ByteSquareTable[uTemp];
			uNewDelta = uNewDelta+uTemp;
			if (uNewDelta < uClosestDist) {	// Closer?
				uClosestIndex = i;			// This color is the closest match
				if (!uNewDelta) {			// Perfect match?
					break;
				}
				uClosestDist = uNewDelta;		// Save the distance
			}
			pPalette = pPalette+3;			// Next index
		} while (++i<uCount);				// All colors checked?
	}
	return uClosestIndex;					// Return the lookup
}

/*! ************************************

	\brief Convert an 8 bit palette to a true color array

	Given an 8 bit palette (256 RGB values)
	create a color lookup table for drawing to a screen
	using this palette

	\param pOutput Pointer to an array of 256 Words
	\param pInput Pointer to an array of 256 3 byte RGB triplets
	\param pDisplay Display context to determine the depth and base palette
	\sa Make8BitLookupTable(Word *,RezFile *,Word,const Display *), ToDisplay(const RGBWord8_t *,const Display *)

***************************************/

void BURGER_API Burger::Palette::Make8BitLookupTable(Word *pOutput,const Word8 *pInput,const Display *pDisplay)
{
	Word i = 256;
	do {
		pOutput[0] = ToDisplay(reinterpret_cast<const RGBWord8_t *>(pInput),pDisplay);	// Convert RGB to native
		++pOutput;	// Next output pointer
		pInput+=3;	// Next triplet
	} while (--i);	// All done?
}

/*! ************************************

	\brief Convert an 8 bit palette to a true color array

	Given an 8 bit palette (256 RGB values)
	create a 16 bit color lookup table for drawing to a screen
	using this palette. The palette is read in from a resource
	file. The resource is released on exit.

	\param pOutput Pointer to an array of 256 Words
	\param pInput Pointer to the reference of the resource file
	\param uRezNum Which resource ID to load
	\param pDisplay Display context to determine the depth and base palette
	\sa Make8BitLookupTable(Word *,const Word8 *,const Display *), ToDisplay(const RGBWord8_t *,const Display *)

***************************************/

void BURGER_API Burger::Palette::Make8BitLookupTable(Word *pOutput,Burger::RezFile *pInput,Word uRezNum,const Display *pDisplay)
{
	const Word8 *pPalette = static_cast<const Word8 *>(pInput->Load(uRezNum));	// Load the resource
	if (pPalette) {										// Ok?
		Make8BitLookupTable(pOutput,pPalette,pDisplay);	// Create the table
		pInput->Release(uRezNum);						// Release the resource
	}
}

/*! ************************************

	\brief Create an 8 bit remap table

	Create a pixel data conversion table by taking
	the a source palette and trying to find the best match
	in a destination palette.
	The returned data will allow you to quickly color map a 256 color
	image onto any palette.

	\param pOutput Pointer to an array of 256 bytes
	\param pNewPalette Pointer to a palette of 256 colors (768 bytes) to map to
	\param pOldPalette Pointer to the palette of the shape (768 bytes) to map from
	\sa FindColorIndex(), MakeRemapLookupMasked()

***************************************/

void BURGER_API Burger::Palette::MakeRemapLookup(Word8 *pOutput,const Word8 *pNewPalette,const Word8 *pOldPalette)
{
	pNewPalette+=3;
	Word i = 256;
	do {
		pOutput[0] = static_cast<Word8>(FindColorIndex(pNewPalette,pOldPalette[0],pOldPalette[1],pOldPalette[2],254)+1);
		++pOutput;
		pOldPalette += 3;
	} while (--i);		// All 256 colors done?
}

/*! ************************************

	\brief Create an 8 bit remap table

	Create a pixel data conversion table by taking
	the a source palette and trying to find the best match
	in a destination palette.
	The returned data will allow you to quickly color map a 256 color
	image onto any palette.

	\note This will not alter color #0 and will not map to either color #0
	or color #255. Color #255 will be remapped to an equivalent color.

	\param pOutput Pointer to an array of 256 bytes
	\param pNewPalette Pointer to a palette of 256 colors (768 bytes) to map to
	\param pOldPalette Pointer to the palette of the shape (768 bytes) to map from
	\sa FindColorIndex(), MakeRemapLookup()

***************************************/

void BURGER_API Burger::Palette::MakeRemapLookupMasked(Word8 *pOutput,const Word8 *pNewPalette,const Word8 *pOldPalette)
{
	pNewPalette+=3;
	pOldPalette+=3;		// Skip the first color
	Word i = 255;
	pOutput[0] = 0;
	++pOutput;
	do {
		pOutput[0] = static_cast<Word8>(FindColorIndex(pNewPalette,pOldPalette[0],pOldPalette[1],pOldPalette[2],254)+1);
		++pOutput;
		pOldPalette += 3;
	} while (--i);		// All 256 colors done?
}

/*! ************************************

	\brief Create an 8 bit mask table

	Create a pixel mask table to make the MaskColor index
	transparent. This table will have the first 256 bytes containing
	the value to 'OR' with. The second 256 bytes will be the mask value.

	What effectively occurs is that the first 256 bytes are 0-255 in
	that order and the last 256 bytes are 0. The mask color will have a
	mask of 255 and a color of zero. This is to create a table
	to draw masked shapes without having to incur branch penalties
	on processors that have problems with mispredicted branches.

	\param pOutput Pointer to an array of 512 bytes
	\param uMaskIndex Color index to mask as invisible (0-255) or (-1) for not used
	\sa MakeRemapLookup()

***************************************/

void BURGER_API Burger::Palette::MakeColorMasks(Word8 *pOutput,Word uMaskIndex)
{
	Word i = 0;
	Word8 *pMask = pOutput+256;
	do {
		pOutput[i] = static_cast<Word8>(i);		// Save the or mask
		pMask[i] = 0;							// Save the and mask
	} while (++i<256);
	if (uMaskIndex<256) {						// Used?
		pMask[uMaskIndex] = 0xFFU;				// Set the and mask for invisible
		pOutput[uMaskIndex] = 0;				// Set the or mask for invisible
	}
}

/*! ************************************

	\brief Create a remap table to brighten or darken a shape

	If you wish to remap an 8 bit image by altering the brightness
	of one or more colors, this routine will create a remap table
	based on the new brightness.
	Give the palette for the shape and pass the red, green and blue
	scaling adjustments in percentage points.
	100 is treated as 1.0, 50 is .5 and 200 is 2.0 intensity

	\param pOutput Pointer to an array of 256 bytes for remap table
	\param pInput Pointer to an 8 bit palette (768 bytes)
	\param uRedAdjust Red percentage adjust (0-1000)
	\param uGreenAdjust Green percentage adjust (0-1000)
	\param uBlueAdjust Blue percentage adjust (0-1000)
	\sa MakeRemapLookup()

***************************************/

void BURGER_API Burger::Palette::MakeFadeLookup(Word8 *pOutput,const Word8 *pInput,Word uRedAdjust,Word uGreenAdjust,Word uBlueAdjust)
{
	Word8 PaletteBuffer[768];		// Local palette

	Word8 *pWork = PaletteBuffer;	// Create the new table using a temp palette
	const Word RANGEBITS = 6;		// 6 bits of fraction
	uRedAdjust = uRedAdjust<<RANGEBITS;			// Scale up for fraction
	uGreenAdjust = uGreenAdjust<<RANGEBITS;
	uBlueAdjust = uBlueAdjust<<RANGEBITS;
	const Word PERCENT = 100;		// 1.0 in percentages
	uRedAdjust = uRedAdjust/PERCENT;			// Create a fixed point number
	uGreenAdjust = uGreenAdjust/PERCENT;
	uBlueAdjust = uBlueAdjust/PERCENT;
	Word i = 0;						// All 256 colors
	do {
		Word uTemp = pInput[0];
		uTemp = uTemp * uRedAdjust;		// Perform the RGB scale
		uTemp = uTemp >> RANGEBITS;	// Isolate the integer
		if (uTemp>=256) {			// Too high?
			uTemp = 255;
		}
		pWork[0] = static_cast<Word8>(uTemp);

		uTemp = pInput[1];
		uTemp = uTemp * uGreenAdjust;
		uTemp = uTemp >> RANGEBITS;	// Perform the RGB scale
		if (uTemp>=256) {
			uTemp = 255;
		}
		pWork[1] = static_cast<Word8>(uTemp);

		uTemp = pInput[2];
		uTemp = uTemp * uBlueAdjust;
		uTemp = uTemp >> RANGEBITS;	// Perform the RGB scale
		if (uTemp>=256) {
			uTemp = 255;
		}
		pWork[2] = static_cast<Word8>(uTemp);
		pInput=pInput+3;
		pWork=pWork+3;
	} while (++i<256);
	pInput = pInput-768;							// Restore the input pointer
	MakeRemapLookup(pOutput,pInput,PaletteBuffer);	// Perform the actual work
}


/*! ************************************

	\fn void Burger::CopyPalette(RGBWord8_t *pOutput,const RGBWord8_t *pInput,Word uEntries)
	\brief Copy an array of RGBWord8_t entries

	Given a pointer to an array of RGBWord8_t color entries, copy the colors into
	another array of RGBWord8_t entries

	\param pOutput Pointer to the palette to copy to
	\param pInput Pointer to the palette to copy from
	\param uEntries Number of color entries in the palette
	\sa Burger::CopyPalette(RGBWord8_t *,const RGBAWord8_t *,Word)

***************************************/

/*! ************************************

	\brief Copy an array of RGBWord8_t entries from an array of RGBAWord8_t entries

	Given a pointer to an array of RGBAWord8_t color entries, copy the colors into
	another array of RGBWord8_t entries while losing the alpha value

	\param pOutput Pointer to the palette to copy to
	\param pInput Pointer to the palette to copy from
	\param uEntries Number of color entries in the palette
	\sa Burger::CopyPalette(RGBWord8_t *,const RGBWord8_t *,Word)

***************************************/

void BURGER_API Burger::CopyPalette(RGBWord8_t *pOutput,const RGBAWord8_t *pInput,Word uEntries)
{
	if (uEntries) {
		do {
			pOutput->m_uRed = pInput->m_uRed;
			pOutput->m_uGreen = pInput->m_uGreen;
			pOutput->m_uBlue = pInput->m_uBlue;
			++pInput;
			++pOutput;
		} while (--uEntries);
	}
}

/*! ************************************

	\brief Copy an array of RGBAWord8_t entries from an array of RGBWord8_t entries

	Given a pointer to an array of RGBWord8_t color entries, copy the colors into
	another array of RGBAWord8_t entries and set the alpha value to zero.

	\param pOutput Pointer to the palette to copy to
	\param pInput Pointer to the palette to copy from
	\param uEntries Number of color entries in the palette
	\sa Burger::CopyPalette(RGBAWord8_t *,const RGBAWord8_t *,Word)

***************************************/

void BURGER_API Burger::CopyPalette(RGBAWord8_t *pOutput,const RGBWord8_t *pInput,Word uEntries)
{
	if (uEntries) {
		do {
			pOutput->m_uRed = pInput->m_uRed;
			pOutput->m_uGreen = pInput->m_uGreen;
			pOutput->m_uBlue = pInput->m_uBlue;
			pOutput->m_uAlpha = 0;
			++pInput;
			++pOutput;
		} while (--uEntries);
	}
}

/*! ************************************

	\fn void Burger::CopyPalette(RGBAWord8_t *pOutput,const RGBAWord8_t *pInput,Word uEntries)
	\brief Copy an array of RGBAWord8_t entries from an array of RGBAWord8_t entries

	Given a pointer to an array of RGBAWord8_t color entries, copy the colors into
	another array of RGBAWord8_t entries

	\param pOutput Pointer to the palette to copy to
	\param pInput Pointer to the palette to copy from
	\param uEntries Number of color entries in the palette
	\sa Burger::CopyPalette(RGBAWord8_t *,const RGBWord8_t *,Word)

***************************************/