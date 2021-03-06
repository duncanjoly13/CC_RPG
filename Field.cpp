#include "stdafx.h"
#include "Field.h"
#include "Combat.h"
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

/* This file contains the functions for the field: instructions on how to build the maps
   from strings and how to draw the maps. As for traversal and interacting with the tiles,
   those functions are all in GameState.cpp. */

//Initializing occupants for tiles. Their type, location, and extra effects are what's part of them.
Occupant::Occupant()
{
	type = 0;
}

Occupant::Occupant(int kind, int locale, int bonus)
{
	type = kind;
	location = locale;
	extra = bonus;
}

//Initializing tiles. Their type, location in the map array, and extra effects are all initialized here.
Tile::Tile()
{
	type = FAIL;
	for (int i = 0; i < 4; i++)
	{
		adjacent[i] = NULL;
	}
}

Tile::Tile(int kind, int bonus, int area)
{
	extra = bonus;
	location = area;
	for (int i = 0; i < 4; i++)
	{
		adjacent[i] = NULL;
	}
	switch (kind)
	{
	default:
	{
		type = EMP;
		tenant = Occupant(0, area, 0);
		//cout << "CREATED FAILURE\n";
		break;
	}
	case 0: //Empty tile
	{
		type = EMP;
		tenant = Occupant(0, area, 0);
		//cout << "CREATED EMPTY\n";
		break;
	}
	case 1: //Start tile
	{
		type = EMP;
		tenant = Occupant(1, area, 0);
		//cout << "CREATED PLAYER\n";
		break;
	}
	case 2: //Treasure box
	{
		type = TREASURE;
		//Bonus = contents!
		tenant = Occupant(0, area, 0);
		//cout << "CREATED TREASURE\n";
		break;
	}
	case 3: //Vine
	{
		type = VINE;
		//Bonus = effect!
		tenant = Occupant(0, area, 0);
		//cout << "CREATED VINE\n";
		break;
	}
	case 4: //SAVE POINT
	{
		type = SAVE;
		tenant = Occupant(0, area, 0);
		//cout << "CREATED SAVE\n";
		break;
	}
	case 5: //ENEMY
	{
		type = EMP;
		tenant = Occupant(4, area, bonus);
		//Bonus = AI
		//cout << "CREATED ENEMY\n";
		break;
	}
	case 6: //Horizontal log
	{
		type = EMP;
		tenant = Occupant(2, area, 0);
		//cout << "CREATED LOGH\n";
		break;
	}
	case 7: //Vertical log
	{
		type = EMP;
		tenant = Occupant(3, area, 0);
		//cout << "CREATED LOGV\n";
		break;
	}
	case 8: //Water
	{
		type = WATER;
		tenant = Occupant(5, area, 0);
		//cout << "CREATED WATER\n";
		break;
	}
	case 9: //cutscene trigger
	{
		type = EMP;
		tenant = Occupant(6, area, bonus);
		//cout << "CREATED EVENT\n";
		break;
	}
	case 10: //Map transition tile
	{
		type = EMP;
		tenant = Occupant(7, area, bonus);
		//cout << "CREATED TRANSITION\n";
		break;
	}
	case 11: //Text examine tile
	{
		type = TEXT;
		tenant = Occupant(0, area, 0);
		//cout << "CREATED SIGNPOST\n";
		break;
	}
	}
}

//Determines how far you can see from one tile, to hide things behind walls.
//You can see 4 tiles east and west, but only 3 tiles north and south
int Tile::sight(int direction)
{

	Tile reference = *this;
	int counter = 0;
	for (int i = 0; i < 4; i++)
	{
		//cout << i;
		if (reference.adjacent[direction] == NULL)
		{
			if (direction < 2 && i > 3)
				return 3;
			return counter;
		}
		else
		{
			reference = *reference.adjacent[direction];
			counter++;
		}
	}
	if (direction < 2)
	{
		return 3;
	}

	return counter;
}

