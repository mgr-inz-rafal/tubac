/*
 *
 * Turbo Basic Compiler by mgr_inz_rafal.
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <rchabowski@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 *														// mgr inz. Rafal
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <string>
#include <map>

#include "token.h"

class token_provider
{
public:
	enum class TOKENS
	{
		PROGRAM_START,
		PROGRAM_END,
		EXPRESSION_STACK,
		EXPRESSION_STACK_PTR,
		RETURN_ADDRESS_STACK,
		RETURN_ADDRESS_STACK_PTR,
		FOR_CONDITION_STACK,
		FOR_CONDITION_STACK_PTR,
		FOR_COUNTER_STACK,
		FOR_COUNTER_STACK_PTR,
		FOR_STEP_STACK,
		FOR_STEP_STACK_PTR,
		INTEGER,
		PUSH_POP_PTR_TO_INC_DEC,
		PUSH_POP_VALUE_PTR,
		PUSH_POP_TARGET_STACK_PTR,
		LINE_INDICATOR,
		DATA_LINE_INDICATOR,
		VARIABLE,
		AFTER_IF_INDICATOR,
		INSIDE_IF_INDICATOR,
		GENERIC_LABEL,
		WHILE_INDICATOR,
		AFTER_WHILE_INDICATOR,
		REPEAT_INDICATOR,
		AFTER_REPEAT_INDICATOR,
		DO_INDICATOR,
		AFTER_DO_INDICATOR,
		PROCEDURE,
		INTEGER_ARRAY,
		STRING_LITERAL,
		STRING_LITERAL_LENGTH,
		STRING_ARRAY_CAPACITY,
		STRING_ARRAY_CURRENT,
		STRING_ARRAY_CONTENT,
		STRING_LEFT_BASE,
		STRING_LEFT_FIRST_INDEX,
		STRING_LEFT_SECOND_INDEX,
		STRING_RIGHT_BASE,
		STRING_RIGHT_FIRST_INDEX,
		STRING_RIGHT_SECOND_INDEX,
		STRING_LEFT_PTR,
		STRING_RIGHT_PTR,
		STRING_ASSIGNMENT_COUNTER,
		STRING_PRINTED_LENGTH,
		STRING_CMP_LEFT_PTR,
		STRING_CMP_RIGHT_PTR,
		STRING_CMP_LEFT_LENGTH,
		STRING_CMP_RIGHT_LENGTH,
		CURRENT_DATA_PTR,
		DATA_ELEMENTS_BEGIN
	};

private:
	const std::string TOKEN_INDICATOR = "___TUBAC___";
	// TODO: Introduce ZeroPageToken: public Token
	std::map<TOKENS, token> TOKEN_MAP = {
		{ TOKENS::PROGRAM_START,			make_token("PROGRAM_START") },
		{ TOKENS::PROGRAM_END,				make_token("PROGRAM_ENDS_HERE") },
		{ TOKENS::EXPRESSION_STACK,			make_token("EXPRESSION_STACK") },
		{ TOKENS::EXPRESSION_STACK_PTR,		make_token("EXPRESSION_STACK_PTR") },
		{ TOKENS::RETURN_ADDRESS_STACK,		make_token("RETURN_ADDRESS_STACK") },
		{ TOKENS::RETURN_ADDRESS_STACK_PTR,	make_token("RETURN_ADDRESS_STACK_PTR") },
		{ TOKENS::FOR_CONDITION_STACK,		make_token("FOR_CONDITION_STACK") },
		{ TOKENS::FOR_CONDITION_STACK_PTR,	make_token("FOR_CONDITION_STACK_PTR") },
		{ TOKENS::FOR_COUNTER_STACK,		make_token("FOR_COUNTER_STACK") },
		{ TOKENS::FOR_COUNTER_STACK_PTR,	make_token("FOR_COUNTER_STACK_PTR") },
		{ TOKENS::FOR_STEP_STACK,			make_token("FOR_STEP_STACK") },
		{ TOKENS::FOR_STEP_STACK_PTR,		make_token("FOR_STEP_STACK_PTR") },
		{ TOKENS::INTEGER,					make_token("INTEGER_") },
		{ TOKENS::PUSH_POP_PTR_TO_INC_DEC,	make_token("PUSH_POP_PTR_TO_INC_DEC", true, true) },
		{ TOKENS::PUSH_POP_VALUE_PTR,		make_token("PUSH_POP_VALUE_PTR", true, true) },
		{ TOKENS::PUSH_POP_TARGET_STACK_PTR,make_token("PUSH_POP_TARGET_STACK_PTR", true, true) },
		{ TOKENS::LINE_INDICATOR,			make_token("LINE_NUMBER_") },
		{ TOKENS::DATA_LINE_INDICATOR,		make_token("DATA_ELEMENTS_LINE_") },
		{ TOKENS::VARIABLE,					make_token("VARIABLE_") },
		{ TOKENS::AFTER_IF_INDICATOR,		make_token("AFTER_IF_") },
		{ TOKENS::INSIDE_IF_INDICATOR,		make_token("INSIDE_IF_") },
		{ TOKENS::GENERIC_LABEL,			make_token("GENERIC_LABEL_") },
		{ TOKENS::WHILE_INDICATOR,			make_token("WHILE_INDICATOR_") },
		{ TOKENS::AFTER_WHILE_INDICATOR,	make_token("AFTER_WHILE_INDICATOR_") },
		{ TOKENS::REPEAT_INDICATOR,			make_token("REPEAT_INDICATOR_") },
		{ TOKENS::AFTER_REPEAT_INDICATOR,	make_token("AFTER_REPEAT_INDICATOR_") },
		{ TOKENS::DO_INDICATOR,				make_token("DO_INDICATOR_") },
		{ TOKENS::AFTER_DO_INDICATOR,		make_token("AFTER_DO_INDICATOR_") },
		{ TOKENS::PROCEDURE,				make_token("PROCEDURE_") },
		{ TOKENS::INTEGER_ARRAY,			make_token("INTEGER_ARRAY_") },
		{ TOKENS::STRING_LITERAL,			make_token("STRING_LITERAL_") },
		{ TOKENS::STRING_LITERAL_LENGTH,	make_token("STRING_LITERAL_LENGTH_") },
		{ TOKENS::STRING_ARRAY_CAPACITY,	make_token("STRING_ARRAY_CAPACITY_") },
		{ TOKENS::STRING_ARRAY_CURRENT,		make_token("STRING_ARRAY_CURRENT_") },
		{ TOKENS::STRING_ARRAY_CONTENT,		make_token("STRING_ARRAY_CONTENT_") },
		{ TOKENS::STRING_LEFT_BASE,			make_token("STRING_LEFT_BASE_", true, true) },
		{ TOKENS::STRING_LEFT_FIRST_INDEX,	make_token("STRING_LEFT_FIRST_INDEX_", true) },
		{ TOKENS::STRING_LEFT_SECOND_INDEX,	make_token("STRING_LEFT_SECOND_INDEX_", true) },
		{ TOKENS::STRING_LEFT_PTR,			make_token("STRING_LEFT_PTR", true, true) },
		{ TOKENS::STRING_RIGHT_BASE,		make_token("STRING_RIGHT_BASE_", true) },
		{ TOKENS::STRING_RIGHT_FIRST_INDEX,	make_token("STRING_RIGHT_FIRST_INDEX_", true) },
		{ TOKENS::STRING_RIGHT_SECOND_INDEX,make_token("STRING_RIGHT_SECOND_INDEX_", true) },
		{ TOKENS::STRING_RIGHT_PTR,			make_token("STRING_RIGHT_PTR", true, true) },
		{ TOKENS::STRING_ASSIGNMENT_COUNTER,make_token("STRING_ASSIGNMENT_COUNTER", true, true) },
		{ TOKENS::STRING_PRINTED_LENGTH,	make_token("STRING_PRINTED_LENGTH", true) },
		{ TOKENS::STRING_CMP_LEFT_PTR,		make_token("STRING_CMP_LEFT_PTR", true, true) },
		{ TOKENS::STRING_CMP_RIGHT_PTR,		make_token("STRING_CMP_RIGHT_PTR", true, true) },
		{ TOKENS::STRING_CMP_LEFT_LENGTH,	make_token("STRING_COMPARISON_LEFT_LENGTH", true) },
		{ TOKENS::STRING_CMP_RIGHT_LENGTH,	make_token("STRING_COMPARISON_RIGHT_LENGTH", true) },
		{ TOKENS::CURRENT_DATA_PTR,			make_token("CURRENT_DATA_PTR", true, true) },
		{ TOKENS::DATA_ELEMENTS_BEGIN,		make_token("DATA_ELEMENTS_BEGIN") }
		
	};
	token make_token(const std::string& name, bool b1 = false, bool b2 = false) const;

public:
	const std::string& get(TOKENS token) const;
	auto& get_all_tokens() const { return TOKEN_MAP; };
};
