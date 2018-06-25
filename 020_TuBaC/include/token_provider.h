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
#include "variable_token.h"
#include "zero_page_token.h"

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
	std::map<TOKENS, token*> TOKEN_MAP = {
		{ TOKENS::PROGRAM_START,			new token("PROGRAM_START")							},
		{ TOKENS::PROGRAM_END,				new token("PROGRAM_ENDS_HERE")						},
		{ TOKENS::EXPRESSION_STACK,			new token("EXPRESSION_STACK")						},
		{ TOKENS::EXPRESSION_STACK_PTR,		new token("EXPRESSION_STACK_PTR")					},
		{ TOKENS::RETURN_ADDRESS_STACK,		new token("RETURN_ADDRESS_STACK")					},
		{ TOKENS::RETURN_ADDRESS_STACK_PTR,	new token("RETURN_ADDRESS_STACK_PTR")				},
		{ TOKENS::FOR_CONDITION_STACK,		new token("FOR_CONDITION_STACK")					},
		{ TOKENS::FOR_CONDITION_STACK_PTR,	new token("FOR_CONDITION_STACK_PTR")				},
		{ TOKENS::FOR_COUNTER_STACK,		new token("FOR_COUNTER_STACK")						},
		{ TOKENS::FOR_COUNTER_STACK_PTR,	new token("FOR_COUNTER_STACK_PTR")					},
		{ TOKENS::FOR_STEP_STACK,			new token("FOR_STEP_STACK")							},
		{ TOKENS::FOR_STEP_STACK_PTR,		new token("FOR_STEP_STACK_PTR")						},
		{ TOKENS::INTEGER,					new token("INTEGER_")								},
		{ TOKENS::PUSH_POP_PTR_TO_INC_DEC,	new zero_page_token("PUSH_POP_PTR_TO_INC_DEC")		},
		{ TOKENS::PUSH_POP_VALUE_PTR,		new zero_page_token("PUSH_POP_VALUE_PTR")			},
		{ TOKENS::PUSH_POP_TARGET_STACK_PTR,new zero_page_token("PUSH_POP_TARGET_STACK_PTR")	},
		{ TOKENS::LINE_INDICATOR,			new token("LINE_NUMBER_")							},
		{ TOKENS::DATA_LINE_INDICATOR,		new token("DATA_ELEMENTS_LINE_")					},
		{ TOKENS::VARIABLE,					new token("VARIABLE_")								},
		{ TOKENS::AFTER_IF_INDICATOR,		new token("AFTER_IF_")								},
		{ TOKENS::INSIDE_IF_INDICATOR,		new token("INSIDE_IF_")								},
		{ TOKENS::GENERIC_LABEL,			new token("GENERIC_LABEL_")							},
		{ TOKENS::WHILE_INDICATOR,			new token("WHILE_INDICATOR_")						},
		{ TOKENS::AFTER_WHILE_INDICATOR,	new token("AFTER_WHILE_INDICATOR_")					},
		{ TOKENS::REPEAT_INDICATOR,			new token("REPEAT_INDICATOR_")						},
		{ TOKENS::AFTER_REPEAT_INDICATOR,	new token("AFTER_REPEAT_INDICATOR_")				},
		{ TOKENS::DO_INDICATOR,				new token("DO_INDICATOR_")							},
		{ TOKENS::AFTER_DO_INDICATOR,		new token("AFTER_DO_INDICATOR_")					},
		{ TOKENS::PROCEDURE,				new token("PROCEDURE_")								},
		{ TOKENS::INTEGER_ARRAY,			new token("INTEGER_ARRAY_")							},
		{ TOKENS::STRING_LITERAL,			new token("STRING_LITERAL_")						},
		{ TOKENS::STRING_LITERAL_LENGTH,	new token("STRING_LITERAL_LENGTH_")					},
		{ TOKENS::STRING_ARRAY_CAPACITY,	new token("STRING_ARRAY_CAPACITY_")					},
		{ TOKENS::STRING_ARRAY_CURRENT,		new token("STRING_ARRAY_CURRENT_")					},
		{ TOKENS::STRING_ARRAY_CONTENT,		new token("STRING_ARRAY_CONTENT_")					},
		{ TOKENS::STRING_LEFT_BASE,			new zero_page_token("STRING_LEFT_BASE_")			},
		{ TOKENS::STRING_LEFT_FIRST_INDEX,	new variable_token("STRING_LEFT_FIRST_INDEX_")		},
		{ TOKENS::STRING_LEFT_SECOND_INDEX,	new variable_token("STRING_LEFT_SECOND_INDEX_")		},
		{ TOKENS::STRING_LEFT_PTR,			new zero_page_token("STRING_LEFT_PTR")				},
		{ TOKENS::STRING_RIGHT_BASE,		new variable_token("STRING_RIGHT_BASE_")			},
		{ TOKENS::STRING_RIGHT_FIRST_INDEX,	new variable_token("STRING_RIGHT_FIRST_INDEX_")		},
		{ TOKENS::STRING_RIGHT_SECOND_INDEX,new variable_token("STRING_RIGHT_SECOND_INDEX_")	},
		{ TOKENS::STRING_RIGHT_PTR,			new zero_page_token("STRING_RIGHT_PTR")				},
		{ TOKENS::STRING_ASSIGNMENT_COUNTER,new zero_page_token("STRING_ASSIGNMENT_COUNTER")	},
		{ TOKENS::STRING_PRINTED_LENGTH,	new variable_token("STRING_PRINTED_LENGTH")			},
		{ TOKENS::STRING_CMP_LEFT_PTR,		new zero_page_token("STRING_CMP_LEFT_PTR")			},
		{ TOKENS::STRING_CMP_RIGHT_PTR,		new zero_page_token("STRING_CMP_RIGHT_PTR")			},
		{ TOKENS::STRING_CMP_LEFT_LENGTH,	new variable_token("STRING_COMPARISON_LEFT_LENGTH")	},
		{ TOKENS::STRING_CMP_RIGHT_LENGTH,	new variable_token("STRING_COMPARISON_RIGHT_LENGTH")},
		{ TOKENS::CURRENT_DATA_PTR,			new zero_page_token("CURRENT_DATA_PTR")				},
		{ TOKENS::DATA_ELEMENTS_BEGIN,		new token("DATA_ELEMENTS_BEGIN")					}
	};

public:
	const std::string& get(TOKENS token) const;
	auto& get_all_tokens() const { return TOKEN_MAP; };
};