//Creates the string to draw each row of each tile.
//If anyone's combing this code, don't fucking touch this. It took far too long to get it right.
string Tile::printOut(int row)
{
	if (type == FAIL)
		return "      ";
	string output;
	string occupant;
	string typeOut = " ";
	string wall = " ";

	if (adjacent[2] == NULL)
		wall = "|";

	switch (type)
	{
	default:
	{
		typeOut = " ";
		break;
	}
	case 1:
	{
		typeOut = " ";
		break;
	}
	case 2:
	{
		typeOut = "{";
		break;
	}
	case 3:
	{
		typeOut = "S";
		break;
	}
	case 4:
	{
		typeOut = "#";
		break;
	}
	}

	switch (row)
	{
	default:
		output = "      ";
		break;
	case 0:
	{
		switch (tenant.type)
		{
		default:
		{
			occupant = "   ";
			break;
		}
		case 2:
		{
			occupant = "___";
			break;
		}
		case 3:
		{
			occupant = "/-\\";
			break;
		}
		case 4:
		{
			if (tenant.direction == 0)
				occupant = " ^ ";
			else
				occupant = "   ";
			break;
		}
		case 5:
		{
			occupant = "~~~";
			break;
		}
		case 7:
		{
			if (tenant.direction == 0)
				occupant = " ^ ";
			else
				if (tenant.direction == 1)
				{
					occupant = " | ";
				}
				else
					occupant = "   ";
			break;
		}
		}
		output = typeOut + occupant + " " + wall;
		break;
	}
	case 1:
	{
		switch (tenant.type)
		{
		default:
			occupant = "     ";
			break;
		case 1:
		{
			occupant = "  O  ";
			break;
		}
		case 2:
		{
			occupant = "(___)";
			break;
		}
		case 3:
		{
			occupant = " | | ";
			break;
		}
		case 4:
		{
			if (tenant.direction == 3)
				occupant = "< 0  ";
			else
				if (tenant.direction == 2)
					occupant = "  0 >";
				else
					occupant = "  0  ";
			break;
		}
		case 5:
		{
			occupant = " ~~~ ";
			break;
		}
		case 7:
		{
			if (tenant.direction == 3)
				occupant = "<----";
			else
				if (tenant.direction == 2)
					occupant = "---->";
				else
					occupant = "  |  ";
			break;
		}
		}
		output = occupant + wall;
		break;
	}
	case 2:
	{
		switch (this->tenant.type)
		{
		default:
			occupant = "   ";
			break;
		case 2:
		{
			occupant = "   ";
			break;
		}
		case 3:
		{
			occupant = "\\-/";
			break;
		}
		case 4:
		{
			if (this->tenant.direction == 0)
				occupant = " v ";
			else
				occupant = "   ";
			break;
		}
		case 5:
		{
			occupant = "~~~";
			break;
		}
		case 7:
		{
			if (tenant.direction == 1)
				occupant = " v ";
			else
				if (tenant.direction == 0)
				{
					occupant = " | ";
				}
				else
					occupant = "   ";
			break;
		}
		}
		output = " " + occupant + " " + wall;
		break;
	}
	case 3:
	{
		string floor;
		if (adjacent[1] == NULL)
			floor = "-----";
		else
			floor = "     ";
		output = floor + " ";
		break;
	}
	}

	return output;
}

