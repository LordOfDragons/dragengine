/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDETRIGGEREXPRESSIONPARSER_H_
#define _IGDETRIGGEREXPRESSIONPARSER_H_

#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionComponent.h"

#include <dragengine/common/string/decString.h>

class igdeTriggerExpressionParserState;



/**
 * \brief Trigger Expression Parser.
 */
class DE_DLL_EXPORT igdeTriggerExpressionParser{
private:
	char pSymbolNegate;
	char pSymbolCurState;
	char pSymbolAnd;
	char pSymbolOr;
	char pSymbolGroupStart;
	char pSymbolGroupEnd;
	char pSymbolQuote;
	bool pExceptionOnErrors;
	decString pRequiresQuoteCharacters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create trigger expression. */
	igdeTriggerExpressionParser();
	
	/** \brief Clean up trigger expression. */
	~igdeTriggerExpressionParser();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Throws exception on error otherwise returns partially parsed tree. */
	inline bool GetExceptionOnErrors() const{ return pExceptionOnErrors; }
	
	/** \brief Set to throws exception on error otherwise returns partially parsed tree. */
	void SetExceptionOnErrors(bool exceptionsOnErrors);
	
	/** \brief Create trigger expression from a string. */
	igdeTriggerExpression::Ref StringToExpression(const char *string) const;
	
	/** \brief Parse an expression component. */
	igdeTriggerExpressionComponent::Ref ParseExpressionComponent(
		igdeTriggerExpressionParserState &state, bool requireEnd, bool initCurState, bool initNegate) const;
	
	/** \brief Parse trigger name. */
	void ParseTargetName(decString &name, igdeTriggerExpressionParserState &state, bool quoted) const;
	
	/** \brief Create string from a trigger expression. */
	void ExpressionToString(decString &string, const igdeTriggerExpression &expression) const;
	
	/** \brief Create string from a trigger expression component. */
	void ExpressionComponentToString(decString &string, const igdeTriggerExpressionComponent &component, bool grouping) const;
	
	/** \brief Retrieves a properly quoted version of a trigger name. */
	void QuoteTriggerName(decString &string, const decString &name) const;
	/*@}*/
};

#endif
