
#include "effect_editor.h"
#include "command_window.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "string_constants.h"

using namespace std;
string EffectEditor::larrow(vector<string> args)
{
	if (effect->type > 0) effect->type--;
	else effect->type = EFFECTS_LEN-1;
	resetWindows();
	return "";
}

string EffectEditor::rarrow(vector<string> args)
{
	if (effect->type < EFFECTS_LEN-2) effect->type++;
	else effect->type = 0;
	resetWindows();
	return "";
}
string EffectEditor::exit(vector<string> args)
{
	return STR_EXIT;
}

string EffectEditor::del(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to delete?";
	}
	string delNoun = args[1];
	toLower(&delNoun);
	resetWindows();
	return "";
}

string EffectEditor::set(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to set?";
	}
	if(args.size() < 3) {
		return "Please supply the value directly in the command.";
	}
	string editNoun = args[1];
	toLower(&editNoun);
	
	if(editNoun == STR_MAGNITUDE)
	{
		string magStr = args[2];
		int mag = stoi(magStr,nullptr,10);
		effect->magnitude = mag;
	}
	
	resetWindows();
	return "";
}


string EffectEditor::edit(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to edit?";
	}
	string editNoun = args[1];
	toLower(&editNoun);

	
	clearWindows();
	resetWindows();
	return "";
}

string EffectEditor::add(vector<string> args)
{
	if(args.size() < 2)
	{
		return "What do you want to add?";
	}
	string addNoun = args[1];
	toLower(&addNoun);


	clearWindows();
	resetWindows();
	return "";
}


void EffectEditor::clearWindows()
{
	delwin(commandWindow);
	delwin(responseWindow);
	delwin(mainWindow);
	delwin(headerWindow);
}


void EffectEditor::resetWindows()
{
	commandWindow = newwin(1,getCols(),LINES-1,0);
	keypad(commandWindow,true);
	responseWindow = newwin(1,getCols(),LINES-2,0);
	mainWindow = newwin(LINES-3,getCols(),1,0);
	headerWindow = newwin(1,getCols(),0,0);
	
	getmaxyx(stdscr,h,w); // this doesn't work in windows
	refresh();

	wrefresh(commandWindow);
	wrefresh(responseWindow);
	wrefresh(mainWindow);
	wrefresh(headerWindow);

	string command;

	
	printHeader(headerWindow,effect->parent->parent->getPrimaryName(),effect->parent->getPrimaryName(),"EFFECT:"+effect->getName(),3);
	
	int lineCount = 2;	
	
	setcolor(mainWindow,2,COLOR_WHITE);
	string typeRow = STR_MENU_EFFECT_TYPE + effect->getName();
	mvwprintw(mainWindow,lineCount,0,typeRow.c_str());

	lineCount++;
	string magnitudeRow = STR_MENU_MAGNITUDE + to_string(effect->magnitude);
	mvwprintw(mainWindow,lineCount,0,magnitudeRow.c_str());


	
	wrefresh(mainWindow);
	
}

void EffectEditor::load(DungeonEffect* _effect)
{
	effect = _effect;
	cmdMap[STR_EDIT] = &EffectEditor::edit;
	cmdMap[STR_EXIT] = &EffectEditor::exit;
	cmdMap[STR_ADD] = &EffectEditor::add;
	cmdMap[STR_SET] = &EffectEditor::set;
	cmdMap[STR_DELETE] = &EffectEditor::del;
	cmdMap[STR_PREV] = &EffectEditor::larrow;
	cmdMap[STR_NEXT] = &EffectEditor::rarrow;
	resetWindows();

	CommandWindow cmdW;
	bool cmdFound = false;
	vector<string> cmd;
	while(true) {
		cmd = cmdW.getCommand(commandWindow,STR_PROMPT);
		if(cmd.size() > 0) {
			toLower(&cmd[0]);
			cmdFound = cmdMap.count(cmd[0]) > 0;
		}
		if(!cmdFound) {
			cmd.clear();
			mvwprintw(responseWindow,0,0,"What are you doing, dave?");
			wclrtoeol(responseWindow);
			wrefresh(responseWindow);
		}
		else
		{
			if(cmd[0] == STR_EXIT) break;
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


	clearWindows();
}





