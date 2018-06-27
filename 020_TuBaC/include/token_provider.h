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
#include <memory>

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
		DATA_ELEMENTS_BEGIN,
		INPUT_BUFFER_SOURCE
	};

private:
	std::map<TOKENS, std::shared_ptr<token>> TOKEN_MAP = {
		{ TOKENS::PROGRAM_START,			std::make_shared<token>("PROGRAM_START")							},
		{ TOKENS::PROGRAM_END,				std::make_shared<token>("PROGRAM_ENDS_HERE")						},
		{ TOKENS::EXPRESSION_STACK,			std::make_shared<token>("EXPRESSION_STACK")							},
		{ TOKENS::EXPRESSION_STACK_PTR,		std::make_shared<token>("EXPRESSION_STACK_PTR")						},
		{ TOKENS::RETURN_ADDRESS_STACK,		std::make_shared<token>("RETURN_ADDRESS_STACK")						},
		{ TOKENS::RETURN_ADDRESS_STACK_PTR,	std::make_shared<token>("RETURN_ADDRESS_STACK_PTR")					},
		{ TOKENS::FOR_CONDITION_STACK,		std::make_shared<token>("FOR_CONDITION_STACK")						},
		{ TOKENS::FOR_CONDITION_STACK_PTR,	std::make_shared<token>("FOR_CONDITION_STACK_PTR")					},
		{ TOKENS::FOR_COUNTER_STACK,		std::make_shared<token>("FOR_COUNTER_STACK")						},
		{ TOKENS::FOR_COUNTER_STACK_PTR,	std::make_shared<token>("FOR_COUNTER_STACK_PTR")					},
		{ TOKENS::FOR_STEP_STACK,			std::make_shared<token>("FOR_STEP_STACK")							},
		{ TOKENS::FOR_STEP_STACK_PTR,		std::make_shared<token>("FOR_STEP_STACK_PTR")						},
		{ TOKENS::INTEGER,					std::make_shared<token>("INTEGER_")									},
		{ TOKENS::PUSH_POP_PTR_TO_INC_DEC,	std::make_shared<zero_page_token>("PUSH_POP_PTR_TO_INC_DEC")		},
		{ TOKENS::PUSH_POP_VALUE_PTR,		std::make_shared<zero_page_token>("PUSH_POP_VALUE_PTR")				},
		{ TOKENS::PUSH_POP_TARGET_STACK_PTR,std::make_shared<zero_page_token>("PUSH_POP_TARGET_STACK_PTR")		},
		{ TOKENS::LINE_INDICATOR,			std::make_shared<token>("LINE_NUMBER_")								},
		{ TOKENS::DATA_LINE_INDICATOR,		std::make_shared<token>("DATA_ELEMENTS_LINE_")						},
		{ TOKENS::VARIABLE,					std::make_shared<token>("VARIABLE_")								},
		{ TOKENS::AFTER_IF_INDICATOR,		std::make_shared<token>("AFTER_IF_")								},
		{ TOKENS::INSIDE_IF_INDICATOR,		std::make_shared<token>("INSIDE_IF_")								},
		{ TOKENS::GENERIC_LABEL,			std::make_shared<token>("GENERIC_LABEL_")							},
		{ TOKENS::WHILE_INDICATOR,			std::make_shared<token>("WHILE_INDICATOR_")							},
		{ TOKENS::AFTER_WHILE_INDICATOR,	std::make_shared<token>("AFTER_WHILE_INDICATOR_")					},
		{ TOKENS::REPEAT_INDICATOR,			std::make_shared<token>("REPEAT_INDICATOR_")						},
		{ TOKENS::AFTER_REPEAT_INDICATOR,	std::make_shared<token>("AFTER_REPEAT_INDICATOR_")					},
		{ TOKENS::DO_INDICATOR,				std::make_shared<token>("DO_INDICATOR_")							},
		{ TOKENS::AFTER_DO_INDICATOR,		std::make_shared<token>("AFTER_DO_INDICATOR_")						},
		{ TOKENS::PROCEDURE,				std::make_shared<token>("PROCEDURE_")								},
		{ TOKENS::INTEGER_ARRAY,			std::make_shared<token>("INTEGER_ARRAY_")							},
		{ TOKENS::STRING_LITERAL,			std::make_shared<token>("STRING_LITERAL_")							},
		{ TOKENS::STRING_LITERAL_LENGTH,	std::make_shared<token>("STRING_LITERAL_LENGTH_")					},
		{ TOKENS::STRING_ARRAY_CAPACITY,	std::make_shared<token>("STRING_ARRAY_CAPACITY_")					},
		{ TOKENS::STRING_ARRAY_CURRENT,		std::make_shared<token>("STRING_ARRAY_CURRENT_")					},
		{ TOKENS::STRING_ARRAY_CONTENT,		std::make_shared<token>("STRING_ARRAY_CONTENT_")					},
		{ TOKENS::STRING_LEFT_BASE,			std::make_shared<zero_page_token>("STRING_LEFT_BASE_")				},
		{ TOKENS::STRING_LEFT_FIRST_INDEX,	std::make_shared<variable_token>("STRING_LEFT_FIRST_INDEX_")		},
		{ TOKENS::STRING_LEFT_SECOND_INDEX,	std::make_shared<variable_token>("STRING_LEFT_SECOND_INDEX_")		},
		{ TOKENS::STRING_LEFT_PTR,			std::make_shared<zero_page_token>("STRING_LEFT_PTR")				},
		{ TOKENS::STRING_RIGHT_BASE,		std::make_shared<variable_token>("STRING_RIGHT_BASE_")				},
		{ TOKENS::STRING_RIGHT_FIRST_INDEX,	std::make_shared<variable_token>("STRING_RIGHT_FIRST_INDEX_")		},
		{ TOKENS::STRING_RIGHT_SECOND_INDEX,std::make_shared<variable_token>("STRING_RIGHT_SECOND_INDEX_")		},
		{ TOKENS::STRING_RIGHT_PTR,			std::make_shared<zero_page_token>("STRING_RIGHT_PTR")				},
		{ TOKENS::STRING_ASSIGNMENT_COUNTER,std::make_shared<zero_page_token>("STRING_ASSIGNMENT_COUNTER")		},
		{ TOKENS::STRING_PRINTED_LENGTH,	std::make_shared<variable_token>("STRING_PRINTED_LENGTH")			},
		{ TOKENS::STRING_CMP_LEFT_PTR,		std::make_shared<zero_page_token>("STRING_CMP_LEFT_PTR")			},
		{ TOKENS::STRING_CMP_RIGHT_PTR,		std::make_shared<zero_page_token>("STRING_CMP_RIGHT_PTR")			},
		{ TOKENS::STRING_CMP_LEFT_LENGTH,	std::make_shared<variable_token>("STRING_COMPARISON_LEFT_LENGTH")	},
		{ TOKENS::STRING_CMP_RIGHT_LENGTH,	std::make_shared<variable_token>("STRING_COMPARISON_RIGHT_LENGTH")	},
		{ TOKENS::CURRENT_DATA_PTR,			std::make_shared<zero_page_token>("CURRENT_DATA_PTR")				},
		{ TOKENS::DATA_ELEMENTS_BEGIN,		std::make_shared<token>("DATA_ELEMENTS_BEGIN")						},
		{ TOKENS::INPUT_BUFFER_SOURCE,		std::make_shared<variable_token>("INPUT_BUFFER_SOURCE")				}
	};

public:
	const std::string& get(TOKENS token) const;
	auto& get_all_tokens() const { return TOKEN_MAP; };
};
