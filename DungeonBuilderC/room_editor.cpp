
#include "headers/room_editor.h"

using namespace std;

string RoomEditor::exit(vector<string> args)
{
	return "exit";
}

string RoomEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	if(args.size() < 3) {
		return "Please supply the name directly in the command";
	}
	string editNoun = args[1];
	toLower(&editNoun);
	if(editNoun == "name")
	{
		string newname = join(2,args," ");
		room->name = newname;
		clearWindows();
		resetWindows();
		return "";
	}
	else if(editNoun == "desc" || editNoun == "description")
	{
		string desc = join(2,args," ");
		room->description = desc;
		clearWindows();
		resetWindows();
		return "";
	}
	else {
		return "I don't know how to set that";
	}
}

string RoomEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	if(editNoun == "name")
	{
		return set(args);
	}
	else if(editNoun == "description" || editNoun == "desc")
	{
		TextEditor ed;
		clearWindows();
		string newdesc = ed.edit("Editing Description For Room:"+room->name,room->description);
		room->description = newdesc;
		resetWindows();
		return "";
	}
	else
	{
		return "I don't know how to edit that";
	}

}

string RoomEditor::create(vector<string> args)
{
	if(args.size() < 2) {
		return "What do you want to create?";
	}

	if(args.size() < 3)
	{
		return "Provide name for the " + args[1];
	}
	string createNoun = args[1];

	toLower(&createNoun);


	if(createNoun == "creature")
	{
		CreatureEditor editor;
		DungeonCreature* creature = new DungeonCreature();
		creature->name = join(2,args," ");
		clearWindows();
		editor.load(creature);
		room->creatures.push_back(creature);
		resetWindows();
		return "";
	}
	else if(createNoun == "object")
	{
		ObjectEditor oe;
		DungeonObject* o = new DungeonObject();
		o->name = join(2,args," ");
		clearWindows();
		oe.load(o);
		room->objects.push_back(o);
		resetWindows();

		return "";
	}
	else if(createNoun == "exit")
	{
		ExitEditor editor;
		DungeonExit * e = new DungeonExit();
		e->room = g_startRoom;
		e->name = join(2,args," ");
		clearWindows();
		DungeonRoom* newRoom = editor.load(e,room);
		room->exits.push_back(e);
		if(newRoom != NULL)
		{
			room = newRoom;
		}
		resetWindows();
		return "";
	}
	else {
		return "I don't know how to create that";
	}
}


void RoomEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	clear();
}

void RoomEditor::resetWindows()
{
	commandWindow = newwin(1,COLS,LINES-1,0);
	responseWindow = newwin(1,COLS,LINES-2,0);
	mainWindow = newwin(LINES-2,COLS-8,0,4);
	scrollok(mainWindow,TRUE);
	getmaxyx(stdscr,h,w); // this doesn't work in windows

	int lineCount = 3;

	
	setcolors(mainWindow,1,COLOR_RED,COLOR_BLACK);
	mvwprintwCenterBold(mainWindow,1,"Room Editor");
	setcolor(mainWindow,2,COLOR_WHITE);
	string nameRow = "[Set](Name):" + room->name;
	mvwprintw(mainWindow,lineCount,0,nameRow.c_str());

	lineCount++;
	string descRow = "[Set/Edit](Desc) " + room->description;
	mvwprintw(mainWindow,lineCount,0,descRow.c_str());

	lineCount++;
	lineCount++;
	mvwprintw(mainWindow,lineCount,0,"[Create/Delete/Edit](Object):");
	for(auto i= 0u; i < room->objects.size(); i++)
	{
		lineCount++;
		string row = room->objects[i]->name;
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	lineCount++;
	mvwprintw(mainWindow,lineCount,0,"[Create/Delete/Edit](Creatures):");
	for(auto i=0u; i < room->creatures.size(); i++)
	{
		lineCount++;
		string row = room->creatures[i]->name;
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	lineCount++;
	mvwprintw(mainWindow,lineCount,0,"[Create/Delete/Edit](Exits): ");
	for(auto i=0u; i < room->exits.size(); i++)
	{
		lineCount++;
		string row = room->exits[i]->name + "->" + room->exits[i]->room->name;
		mvwprintw(mainWindow,lineCount,2,row.c_str());
	}

	refresh();
	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);

}

void RoomEditor::load(DungeonRoom *_room)
{
	room = _room;
	cmdMap["edit"] = &RoomEditor::edit;
	cmdMap["set"] = &RoomEditor::set;
	cmdMap["exit"] = &RoomEditor::exit;
	cmdMap["create"] = &RoomEditor::create;

	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.command(commandWindow,":");
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;

			if(!cmdFound) {
				cmd.clear();
				mvwprintw(responseWindow,0,0,"What are you doing, dave?");
				wclrtoeol(responseWindow);
				wrefresh(responseWindow);
			}
			else
			{
				if(cmd[0] == "exit") break;
				commandFunction cmdFunc = cmdMap[cmd[0]];
				string response = (this->*cmdFunc)(cmd);
				if(response.length() > 0) {
					cmd.clear();
					mvwprintw(responseWindow,0,0,response.c_str());
					wclrtoeol(responseWindow);
					wrefresh(responseWindow);
				}
			}
		}
	}


	clearWindows();
}





