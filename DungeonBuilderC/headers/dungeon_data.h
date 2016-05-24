#pragma once
#include "utils.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;


struct DungeonExit;
struct DungeonObject;
struct DungeonCreature;

struct DungeonRoom
{
	int uid;
	string name;
	vector<string> description;
	vector<DungeonExit*> exits;
	vector<DungeonObject*> objects;
	vector<DungeonCreature*> creatures;

	string toJSON();

};
struct DungeonExit
{
	string name;
	vector<string> description;	
	DungeonRoom* room;

	string toJSON();
};

enum USE_EFFECT
{
	HEAL,
	DAMAGE,
	TELEPORT,
	TEXT,
	CREATE_OBJECT,
	CREATE_CREATURE,
	DELETE,
	SHRINK,
	GROW,
	LIGHTER,
	HEAVIER,
	CHANGE,
	NONE,
	NOT_ALLOWED
};

struct DungeonObject
{
	string name;
	vector<string> description;
	
	int damage;  //0 if not a weapon
	vector<string> hitMessages;
	vector<string> missMessages;

	int mass;
	int size;
	
	bool container;
	bool open;
	vector<DungeonObject*> contents;	
	string openMessage;
	string closeMessage;
	
	bool takeable;
	string takeMessage;
	string dropMessage;

	vector<string> useAliases;

	USE_EFFECT selfEffect;
	USE_EFFECT creatureEffect;
	USE_EFFECT objectEffect;


	string toJSON();
};

struct DungeonCreature
{
	string name;
	vector<string> description;
	int hitpoints;
	int alignment;

	string toJSON();
};

struct DungeonPlayer
{
	string name;
	vector<string> description;
	int hitpoints;
	int score;
	vector<DungeonObject*> objects;
	DungeonRoom* location;
};