//Fills in the map array based on a string, and then links them together.
//Maploc determines where you start
Tile constructMap(string mapGen, Tile map[], int mapLoc)
{
	//Process string and create map tiles
	int mapWidth;
	int mapHeight;

	switch (mapGen[0])
	{
	default:
	{
		mapWidth = 25;
		mapHeight = 20;
		break;
	}
	case 0: // 25 x 20
	{
		mapWidth = 25;
		mapHeight = 20;
		break;
	}
	case 1: // 50 x 10
	{
		mapWidth = 50;
		mapHeight = 10;
		break;
	}
	case 2: // 10 x 50
	{
		mapWidth = 10;
		mapHeight = 50;
		break;
	}
	}

	int counter = 0;
	int edit = 0;
	int displacement = 1;
	int ABORT = mapGen.length();
	int saveStart = -1;
	Tile start;


	for (int i = 0; i < mapHeight; i++)
	{

		for (int j = 0; j < mapWidth; j++)
		{
			if ((j + displacement) >= ABORT)
			{
				break;
			}
			int checkHere = j + displacement;
			char currChar = mapGen[checkHere]; 
			if (currChar != 48)
			{

				Tile boop;
				int hope = currChar;
				hope = hope - 49;
				if (hope == 2 || hope == 3 || hope == 5 || hope == 9 || hope == 10 || hope == 11)
				{
					char bonus = mapGen[checkHere + 1];
					int bonanza = bonus;
					bonanza = bonanza - 48;
					boop = Tile(hope, bonanza, counter);
					displacement++;
					if (hope == 10)
					{
						boop.tenant.direction = mapGen[checkHere + 2] - 44;
						displacement++;
					}
				}
				else
				{
					boop = Tile(hope, 0, counter);
				}
				boop.tierH = j;
				boop.tierV = i;
				if (hope != 0 && hope != 1 && hope != 4 && hope != 8 && hope != 10 && hope != 11)
				{
					boop.editNum = edit;
					edit++;
				}
				map[counter] = boop;
				counter++;

				if (hope == 1) //The number 2 is the start tile
				{
					saveStart = counter - 1;
				}
			}
		}
		displacement = displacement + mapWidth;
	}

	//Link all the map tiles
	int compH;
	int compV;

	for (int i = 0; i < 500; i++)
	{
		if (map[i].type != FAIL)
		{
			compH = map[i].tierH;
			compV = map[i].tierV;
			for (int j = 0; j < 500; j++)
			{
				if (map[j].type != FAIL)
				{
					if (map[j].tierH == compH && map[j].tierV == compV - 1) //NORTH
						map[i].adjacent[0] = &map[j];
					if (map[j].tierH == compH && map[j].tierV == compV + 1) //SOUTH
						map[i].adjacent[1] = &map[j];
					if (map[j].tierH == compH + 1 && map[j].tierV == compV) //EAST
						map[i].adjacent[2] = &map[j];
					if (map[j].tierH == compH - 1 && map[j].tierV == compV) //WEST
						map[i].adjacent[3] = &map[j];
				}
			}
		}
	}

	if (saveStart < 0)
		start = map[0];
	else
		start = map[saveStart];

	if (mapLoc > -1)
	{
		start = map[mapLoc];
	}


	return start;
}

//Spits back one of the preset map layouts. See the notes and crap below.
string mapGeneration(int layout)
{
	string output;
	switch (layout)
	{
	case 0:
	{
		output = "000000000000000000000000000111111110000000111111110011111111000000011111611100;//11111111111111111111110";
		output = output + "01111111100010001111162110011111111000100011111111000000000000010000000000000000000000001000000000000"; 
		output = output + "000000114011111113011000000000000111811211711100000000000011511111119110000000000000000000000000000000";
		break;
	}
	/*
	0	0000000000000000000000000			ITEMS: Band-aid (0)
		0111111110000000111111110			SWITCHES: (0) INOPERABLE CURRENTLY
		0111111110000000111116110			ENEMY MOBS: (1) (2)
		0;11111111111111111111110
		0111111110001000111116110
		0111111110001000111111110
		0000000000001000000000000
		0000000000001000000000000
		0000001141111111311000000
		0000001118112117111000000
		0000001151111111911000000
		0000000000000000000000000
	 */
	case 1:
	{
		output = "00000000000000000000000000000000000111111100000000000000000011111110000000000000000001112111000000000";
		output = output + "000000000111111100000000000000000011111110000000000000000000000000000000000";
		break;
	}
	/*
	0	0000000000000000000000000
		0000000001111111000000000
		0000000001111111000000000
		0000000001112111000000000
		0000000001111111000000000
		0000000001111111000000000
		0000000000000000000000000
	*/
	}
	return output;
}
	
