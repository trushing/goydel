#pragma once

#include <fstream>

struct Controls
{
	struct on_foot
	{
		int forward;
		int back;
		int left;
		int right;
		int jump;
		//int crouch;
		int toggle_edit;
	}foot;

	struct vehicle
	{

	}vehicle;

	struct editor
	{

	}editor;

	bool keyremap; // temp
}



controls;



class InputConfig
{
	std::fstream ctrlmap;
	bool end;
public:
	InputConfig();
	void load_ctrlmap();
	void remap(int&, EKEY_CODE);
	void save_ctrlmap();
}input;

InputConfig::InputConfig()
{
	controls.keyremap = false;
	input.load_ctrlmap();
}

void InputConfig::load_ctrlmap()	// Reads keybinds from file.
{
	end = false;
	ctrlmap.open("config/controls", std::ios::in); // Open for input.

	while(!end && !ctrlmap.fail())	// While input is of expected type.
	{
		ctrlmap >> controls.foot.forward;
		ctrlmap >> controls.foot.back;
		ctrlmap >> controls.foot.left;
		ctrlmap >> controls.foot.right;
		ctrlmap >> controls.foot.jump;
		ctrlmap >> controls.foot.toggle_edit;

		end = true;
	}
	if(ctrlmap.fail())
		std::cout << "error reading from file: controls";

	ctrlmap.close();
}

void InputConfig::remap(int &action, EKEY_CODE bind)	// Binds the given Control struct member to given key. Currently saves immediately.
{
	action = bind;
	input.save_ctrlmap();
	controls.keyremap = false;	// ghetto rigging to fit into Player
}


void InputConfig::save_ctrlmap()	// Writes keybinds to file.
{
	end = false;
	ctrlmap.open("config/controls", std::ios::out); // Open for output.

	while(!end && !ctrlmap.bad())	// While file is readable.
	{
		ctrlmap << controls.foot.forward;
		ctrlmap << "\n" << controls.foot.back;
		ctrlmap << "\n" << controls.foot.left;
		ctrlmap << "\n" << controls.foot.right;
		ctrlmap << "\n" << controls.foot.jump;
		ctrlmap << "\n" << controls.foot.toggle_edit;

		end = true;
	}
	if(ctrlmap.bad())
		std::cout << "error writing to file: controls";

	ctrlmap.close();
}