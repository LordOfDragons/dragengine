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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionParser.h"
#include "igdeTriggerExpressionParserState.h"
#include "igdeTriggerExpressionComponent.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class igdeTriggerExpressionParser
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionParser::igdeTriggerExpressionParser(){
	pSymbolNegate = '!';
	pSymbolCurState = '@';
	pSymbolAnd = '&';
	pSymbolOr = '|';
	pSymbolGroupStart = '(';
	pSymbolGroupEnd = ')';
	pSymbolQuote = '"';
	
	pExceptionOnErrors = false;
	
	pRequiresQuoteCharacters.AppendCharacter( ' ' );
	pRequiresQuoteCharacters.AppendCharacter( '\t' );
	pRequiresQuoteCharacters.AppendCharacter( pSymbolNegate );
	pRequiresQuoteCharacters.AppendCharacter( pSymbolCurState );
	pRequiresQuoteCharacters.AppendCharacter( pSymbolAnd );
	pRequiresQuoteCharacters.AppendCharacter( pSymbolOr );
	pRequiresQuoteCharacters.AppendCharacter( pSymbolGroupStart );
	pRequiresQuoteCharacters.AppendCharacter( pSymbolGroupEnd );
	pRequiresQuoteCharacters.AppendCharacter( pSymbolQuote );
}

igdeTriggerExpressionParser::~igdeTriggerExpressionParser(){
}



// Management
///////////////

void igdeTriggerExpressionParser::SetExceptionOnErrors( bool exceptionsOnErrors ){
	pExceptionOnErrors = exceptionsOnErrors;
}



igdeTriggerExpression *igdeTriggerExpressionParser::StringToExpression( const char *string ) const{
	igdeTriggerExpressionParserState state( string );
	igdeTriggerExpressionComponent *component = NULL;
	igdeTriggerExpression *expression = NULL;
	
	try{
		expression = new igdeTriggerExpression;
		component = ParseExpressionComponent( state, false, false, false );
		expression->SetRootComponent( component );
		if( component ){
			component->FreeReference();
		}
		
	}catch( const deException & ){
		if( component ){
			component->FreeReference();
		}
		if( expression ){
			expression->FreeReference();
		}
		throw;
	}
	
	return expression;
}

