#include "context.h"

void context::array_assignment_side_reset()
{
	array_assignment_side = ARRAY_ASSIGNMENT_SIDE::LEFT;
}

void context::array_assignment_side_switch_to_right()
{
	array_assignment_side = ARRAY_ASSIGNMENT_SIDE::RIGHT;
}

basic_array& context::array_get()
{
	return array_being_declared.at(array_assignment_side);
}

basic_array& context::array_get(ARRAY_ASSIGNMENT_SIDE side)
{
	return array_being_declared.at(side);
}

void context::set_last_string_literal_id(int literal_id)
{
	last_string_literal_id_ = literal_id;
}

int context::get_last_string_literal_id() const
{
	// TODO: Potentially, this could be still uninitialized...
	return last_string_literal_id_;
}

basic_array& context::string_array_get()
{
	return string_array_being_declared.at(string_array_assignment_side);
}

basic_array& context::string_array_get(ARRAY_ASSIGNMENT_SIDE side)
{
	return string_array_being_declared.at(side);
}

void context::string_array_assignment_side_reset()
{
	string_array_assignment_side = ARRAY_ASSIGNMENT_SIDE::LEFT;
}

void context::string_array_assignment_side_switch_to_right()
{
	string_array_assignment_side = ARRAY_ASSIGNMENT_SIDE::RIGHT;
}
