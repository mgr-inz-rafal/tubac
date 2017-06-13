#include "context.h"

void context::array_assignment_side_reset()
{
	array_assignment_side = ARRAY_ASSIGNMENT_SIDE::LEFT;
}

void context::array_assignment_side_switch_to_right()
{
	array_assignment_side = ARRAY_ASSIGNMENT_SIDE::RIGHT;
}

bool context::array_assignment_side_is_right() const
{
	return ARRAY_ASSIGNMENT_SIDE::RIGHT == array_assignment_side;
}

basic_array& context::array_get()
{
	return array_being_declared.at(array_assignment_side);
}

basic_array& context::array_get(ARRAY_ASSIGNMENT_SIDE side)
{
	return array_being_declared.at(side);
}
