#pragma once

#include <string>
#include <map>

struct basic_array
{
	std::string name;
	int size_1;
	int size_2;

	void init()
	{
		name = "";
		size_1 = size_2 = 0;
	}
};

// Represents current compilation context
class context
{
public:
	enum class ARRAY_ASSIGNMENT_SIDE
	{
		LEFT,
		RIGHT
	};

public:
	ARRAY_ASSIGNMENT_SIDE array_assignment_side;	// TODO: Privatize
	std::map<ARRAY_ASSIGNMENT_SIDE, basic_array> array_being_declared;
	std::map<ARRAY_ASSIGNMENT_SIDE, bool> assigning_to_two_dimensional_array;

	void array_assignment_side_reset();
	void array_assignment_side_switch_to_right();
	bool array_assignment_side_is_right() const;

	//int left_side;	// 0 - left, 1 - right
};
