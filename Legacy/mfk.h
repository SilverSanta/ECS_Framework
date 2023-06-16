#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "SDL.h"

namespace mfk {

	//STATICS
	static float Pi = 3.1415927f;
	static float Sqrt2 = 1.4142136f;


	//ENUMS
	enum class ColourList {

		RED_OPAQUE,
		GREEN_OPAQUE,
		BLUE_OPAQUE,
		CYAN_OPAQUE,
		YELLOW_OPAQUE,
		PURPLE_OPAQUE,
		ORANGE_OPAQUE,
		BLACK_OPAQUE,
		WHITE_OPAQUE,
		GREY_OPAQUE,
	};
	

	//STRUCTS
	struct NormalVector2D {
		NormalVector2D()
		{
			x = 0;
			y = 0;
		}

		NormalVector2D(float initx, float inity)
		{
			NormalBetweenTwoPoints(0.f, 0.f, initx, inity);			
		}

		float x;
		float y;

		float LengthBetweenTwoPoints(float x1, float y1, float x2, float y2) {

			float x = x2 - x1;
			float y = y2 - y1;

			float result = (float)pow((x * x + y * y), 0.5);
			return result;
		}
		void NormalBetweenTwoPoints(float x1, float y1, float x2, float y2) {

			float Length = LengthBetweenTwoPoints(x1, y1, x2, y2);
			
			if(Length != 0)
			{
				x = (x2-x1) / Length;
				y = (y2-y1) / Length;
			}
		}
	};
	struct ColourTable {

		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alfa;
	};
	struct Colour {

		static ColourTable Get_ColourTable(ColourList color) {
			ColourTable singlecolour = { 0,0,0,0 };
			switch (color) {

			case ColourList::RED_OPAQUE:
				singlecolour.red = 255, singlecolour.green = 0, singlecolour.blue = 0, singlecolour.alfa = 1;
				break;
			case ColourList::GREEN_OPAQUE:
				singlecolour.red = 0, singlecolour.green = 255, singlecolour.blue = 0, singlecolour.alfa = 1;
				break;
			case ColourList::BLUE_OPAQUE:
				singlecolour.red = 0, singlecolour.green = 0, singlecolour.blue = 255, singlecolour.alfa = 1;
				break;
			case ColourList::CYAN_OPAQUE:
				singlecolour.red = 0, singlecolour.green = 255, singlecolour.blue = 255, singlecolour.alfa = 1;
				break;
			case ColourList::YELLOW_OPAQUE:
				singlecolour.red = 255, singlecolour.green = 255, singlecolour.blue = 0, singlecolour.alfa = 1;
				break;
			case ColourList::PURPLE_OPAQUE:
				singlecolour.red = 255, singlecolour.green = 0, singlecolour.blue = 255, singlecolour.alfa = 1;
				break;
			case ColourList::ORANGE_OPAQUE:
				singlecolour.red = 255, singlecolour.green = 127, singlecolour.blue = 0, singlecolour.alfa = 1;
				break;
			case ColourList::BLACK_OPAQUE:
				singlecolour.red = 0, singlecolour.green = 0, singlecolour.blue = 0, singlecolour.alfa = 1;
				break;
			case ColourList::WHITE_OPAQUE:
				singlecolour.red = 255, singlecolour.green = 255, singlecolour.blue = 255, singlecolour.alfa = 1;
				break;
			case ColourList::GREY_OPAQUE:
				singlecolour.red = 127, singlecolour.green = 127, singlecolour.blue = 127, singlecolour.alfa = 1;
				break;
			}
			return singlecolour;
		}
	};
		

	//FUNCTIONS
	template<typename T>
	void Print(std::string text, T value) {

		std::cout << text << value << ".\n";
	}

	template <class T>
	void FindObjectInRepoAndKillIt(T* object, std::vector<T>* objectrepo)
	{
		if (object != nullptr)
		{
			typename std::vector<T>::iterator itr;
			for (itr = objectrepo->begin(); itr != objectrepo->end();)
			{
				if (object->m_Id == itr->m_Id)
				{
					itr = objectrepo->erase(itr);
				}
				else
				{
					itr++;
				}
			}
		}
	}	
}


