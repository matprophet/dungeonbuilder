#include "headers/JSON_loader.h"

using namespace std;
JSONLoader::JSONLoader(string filename)
{
	char ch = ' ';
	filename.append(".json");
	fin.open(filename.c_str());
	if (fin)
		open = true;
	else open = false;
}
JSONLoader::~JSONLoader()
{
	fin.close();
}

vector<DungeonRoom*> JSONLoader::loadMap()
{
	vector<DungeonRoom*> roomList;
	while (getJSONEntry())
	{
		if (currEntry[0] == "rooms")
			roomList.push_back(loadRoom(roomList));
		fin >> ch;
	}

	return roomList;
}

DungeonRoom* JSONLoader::loadRoom( vector<DungeonRoom*> roomList)
{
	DungeonRoom* room = new DungeonRoom;
	do
	{
		if (currEntry[0] == "uid")
			room->uid = atoi(currEntry[1].c_str());
		else if (currEntry[0] == "name")
			room->name = currEntry[1];
		else if (currEntry[0] == "description")
			room->description = currEntry[1];
		else if (currEntry[0] == "objects")
			while( getJSONEntry() )
				room->objects.push_back(loadObject());
		else if (currEntry[0] == "creatures")
			while (getJSONEntry())
				room->creatures.push_back(loadCreature());
		else if (currEntry[0]=="exits")
			while (getJSONEntry())
				room->exits.push_back(loadExit(roomList));
	} while (ch != '}' && getJSONEntry());

	return room;
}

DungeonObject* JSONLoader::loadObject()
{
	DungeonObject* object = new DungeonObject;

	do
	{
		if (currEntry[0] == "name")
			object->name = currEntry[1];
		else if (currEntry[0] == "description")
			object->description = currEntry[1];
	} while (ch != '}' && getJSONEntry());
	
	return object;
}

DungeonCreature* JSONLoader::loadCreature()
{
	DungeonCreature* creature = new DungeonCreature;

	do
	{
		if (currEntry[0] == "name")
			creature->name = currEntry[1];
		else if (currEntry[0] == "description")
			creature->description = currEntry[1];
		else if (currEntry[0] == "hitpoints")
			creature->hitpoints = atoi(currEntry[1].c_str());
		else if (currEntry[0] == "alignment")
			creature->alignment = atoi(currEntry[1].c_str());
	} while (ch != '}' && getJSONEntry());

	return creature;
}

DungeonExit* JSONLoader::loadExit( vector<DungeonRoom*> roomList)
{
	DungeonExit* exit = new DungeonExit;

	do
	{
		if (currEntry[0] == "Name")
			exit->name = currEntry[1];
		else if (currEntry[0] == "Description")
			exit->description = currEntry[1];
		else if (currEntry[0] == "links")
			exit->room = roomList[atoi(currEntry[1].c_str())];
	} while (ch != '}' && getJSONEntry());

	return exit;
}
/*
[split] -- Splits entry string on ':' and removes quotes from entry
@param string entryString - a string of a full JSON entity 
*/
void JSONLoader::split(string entryString)
{
	int i = 0;
	while (entryString[i] != ':')
	{
		if(entryString[i] != '\"')
			currEntry[0].push_back(entryString[i]);
		++i;
	}
	i += 1;
	if (entryString[i] == '\"')
	{
		++i;
		while (entryString[i] != '\"')
		{
			currEntry[1].push_back(entryString[i]);
			++i;
		}
	}
	else
	{
		while ( i < entryString.length() )
		{
			currEntry[1].push_back(entryString[i]);
			++i;
		}
	}
}

 /*
 [getJSONLoader]
 @param fin    ifstream& - input file object which is used for whole json file reading
 @param ch     char&     - charachter which stores current char in file
 @return  bool  returns true if there was an entry before end of current brackets
 */
bool JSONLoader::getJSONEntry() 
{	
	string curr;
	if (ch == '}' || ch == ']')
		fin >> ch;
	while (ch != '\"' && (ch != '}' && ch != ']'))
		fin >> ch;
	curr.push_back(ch);

	char prev = ch;
	if (ch != '}' && ch != ']')
	{
		bool inQuote = true;
		while (inQuote || (ch != ',' && ch != '}' && ch != '['))
		{
			fin >> noskipws >> ch;
			if (ch == '\"' && prev != '\\')
				inQuote = inQuote ? false : true;
			if (inQuote || ch != ' ')
				curr.push_back(ch);

			prev = ch;
		}
		currEntry[0] = currEntry[1] = "";
		split(curr);
		return true;
	}

	else return false;
}