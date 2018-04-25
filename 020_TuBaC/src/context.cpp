#include "context.h"

void context::array_assignment_side_reset()
{
	array_assignment_side = ARRAY_ASSIGNMENT_SIDE::LEFT;
}

void context::array_assignment_side_switch_to_right()
{
	array_assignment_side = ARRAY_ASSIGNMENT_SIDE::RIGHT;
}

bool context::get_invert_logical_value() const
{
	return invert_logical_value;
}

void context::set_invert_logical_value(const bool invert_logical_value)
{
	this->invert_logical_value = invert_logical_value;
}

context::COMPARE_TYPE context::get_compare_type() const
{
	return compare_type;
}

void context::set_compare_type(const COMPARE_TYPE compare_type)
{
	this->compare_type = compare_type;
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

context::ARRAY_ASSIGNMENT_SIDE context::get_string_assignment_array_side() const
{
	return string_array_assignment_side;
}
