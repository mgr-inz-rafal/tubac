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

#include "token.h"
#include "token_provider.h"

const std::string& token_provider::get(TOKENS token) const
{
	return TOKEN_MAP.at(token).get_label();
}

// TODO: No longer necessary, delegate to "token" class
token token_provider::make_token(const std::string& name, bool b1, bool b2) const
{
	return token(TOKEN_INDICATOR + name, b1, b2);
}