//Spits back a miniature map of the area for you to see. I forget how you activate this.
void minimap(Tile map[])
{
	cout << "\n\n";
	int leftMost = 0;
	int layer = 0;
	for (int i = 0; i < 500; i++)
	{
		if (map[i].type != FAIL)
		{
			if (layer != map[i].tierV)
			{
				for (int j = 0; j < (map[i].tierV - layer); j++)
				{
					cout << "\n";
				}
				layer = map[i].tierV;
				leftMost = 0;
			}

			for (int j = 0; j < (map[i].tierH - leftMost); j++)
			{
				cout << " ";
			}
			leftMost = map[i].tierH + 1;

			switch (map[i].type)
			{
			default:
				cout << "0";
				break;
			case EMP:
				cout << "0";
				break;
			case VINE:
				cout << "{";
				break;
			case SAVE:
				cout << "*";
				break;
			case TREASURE:
				cout << "$";
				break;
			case WATER:
				cout << "~";
				break;
			}
		}

	}
	cout << "\n\n";
}

//Draws the seeable tiles on screen
void drawTiles(Tile map[])
{
	bool ceilingCheck[9]{false, false, false, false, false, false, false, false, false};
	int offset = 0;

	cout << "  ";
	for (int i = 0; i < 9; i++)
	{
		if (map[i].type != FAIL)
		{
			if (map[i].adjacent[0] == NULL)
			{
				cout << "----- ";
			}
			else
			{
				cout << "      ";
			}
			ceilingCheck[i] = true;
		}
		
	}
	cout << "\n ";

	for (int i = 0; i < 7; i++) //7 rows
	{
		for (int j = 0; j < 3; j++) //3 rows per tile center
		{
			bool wall = false;
			for (int k = 0; k < 9; k++) //9 columns
			{
				if (map[k + offset].type != FAIL && wall == false)
				{
					if (map[k + offset].adjacent[3] == NULL || map[k + offset].adjacent[3]->type == FAIL)
						cout << "|";
					else
						cout << " ";
					wall = true;
				}
				cout << map[k + offset].printOut(j);
			}
			if (i == 3)
			{
				switch (j)
				{
				case 0:
					cout << "         1";
					break;
				case 1:
					cout << "       3 O 4";
					break;
				case 2:
					cout << "         2";
					break;
				}
			}
			if (i == 4)
			{
				switch (j)
				{
				case 0:
					break;
				case 1:
					cout << "         e";
					break;
				case 2:
					break;
				}
			}
			cout << "\n ";
		}
		cout << " ";

		for (int j = 0; j < 9; j++) //Row 4 check
		{
			if (ceilingCheck[j] == false)
			{
				if (map[j + offset + 9].type != FAIL)
				{
					if (map[j + offset + 9].adjacent[0] == NULL)
					{
						cout << "----- ";
					}
					else
					{
						cout << "      ";
					}
					ceilingCheck[j] = true;
				}
				else
				{
					cout << "      ";
				}
			}
			else
			{
				cout << map[j + offset].printOut(3);
			}
		}
		if (i == 3)
		{
			cout << " Type number to move";
		}
		if (i == 4)
		{
			cout << " Type 'e' to examine";
		}
		cout << "\n ";
		offset = offset + 9;
		sleep(.025);
	}

}

