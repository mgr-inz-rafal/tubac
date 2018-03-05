#pragma once

#include "basic_array.h"

#include <map>

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
	int last_string_literal_id_;

public:
	basic_array& array_get();
	basic_array& array_get(ARRAY_ASSIGNMENT_SIDE side);
	void array_assignment_side_reset();
	void array_assignment_side_switch_to_right();
	void set_last_string_literal_id(int literal_id);
	int get_last_string_literal_id() const;
};
