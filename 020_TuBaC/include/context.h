#pragma once

#include <string>
#include <map>

struct basic_array
{
	std::string name;
	int size_1;
	int size_2;
	bool two_dimensional;

	void init()
	{
		name = "";
		size_1 = size_2 = 0;
		two_dimensional = false;
	}

	bool is_2d() const
	{
		return two_dimensional;
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

private:
	ARRAY_ASSIGNMENT_SIDE array_assignment_side;
	std::map<ARRAY_ASSIGNMENT_SIDE, basic_array> array_being_declared = {
		{ context::ARRAY_ASSIGNMENT_SIDE::LEFT, {} },
		{ context::ARRAY_ASSIGNMENT_SIDE::RIGHT, {} }};
	std::map<ARRAY_ASSIGNMENT_SIDE, bool> assigning_to_two_dimensional_array;

public:
	basic_array& array_get();
	basic_array& array_get(ARRAY_ASSIGNMENT_SIDE side);
	void array_assignment_side_reset();
	void array_assignment_side_switch_to_right();
};