//Prepares the tiles to be drawn, and then calls the functions to draw them.
void mapDraw(Tile start)
{
	cleanup();
	Tile seeableMap[63];
	for (int a = 0; a < 63; a++)
	{
		seeableMap[a] = Tile();
	}

	seeableMap[31] = start;
	int boundary;
	Tile reference = start;

	boundary = start.sight(3); 
	for (int i = 0; i < boundary; i++)
	{
		seeableMap[31 - (i + 1)] = *reference.adjacent[3];
		reference = *reference.adjacent[3];
	}
	reference = start;

	boundary = start.sight(2);
	for (int i = 0; i < boundary; i++)
	{
		seeableMap[31 + (i + 1)] = *reference.adjacent[2];
		reference = *reference.adjacent[2];
	}
	reference = start;

	boundary = start.sight(0);
	for (int i = 0; i < boundary; i++)
	{
		int boundTwo;
		int newRef = 31 - (9 * (i + 1));
		if (newRef < 0)
			break;

		seeableMap[newRef] = *reference.adjacent[0];
		reference = *reference.adjacent[0];
		Tile refTwo = reference;

		boundTwo = reference.sight(3);
		for (int j = 0; j < boundTwo; j++)
		{
			if (seeableMap[newRef - (j + 1) + 9].type != FAIL)
			{
				seeableMap[newRef - (j + 1)] = *refTwo.adjacent[3];
				refTwo = *refTwo.adjacent[3];
			}
		}
		refTwo = reference;

		boundTwo = reference.sight(2);
		for (int j = 0; j < boundTwo; j++)
		{
			if (seeableMap[newRef + (j + 1) + 9].type != FAIL)
			{
				seeableMap[newRef + (j + 1)] = *refTwo.adjacent[2];
				refTwo = *refTwo.adjacent[2];
			}
		}
	}
	reference = start;

	boundary = start.sight(1);
	for (int i = 0; i < boundary; i++)
	{
		int boundTwo;
		int newRef = 31 + (9 * (i + 1));
		if (newRef > 62)
			break;

		seeableMap[newRef] = *reference.adjacent[1];
		reference = *reference.adjacent[1];
		Tile refTwo = reference;

		boundTwo = reference.sight(3);
		for (int j = 0; j < boundTwo; j++)
		{
			if (seeableMap[newRef - (j + 1) - 9].type != FAIL)
			{
				seeableMap[newRef - (j + 1)] = *refTwo.adjacent[3];
				refTwo = *refTwo.adjacent[3];
			}
		}
		refTwo = reference;

		boundTwo = reference.sight(2);
		for (int j = 0; j < boundTwo; j++)
		{
			if (seeableMap[newRef + (j + 1) - 9].type != FAIL)
			{
				seeableMap[newRef + (j + 1)] = *refTwo.adjacent[2];
				refTwo = *refTwo.adjacent[2];
			}
		}
	}
	/*for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cout << seeableMap[(i * 9) + j].type << " ";
		}
		cout << "\n";
	}*/ //For debugging tile issues

	//minimap(seeableMap);

	drawTiles(seeableMap);
}

//Alters the string so when recalling the map after a battle, changes stay.
string mapUpdate(string map, int locations[], int tileType[]) //WARNING: USE ASCII VALUES
{
	string updatedMap = map;
	int tileNum = 0;
	int realNums[500]{ 0 };
	int counter = 0;

	for (int i = 0; i < map.length(); i++)
	{
		if (updatedMap[i] != 48)
		{
			realNums[counter] = i;
			counter++;
		}
	}
	counter = 0;
	for (int i = 0; i < map.length(); i++)
	{
		if (updatedMap[i] == 50)
		{ 
			updatedMap[i] = 49;
		}
		if (updatedMap[i] == 51 || updatedMap[i] == 52 || updatedMap[i] == 54 || updatedMap[i] == 55 || updatedMap[i] == 56 || updatedMap[i] == 58)
		{
			//All of these are the ones without extra data and/or variable locations.
			switch (tileType[counter])
			{
			default:
				i++;
				break;
			case 2:
				break;
			case 3:
				break;
			case 4: //An enemy!
			{
				if (locations[counter] != -1)
				{
					i++;
					cout << "Good for you!\n";
				}
				else
				{
					updatedMap[i] = 47;
					for (int j = i + 1; j < map.length() - 1; j++)
						updatedMap[j] = updatedMap[j + 1];
				}
				break;
			}
			}
			counter++;
		}
		if (updatedMap[i] != 48) //For keeping track of the actual tile numbers, for locations
		{
			tileNum++;
		}

	}
	return updatedMap;
}

//Creates the map array, and then creates a new map or loads an old one
Tile *loadMap(int mapStart, bool mapit, int mapLoc, int feed1[], int feed2[])
{
	Tile map[500];
	string testMap = mapGeneration(mapStart);
	if (mapLoc != -1)
	{
		testMap = mapUpdate(testMap, feed1, feed2);
	}
	Tile start = constructMap(testMap, map, mapLoc);
	if (mapit == true)
		minimap(map);
	return &start;
}