igdeTriggerExpressionComponent *igdeTriggerExpressionParser::ParseExpressionComponent(
igdeTriggerExpressionParserState &state, bool requireEnd, bool initCurState, bool initNegate ) const{
	// modes:
	// 0 => parse trigger/child(0) (negate,curState allowed)
	// 1 => parse logical operator
	// 2 => parse child(1..N) (negate,curState allowed)
	// 3 => parse identical logical operator or end of group or end of expression
	// 4 => exit parser
	// 5 => exit parser due to exception
	deObjectReference refComponent, child;
	bool curState = initCurState;
	bool negate = initNegate;
	decString name;
	int mode = 0;
	
	refComponent.TakeOver( new igdeTriggerExpressionComponent );
	igdeTriggerExpressionComponent &component = ( igdeTriggerExpressionComponent& )( deObject& )refComponent;
	
	while( mode != 4 && mode != 5 && state.HasMoreCharacters() ){
		const int character = state.GetNextCharacter();
		
		if( mode == 2 && ! component.GetTargetName().IsEmpty() ){
			child.TakeOver( new igdeTriggerExpressionComponent );
			igdeTriggerExpressionComponent &c = ( igdeTriggerExpressionComponent& )( deObject& )child;
			c.SetTargetName( component.GetTargetName() );
			c.SetNegate( component.GetNegate() );
			c.SetCurState( component.GetCurState() );
			component.AddChild( &c );
			component.SetTargetName( "" );
			component.SetNegate( false );
			component.SetCurState( false );
		}
		
		if( character == ' ' || character == '\t' ){
			continue;
			
		}else if( character == pSymbolNegate ){
			switch( mode ){
			case 0:
			case 2:
				negate = true;
				break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			
		}else if( character == pSymbolCurState ){
			switch( mode ){
			case 0:
			case 2:
				curState = true;
				break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			
		}else if( character == pSymbolGroupStart ){
			if( curState ){
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
				break;
			}
			
			switch( mode ){
			case 0:{
				child = ParseExpressionComponent( state, true, false, false );
				( ( igdeTriggerExpressionComponent& )( deObject& )child ).SetNegate( negate );
				component.AddChild( ( igdeTriggerExpressionComponent* )( deObject* )child );
				
				negate = false;
				curState = false;
				mode = 1;
				}break;
				
			case 2:
				child = ParseExpressionComponent( state, true, false, false );
				( ( igdeTriggerExpressionComponent& )( deObject& )child ).SetNegate( negate );
				component.AddChild( ( igdeTriggerExpressionComponent* )( deObject* )child );
				
				negate = false;
				curState = false;
				mode = 3;
				break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			
		}else if( character == pSymbolGroupEnd ){
			switch( mode ){
			case 3:
				if( ! requireEnd ){
					state.AdjustPosition( -1 );
				}
				mode = 4;
				break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			break;
			
		}else if( character == pSymbolAnd ){
			switch( mode ){
			case 1:
				component.SetType( igdeTriggerExpressionComponent::ectAnd );
				mode = 2;
				break;
				
			case 3:
				if( component.GetType() != igdeTriggerExpressionComponent::ectAnd ){
					if( pExceptionOnErrors ){
						DETHROW( deeInvalidParam ); // and/or changed inside list
					}
					mode = 5;
					break;
				}
				mode = 2;
				break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			
		}else if( character == pSymbolOr ){
			switch( mode ){
			case 1:
				component.SetType( igdeTriggerExpressionComponent::ectOr );
				mode = 2;
				break;
				
			case 3:
				if( component.GetType() != igdeTriggerExpressionComponent::ectOr ){
					if( pExceptionOnErrors ){
						DETHROW( deeInvalidParam ); // and/or changed inside list
					}
					mode = 5;
					break;
				}
				mode = 2;
				break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			
		}else if( character == pSymbolQuote ){
			switch( mode ){
			case 0:
				ParseTargetName( name, state, true );
				component.SetTargetName( name );
				component.SetNegate( negate );
				component.SetCurState( curState );
				
				negate = false;
				curState = false;
				
				mode = 1;
				break;
				
			case 2:{
				ParseTargetName( name, state, true );
				child.TakeOver( new igdeTriggerExpressionComponent );
				igdeTriggerExpressionComponent &c = ( igdeTriggerExpressionComponent& )( deObject& )child;
				c.SetTargetName( name );
				c.SetNegate( negate );
				c.SetCurState( curState );
				component.AddChild( &c );
				
				negate = false;
				curState = false;
				mode = 3;
				}break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			
		// ( character >= 'a' && character <= 'z' ) || ( character >= 'A' && character <= 'Z' ) || character == '_'
		}else{
			state.AdjustPosition( -1 );
			
			switch( mode ){
			case 0:
				ParseTargetName( name, state, false );
				component.SetTargetName( name.GetString() );
				component.SetNegate( negate );
				component.SetCurState( curState );
				
				negate = false;
				curState = false;
				mode = 1;
				break;
				
			case 2:{
				ParseTargetName( name, state, false );
				child.TakeOver( new igdeTriggerExpressionComponent );
				igdeTriggerExpressionComponent &c = ( igdeTriggerExpressionComponent& )( deObject& )child;
				c.SetTargetName( name );
				c.SetNegate( negate );
				c.SetCurState( curState );
				component.AddChild( &c );
				
				negate = false;
				curState = false;
				mode = 3;
				}break;
				
			default:
				if( pExceptionOnErrors ){
					DETHROW( deeInvalidParam );
				}
				mode = 5;
			}
			
		//}else{
		//	DETHROW( deeInvalidParam );
		}
	}
	
	if( requireEnd && mode != 4 ){
		if( pExceptionOnErrors ){
			DETHROW( deeInvalidParam );
		}
		mode = 5;
	}
	
	if( mode == 0 ){
		return nullptr;
	}
	
	if( mode != 5 ){
		if( mode == 2 ){
			// missing target after and/or. add an empty one to allow editors to work properly
			child.TakeOver( new igdeTriggerExpressionComponent );
			igdeTriggerExpressionComponent &c = ( igdeTriggerExpressionComponent& )( deObject& )child;
			c.SetNegate( negate );
			c.SetCurState( curState );
			component.AddChild( &c );
		}
		
		if( component.GetChildCount() == 1 ){
			const igdeTriggerExpressionComponent &c = *component.GetChildAt( 0 );
			if( c.GetTargetName().IsEmpty() ){
				refComponent = component.GetChildAt( 0 );
				
			}else{
				component.SetTargetName( c.GetTargetName() );
				component.SetNegate( c.GetNegate() );
				component.SetCurState( c.GetCurState() );
				component.RemoveAllChildren();
			}
		}
	}
	
	refComponent->AddReference(); // caller takes over reference
	return ( igdeTriggerExpressionComponent* )( deObject* )refComponent;
}

void igdeTriggerExpressionParser::ParseTargetName( decString &name, igdeTriggerExpressionParserState &state, bool quoted ) const{
	// modes
	// 0 => parse first character
	// 1 => parse other characters
	char character;
	int mode = 0;
	
	name = "";
	
	while( state.HasMoreCharacters() ){
		character = state.GetNextCharacter();
		
		if( quoted ){
			if( character == '\\' ){
				if( ! state.HasMoreCharacters() ){
					DETHROW( deeInvalidParam );
				}
				name.AppendCharacter( state.GetNextCharacter() );
				mode = 1;
				
			}else if( character == pSymbolQuote ){
				if( mode == 0 ){
					DETHROW( deeInvalidParam );
				}
				break;
				
			}else{
				name.AppendCharacter( character );
				mode = 1;
			}
			
		}else{
			if( pRequiresQuoteCharacters.Find( character ) == -1 ){
			/*if( ( character >= 'a' && character <= 'z' ) || ( character >= 'A' && character <= 'Z' )
			|| character == '_' || character == '.' || ( character >= '0' && character <= '9' ) ){*/
				name.AppendCharacter( character );
				mode = 1;
				
			}else{
				if( mode == 0 ){
					DETHROW( deeInvalidParam );
				}
				
				state.AdjustPosition( -1 );
				break;
			}
		}
	}
}

void igdeTriggerExpressionParser::ExpressionToString( decString &string, const igdeTriggerExpression &expression ) const{
	const igdeTriggerExpressionComponent * const component = expression.GetRootComponent();
	
	if( component ){
		ExpressionComponentToString( string, *component, false );
		
	}else{
		string = "";
	}
}

void igdeTriggerExpressionParser::ExpressionComponentToString( decString &string,
const igdeTriggerExpressionComponent &component, bool grouping ) const{
	const decString &targetName = component.GetTargetName();
	const int type = component.GetType();
	decString subString;
	
	string.Empty();
	
	if( component.GetNegate() ){
		string.AppendCharacter( pSymbolNegate );
		grouping = true;
	}
	if( component.GetCurState() && type == igdeTriggerExpressionComponent::ectTarget ){
		string.AppendCharacter( pSymbolCurState );
		grouping = true;
	}
	
	switch( type ){
	case igdeTriggerExpressionComponent::ectTarget:
		QuoteTriggerName( subString, targetName );
		string += subString;
		break;
		
	case igdeTriggerExpressionComponent::ectAnd:{
		const int count = component.GetChildCount();
		int i;
		
		if( grouping ){
			string.AppendCharacter( pSymbolGroupStart );
		}
		for( i=0; i<count; i++ ){
			if( i > 0 ){
				string.AppendCharacter( pSymbolAnd );
			}
			ExpressionComponentToString( subString, *component.GetChildAt( i ), true );
			string += subString;
		}
		if( grouping ){
			string.AppendCharacter( pSymbolGroupEnd );
		}
		
		}break;
		
	case igdeTriggerExpressionComponent::ectOr:{
		const int count = component.GetChildCount();
		int i;
		
		if( grouping ){
			string.AppendCharacter( pSymbolGroupStart );
		}
		for( i=0; i<count; i++ ){
			if( i > 0 ){
				string.AppendCharacter( pSymbolOr );
			}
			ExpressionComponentToString( subString, *component.GetChildAt( i ), true );
			string += subString;
		}
		if( grouping ){
			string.AppendCharacter( pSymbolGroupEnd );
		}
		
		}break;
		
	default:
		break;
	}
}

void igdeTriggerExpressionParser::QuoteTriggerName( decString &string, const decString &name ) const{
	if( name.Find( pRequiresQuoteCharacters ) == -1 ){
		string = name;
		
	}else{
		string.Format( "%c%s%c", pSymbolQuote, name.GetReplacedString( "\"", "\\\"" ).GetString(), pSymbolQuote );
	}
}
