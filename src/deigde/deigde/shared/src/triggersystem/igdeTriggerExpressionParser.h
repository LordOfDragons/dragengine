/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDETRIGGEREXPRESSIONPARSER_H_
#define _IGDETRIGGEREXPRESSIONPARSER_H_

#include <dragengine/common/string/decString.h>

class igdeTriggerExpression;
class igdeTriggerExpressionComponent;
class igdeTriggerExpressionParserState;



/**
 * \brief Trigger Expression Parser.
 */
class igdeTriggerExpressionParser{
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
	/** \brief Determines if an exception is thrown on errors or if the parse continues as good as possible. */
	inline bool GetExceptionOnErrors() const{ return pExceptionOnErrors; }
	
	/** \brief Sets if an exception is thrown on errors or if the parse continues as good as possible. */
	void SetExceptionOnErrors( bool exceptionsOnErrors );
	
	/** \brief Create trigger expression from a string. */
	igdeTriggerExpression *StringToExpression( const char *string ) const;
	
	/** \brief Parse an expression component. */
	igdeTriggerExpressionComponent *ParseExpressionComponent( igdeTriggerExpressionParserState &state, bool requireEnd, bool initCurState, bool initNegate ) const;
	
	/** \brief Parse trigger name. */
	void ParseTargetName( decString &name, igdeTriggerExpressionParserState &state, bool quoted ) const;
	
	/** \brief Create string from a trigger expression. */
	void ExpressionToString( decString &string, const igdeTriggerExpression &expression ) const;
	
	/** \brief Create string from a trigger expression component. */
	void ExpressionComponentToString( decString &string, const igdeTriggerExpressionComponent &component, bool grouping ) const;
	
	/** \brief Retrieves a properly quoted version of a trigger name. */
	void QuoteTriggerName( decString &string, const decString &name ) const;
	/*@}*/
};

#endif
