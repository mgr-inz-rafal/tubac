#pragma once

#include "basic_array.h"

#include <map>

// Represents current compilation context
class context
{
public:
	enum class INPUT_SOURCE
	{
		INPUT,
		READ
	};

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
	int current_line;
	bool invert_logical_value;
	bool is_string_dimensional;
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
	bool last_variable_is_string_;
	COMPARE_TYPE compare_type;
	INPUT_SOURCE input_source;

public:
	void set_input_source(const INPUT_SOURCE input_source_);
	INPUT_SOURCE get_input_source() const;
	int get_current_line() const;
	void set_current_line(int line_);
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
	bool get_last_variable_is_string() const;
	void set_last_variable_is_string(bool b);
	void set_last_string_literal_id(int literal_id);
	int get_last_string_literal_id() const;
	void set_is_string_dimensional(bool b);
	bool get_is_string_dimensional() const;
	bool get_invert_logical_value() const;
	void set_invert_logical_value(const bool invert_logical_value);
};
