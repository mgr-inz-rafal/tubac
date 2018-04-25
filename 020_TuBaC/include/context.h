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

	// Only three types, since GREATER_EQUAL is handled by LESS and so on...
	enum class COMPARE_TYPE
	{
		LESS,
		GREATER,
		EQUAL
	};

private:
	bool invert_logical_value;
	ARRAY_ASSIGNMENT_SIDE array_assignment_side;
	ARRAY_ASSIGNMENT_SIDE string_array_assignment_side;
	// TODO: Combine into std::array<T, 2> and remove duplicated methods that differs only by "string_" prefix
	std::map<ARRAY_ASSIGNMENT_SIDE, basic_array> array_being_declared = {
		{ context::ARRAY_ASSIGNMENT_SIDE::LEFT, {} },
		{ context::ARRAY_ASSIGNMENT_SIDE::RIGHT, {} }};
	std::map<ARRAY_ASSIGNMENT_SIDE, basic_array> string_array_being_declared = {
		{ context::ARRAY_ASSIGNMENT_SIDE::LEFT, {} },
		{ context::ARRAY_ASSIGNMENT_SIDE::RIGHT, {} }};
	int last_string_literal_id_;
	COMPARE_TYPE compare_type;

public:
	COMPARE_TYPE get_compare_type() const;
	void set_compare_type(const COMPARE_TYPE compare_type);
	basic_array& array_get();
	basic_array& string_array_get();
	basic_array& array_get(ARRAY_ASSIGNMENT_SIDE side);
	basic_array& string_array_get(ARRAY_ASSIGNMENT_SIDE side);
	void array_assignment_side_reset();
	void string_array_assignment_side_reset();
	void array_assignment_side_switch_to_right();
	void string_array_assignment_side_switch_to_right();
	ARRAY_ASSIGNMENT_SIDE get_string_assignment_array_side() const;

	void set_last_string_literal_id(int literal_id);
	int get_last_string_literal_id() const;

	bool get_invert_logical_value() const;
	void set_invert_logical_value(const bool invert_logical_value);
};
