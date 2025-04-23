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

#include <string.h>
#include <stdlib.h>

#include "deoglShaderDefines.h"
#include "deoglShaderPreprocessor.h"
#include "deoglShaderPreprocessorSymbol.h"
#include "deoglShaderSourceLocation.h"
#include "deoglShaderUnitSourceCode.h"
#include "deoglShaderManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderPreprocessor
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderPreprocessor::deoglShaderPreprocessor( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pSources( NULL ),
pSourcesLen( 0 ),
pSourcesSize( 1024 ),
pInputNext( NULL ),
pInputFile( NULL ),
pInputLine( 0 ),
pOutputLine( 1 ),
pOutputCode( true ),
pOutputCodeCase( true ),
pEndDirectiveBlock( false ),
pLastMappedOutputLine( 0 ),
pResolveBuffer( NULL ),
pResolveBufferLen( 0 ),
pResolveBufferSize( 1024 ),
pResolveSymbolName( NULL ),
pResolveSymbolNameLen( 0 ),
pResolveSymbolNameSize( 200 ),

pDisablePreprocessing( false ),
#ifdef WITH_OPENGLES
pDebugLogParsing( false )
#else
pDebugLogParsing( false )
#endif
{
	pSources = ( char* )malloc( pSourcesSize );
	pSources[ 0 ] = '\0';
	
	pResolveBuffer = ( char* )malloc( pResolveBufferSize );
	pResolveBuffer[ 0 ] = '\0';
	
	pResolveSymbolName = ( char* )malloc( pResolveSymbolNameSize );
	pResolveSymbolName[ 0 ] = '\0';
}

deoglShaderPreprocessor::~deoglShaderPreprocessor(){
	if( pSources ){
		free( pSources );
	}
	if( pResolveBuffer ){
		free( pResolveBuffer );
	}
	if( pResolveSymbolName ){
		free( pResolveSymbolName );
	}
}



// Management
///////////////

void deoglShaderPreprocessor::Clear(){
	pSourcesLen = 0;
	pSources[ 0 ] = '\0';
	
	ClearAllSymbols();
	
	pSourceLocations.RemoveAll();
	pInputNext = NULL;
	pInputFile = NULL;
	pInputLine = 1;
	pOutputLine = 1;
	pLastMappedOutputLine = 0;
}

void deoglShaderPreprocessor::LogSourceLocationMap(){
	const int count = pSourceLocations.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglShaderSourceLocation &location = *( ( deoglShaderSourceLocation* )pSourceLocations.GetAt( i ) );
		pRenderThread.GetLogger().LogInfoFormat( "LineMap: %d -> %s:%d", location.GetOutputLine(),
			location.GetInputFile().GetString(), location.GetInputLine() );
	}
}

const deoglShaderSourceLocation *deoglShaderPreprocessor::ResolveSourceLocation( int line ) const{
	const int count = pSourceLocations.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglShaderSourceLocation * const location = ( deoglShaderSourceLocation* )pSourceLocations.GetAt( i );
		if( location->GetOutputLine() == line ){
			return location;
		}
	}
	
	return NULL;
}



// Sources
////////////

void deoglShaderPreprocessor::SourcesAppend( const char *text, bool mapLines ){
	if( ! text ){
		DETHROW( deeInvalidParam );
	}
	SourcesAppend( text, ( int )strlen( text ), mapLines );
}

void deoglShaderPreprocessor::SourcesAppend( const char *text, int length, bool mapLines ){
	if(!pOutputCode){
		if(pDebugLogParsing){
			pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor:"
				" suppress line '%s' at %d (output %d)", text, pInputLine, pOutputCode);
		}
		return; // for exampe due to #if/#endif hiding code
	}
	
	if( ! text || length < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( length == 0 ){
		return;
	}
	
	if(pDebugLogParsing){
		pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor:"
			" sources append (len %d) at %d (output %d)", length, pInputLine, pOutputCode);
	}
	if( pSourcesLen + length > pSourcesSize ){
		const int newSize = pSourcesLen + length + 1024;  // increment by steps of 1k
		char * const newSources = ( char* )realloc( pSources, newSize + 1 );
		if( ! newSources ){
			DETHROW( deeOutOfMemory );
		}
		pSources = newSources;
		pSourcesSize = newSize;
	}
	
	if( pOutputLine != pLastMappedOutputLine ){
		pLastMappedOutputLine = pOutputLine;
		
		if( mapLines && pInputFile ){
			deoglShaderSourceLocation *location = NULL;
			
			try{
				location = new deoglShaderSourceLocation( pInputFile, pInputLine, pOutputLine );
				pSourceLocations.Add( location );
				location->FreeReference();
				
			}catch( const deException & ){
				if( location ){
					location->FreeReference();
				}
				throw;
			}
		}
	}
	
	const int end = pSourcesLen + length;
	while( pSourcesLen != end ){
		if( *text == '\n' ){
			if( pOutputLine != pLastMappedOutputLine ){
				pLastMappedOutputLine = pOutputLine;
				
				if( mapLines && pInputFile ){
					deoglShaderSourceLocation *location = NULL;
					
					try{
						location = new deoglShaderSourceLocation( pInputFile, pInputLine, pOutputLine );
						pSourceLocations.Add( location );
						location->FreeReference();
						
					}catch( const deException & ){
						if( location ){
							location->FreeReference();
						}
						throw;
					}
				}
			}
			
			pOutputLine++;
		}
		
		pSources[ pSourcesLen++ ] = *text++;
	}
	
	pSources[ pSourcesLen ] = '\0';
}

void deoglShaderPreprocessor::SourcesAppendProcessed( const char *sourceCode ){
	SourcesAppendProcessed( sourceCode, NULL );
}

void deoglShaderPreprocessor::SourcesAppendProcessed( const char *sourceCode,
const char *inputFile, bool resetState ){
	if( ! sourceCode ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDebugLogParsing ){
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: process sources from %s",
			inputFile != NULL ? inputFile : "?" );
	}
	
	pInputNext = sourceCode;
	pInputFile = inputFile;
	pInputLine = 1;
	if( resetState ){
		pOutputCode = true;
		pOutputCodeCase = true;
		pEndDirectiveBlock = false;
	}
	
	pProcessSources();
	
	if( *pInputNext ){
		// not at end of string. this should never happen
		pRenderThread.GetLogger().LogErrorFormat(
			"Shader Preprocessor: Not at end of string (Invalid character '%c' at %s:%d",
			*pInputNext, pInputFile != NULL ? pInputFile : "?", pInputLine );
		DETHROW( deeInvalidParam );
	}
	
	pInputNext = NULL;
	pInputFile = NULL;
	pInputLine = 1;
}



// Symbol Table
/////////////////

int deoglShaderPreprocessor::GetSymbolCount() const{
	return pSymbolTable.GetCount();
}

decStringList deoglShaderPreprocessor::GetSymbolNames() const{
	return pSymbolTable.GetKeys();
}

bool deoglShaderPreprocessor::HasSymbolNamed( const char *name ) const{
	return pSymbolTable.Has( name );
}

deoglShaderPreprocessorSymbol *deoglShaderPreprocessor::GetSymbolNamed( const char *name ) const{
	deObject *object = NULL;
	(void)pSymbolTable.GetAt( name, &object );
	return ( deoglShaderPreprocessorSymbol* )object;
}

void deoglShaderPreprocessor::SetSymbol( deoglShaderPreprocessorSymbol *symbol ){
	if( ! symbol ){
		DETHROW( deeInvalidParam );
	}
	pSymbolTable.SetAt( symbol->GetName(), symbol );
}

void deoglShaderPreprocessor::SetSymbol( const char *name, const char *value ){
	deoglShaderPreprocessorSymbol *symbol = NULL;
	
	pResolveString( value, ( int )strlen( value ) );
	
	try{
		symbol = new deoglShaderPreprocessorSymbol( name, pResolveBuffer );
		pSymbolTable.SetAt( name, symbol );
		symbol->FreeReference();
		
	}catch( const deException & ){
		if( symbol ){
			symbol->FreeReference();
		}
		throw;
	}
	
	// HACK HACK HACK
	SourcesAppend( decString( "#define " ) + name + " " + pResolveBuffer + "\n", false );
	// HACK HACK HACK
}

void deoglShaderPreprocessor::ClearSymbol( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	pSymbolTable.RemoveIfPresent( name );
}

void deoglShaderPreprocessor::ClearAllSymbols(){
	pSymbolTable.RemoveAll();
}

void deoglShaderPreprocessor::SetSymbolsFromDefines( const deoglShaderDefines &defines ){
	const int count = defines.GetDefineCount();
	int i;
	
	for( i=0; i<count; i++ ){
		SetSymbol( defines.GetDefineNameAt( i ), defines.GetDefineValueAt( i ) );
	}
}

void deoglShaderPreprocessor::SetDebugLogParsing(bool enable){
	pDebugLogParsing = enable;
}



// Private Functions
//////////////////////

void deoglShaderPreprocessor::pProcessSources(){
	const char *beginLine = pInputNext;
	bool directiveAllowed = true;
	bool emptyLine = true;
	
	if( pDebugLogParsing ){
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor:"
			" process sources at %d (output %d)", pInputLine, pOutputCode );
	}
	
	while( *pInputNext ){
		if(pDebugLogParsing){
			pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor:"
				" next(%c)(%d) at %d (output %d)(begin %d)(empty %d)",
				*pInputNext, *pInputNext, pInputLine, pOutputCode,
				(int)(pInputNext - beginLine), emptyLine);
		}
		if( *pInputNext == '/' && pInputNext[1] == '/' ){
			if( pInputNext != beginLine && ! emptyLine ){
				SourcesAppend( beginLine, ( int )( pInputNext - beginLine ), true );
				pProcessSingleLineComment();
				SourcesAppend( "\n", 1, false );
				
			}else{
				pProcessSingleLineComment();
			}
			
			beginLine = pInputNext;
			directiveAllowed = true;
			emptyLine = true;
			
		}else if( *pInputNext == '/' && pInputNext[1] == '*' ){
			if( pInputNext != beginLine ){
				SourcesAppend( beginLine, ( int )( pInputNext - beginLine ), true );
			}
			
			pProcessMultiLineComment();
			beginLine = pInputNext;
			directiveAllowed = false;
			
		}else if( directiveAllowed && *pInputNext == '#' ){
			pProcessDirective( beginLine );
			if( pEndDirectiveBlock ){
				return;
			}
			beginLine = pInputNext;
			directiveAllowed = true;
			emptyLine = true;
			
		}else if( *pInputNext == ' ' || *pInputNext == '\t' ){
			if(pDebugLogParsing){
				pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor:"
					" whitespace(%d) at %d (output %d)(begin %d)(empty %d)",
					*pInputNext, pInputLine, pOutputCode, (int)(pInputNext - beginLine), emptyLine);
			}
			pInputNext++;
			
		}else if( *pInputNext == '\n' ){
			if(pDebugLogParsing){
				pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor:"
					" newline at %d (output %d)(begin %d)(empty %d)",
					pInputLine, pOutputCode, (int)(pInputNext - beginLine), emptyLine);
			}
			if( pInputNext != beginLine && ! emptyLine ){
				SourcesAppend( beginLine, ( int )( pInputNext - beginLine ) + 1, true );
			}
			pInputNext++;
			pInputLine++;
			beginLine = pInputNext;
			directiveAllowed = true;
			emptyLine = true;
			
		}else{
			if(pDebugLogParsing){
				pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor:"
					" text(%c) at %d (output %d)(begin %d)(empty %d)", *pInputNext,
					pInputLine, pOutputCode, (int)(pInputNext - beginLine), emptyLine);
			}
			pInputNext++;
			directiveAllowed = false;
			emptyLine = false;
		}
	}
	
	if(pDebugLogParsing){
		pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor:"
			" end at %d (output %d)(begin %d)", pInputLine, pOutputCode, (int)(pInputNext - beginLine));
	}
	if( pInputNext != beginLine ){
		SourcesAppend( beginLine, ( int )( pInputNext - beginLine ), true );
	}
}

void deoglShaderPreprocessor::pProcessSingleLineComment(){
	while( *pInputNext ){
		if( *pInputNext == '\\' && pInputNext[1] == '\n' ){
			// line splicing
			pInputNext += 2;
			pInputLine++;
			
		}else if( *pInputNext == '\n' ){
			// end of line
			pInputNext++;
			pInputLine++;
			break;
			
		}else{
			pInputNext++;
		}
	}
}

void deoglShaderPreprocessor::pProcessMultiLineComment(){
	while( *pInputNext ){
		if( *pInputNext == '*' && pInputNext[1] == '/' ){
			// end of comment
			pInputNext += 2;
			break;
			
		}else if( *pInputNext == '\n' ){
			pInputNext++;
			pInputLine++;
			
		}else{
			pInputNext++;
		}
	}
}

void deoglShaderPreprocessor::pProcessDirective( const char *beginLine ){
	const char * const begin = ++pInputNext; // start of directive
	
	while( *pInputNext ){
		if( pIsSymbol( *pInputNext ) ){
			pInputNext++;
			continue;
			
		}else if( pDisablePreprocessing ){
			// only used for debuggin purpose
			if( strncmp( begin, "include", 7 ) == 0 ){
				pProcessDirectiveInclude();
				return;
				
			}else{
				// unknown directive
				break;
			}
			
		}else{
			if( strncmp( begin, "include", 7 ) == 0 ){
				pProcessDirectiveInclude();
				return;
				
			}else if( strncmp( begin, "define", 6 ) == 0 ){
				pProcessDirectiveDefine( beginLine );
				return;
				
			}else if( strncmp( begin, "undef", 5 ) == 0 ){
				pProcessDirectiveUndefine( beginLine );
				return;
				
			}else if( strncmp( begin, "ifdef", 5 ) == 0 ){
				pProcessDirectiveIfDef();
				return;
				
			}else if( strncmp( begin, "ifndef", 6 ) == 0 ){
				pProcessDirectiveIfNotDef();
				return;
				
			}else if( strncmp( begin, "if", 2 ) == 0 ){
				pProcessDirectiveIf();
				return;
				
			}else if( strncmp( begin, "elif", 4 ) == 0 ){
				pProcessDirectiveElseIf();
				return;
				
			}else if( strncmp( begin, "else", 4 ) == 0 ){
				pProcessDirectiveElse();
				return;
				
			}else if( strncmp( begin, "endif", 5 ) == 0 ){
				pProcessDirectiveEndIf();
				return;
				
			}else{
				// unknown directive
				break;
			}
		}
	}
	
	while( *pInputNext && *pInputNext != '\n' ){
		pInputNext++;
	}
	
	SourcesAppend( begin - 1, ( int )( pInputNext - begin ) + 2, true );
	
	// has to be done last or the mapping will be wrong
	pInputNext++;
	pInputLine++;
}

void deoglShaderPreprocessor::pProcessDirectiveInclude(){
	// #include "filename"
	sToken token;
	pExpectDirectiveToken( token, edtString, "include" );
	const decString filename( pDirectiveTokenString( token ) );
	
	pExpectDirectiveToken( edtNewline, "include" );
	
	if( ! pOutputCode ){
		return;
	}
	
	const decString *sources = nullptr;
	if(!pRenderThread.GetShader().GetShaderManager().GetIncludableSources().GetAt(filename, &sources)){
		pRenderThread.GetLogger().LogErrorFormat("Shader Preprocessor: #include: File not found %s at %s:%d",
			filename.GetString(), pInputFile != NULL ? pInputFile : "?", pInputLine);
		DETHROW(deeInvalidParam);
	}
	
	if(pDebugLogParsing){
		pRenderThread.GetLogger().LogInfoFormat("Shader Preprocessor: #include: Include '%s'", filename.GetString());
	}
	
	const char * const oldInputNext = pInputNext;
	const char * const oldInputFile = pInputFile;
	const int oldInputLine = pInputLine;
	
	SourcesAppendProcessed(*sources, filename, false);
	
	pInputNext = oldInputNext;
	pInputFile = oldInputFile;
	pInputLine = oldInputLine + 1;
}

void deoglShaderPreprocessor::pProcessDirectiveDefine( const char *beginLine ){
	// #define symbol {value}
	// #define macro(param1,param2,...) {value}
	sToken token;
	pExpectDirectiveToken( token, edtIdentifier, "define" );
	const decString symbol( pDirectiveTokenString( token ) );
	
	switch( pParseDirectiveToken( token ) ){
	case edtLeftParanthesis:{
		if( pDebugLogParsing ){
			pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #define:"
				" macro '%s' ignored (output %d)", symbol.GetString(), pOutputCode );
		}
		
		// output text for GPU to use too. we take care of removing #if/#endif using symbols
		// and removing line splicing
		const int inputLine = pInputLine;
		decString value;
		value.Set( ' ', ( int )( pInputNext - beginLine ) );
		memcpy( ( char* )value.GetString(), beginLine, ( int )( pInputNext - beginLine ) );
		
		while( true ){
			if( pParseDirectiveAnything( token ) ){
				decString line( pDirectiveTokenString( token ) );
				const int len = line.GetLength();
				
				if( len > 1 && line[ len - 1 ] == '\\' ){
					// line splicing
					line[ len - 1 ] = ' ';
					value += line;
					
				}else{
					value += line;
					break;
				}
			}
		}
		
		value.AppendCharacter( '\n' );
		
		const int nextInputLine = pInputLine;
		pInputLine = inputLine;
		SourcesAppend( value, true );
		pInputLine = nextInputLine;
		}break;
		
	case edtInvalid:
		pErrorInvalidToken( token, "define" );
		return; // never reached since pErrorInvalidToken throws an exception
		
	case edtNewline:
		pInputNext = token.begin;
		pInputLine = token.line;
		if( pDebugLogParsing ){
			pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #define:"
				" symbol '%s' (output %d)", symbol.GetString(), pOutputCode );
		}
		if( ! pOutputCode ){
			return;
		}
		SetSymbol( symbol, "" );
		break;
		
	default:{
		pInputNext = token.begin;
		pInputLine = token.line;
		
		decString value;
		while( true ){
			if( pParseDirectiveAnything( token ) ){
				decString line( pDirectiveTokenString( token ) );
				const int len = line.GetLength();
				
				if( len > 1 && line[ len - 1 ] == '\\' ){
					// line splicing
					line[ len - 1 ] = ' ';
					value += line;
					
				}else{
					value += line;
					break;
				}
			}
		}
		
		if( pDebugLogParsing ){
			pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #define:"
				" symbol '%s' = '%s' (output %d)", symbol.GetString(), value.GetString(), pOutputCode );
		}
		
		if( ! pOutputCode ){
			return;
		}
		
		SetSymbol( symbol, value );
		}
	}
}

void deoglShaderPreprocessor::pProcessDirectiveUndefine( const char *beginLine ){
	// #undef symbol
	sToken token;
	pExpectDirectiveToken( token, edtIdentifier, "undef" );
	const decString symbol( pDirectiveTokenString( token ) );
	
	pExpectDirectiveToken( edtNewline, "undef" );
	
	if( pDebugLogParsing ){
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #undef:"
			" symbol '%s' (output %d)", symbol.GetString(), pOutputCode );
	}
	
	if( ! pOutputCode ){
		return;
	}
	
	ClearSymbol( symbol );
	
	// output text for GPU to use too. we take care of removing #if/#endif using symbols
	pInputLine--;
	SourcesAppend( beginLine, ( int )( pInputNext - beginLine ), true );
	pInputLine++;
}

void deoglShaderPreprocessor::pProcessDirectiveIfDef() {
	// #ifdef symbol
	sToken token;
	pExpectDirectiveToken( token, edtIdentifier, "ifdef" );
	const decString symbol( pDirectiveTokenString( token ) );
	
	pExpectDirectiveToken( edtNewline, "ifdef" );
	
	if( pDebugLogParsing ){
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #ifdef:"
			" symbol '%s' = %d (output %d)", symbol.GetString(),
			HasSymbolNamed( symbol ), pOutputCode );
	}
	
	const bool oldOutputCode = pOutputCode;
	const bool oldOutputCodeCase = pOutputCodeCase;
	
	if( pOutputCode ){
		pOutputCode = HasSymbolNamed( symbol );
		pOutputCodeCase = ! pOutputCode;
		
	}else{
		pOutputCode = false;
		pOutputCodeCase = false;
	}
	
	pProcessSources(); // returns after #endif. can pass by #else and #elif
	
	pEndDirectiveBlock = false;
	pOutputCode = oldOutputCode;
	pOutputCodeCase = oldOutputCodeCase;
}

void deoglShaderPreprocessor::pProcessDirectiveIfNotDef() {
	// #ifndef symbol
	sToken token;
	pExpectDirectiveToken( token, edtIdentifier, "ifndef" );
	const decString symbol( pDirectiveTokenString( token ) );
	
	pExpectDirectiveToken( edtNewline, "ifndef" );
	
	if( pDebugLogParsing ){
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #ifndef:"
			" symbol '%s' = %d (output %d)", symbol.GetString(),
			HasSymbolNamed( symbol ), pOutputCode );
	}
	
	const bool oldOutputCode = pOutputCode;
	const bool oldOutputCodeCase = pOutputCodeCase;
	
	if( pOutputCode ){
		pOutputCode = ! HasSymbolNamed( symbol );
		pOutputCodeCase = ! pOutputCode;
		
	}else{
		pOutputCode = false;
		pOutputCodeCase = false;
	}
	
	pProcessSources(); // returns after #endif. can pass by #else and #elif
	
	pEndDirectiveBlock = false;
	pOutputCode = oldOutputCode;
	pOutputCodeCase = oldOutputCodeCase;
}

void deoglShaderPreprocessor::pProcessDirectiveIf() {
	// #if condition
	const char * const beginCondition = pInputNext;
	const bool result = pProcessDirectiveCondition( "if", false );
	
	if( pDebugLogParsing ){
		pSetResolveSymbolName( beginCondition, ( int )( pInputNext - beginCondition ) - 1 );
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #if:"
			" condition '%s' = %d (output %d)", pResolveSymbolName, result, pOutputCode );
	}
	
	const bool oldOutputCode = pOutputCode;
	const bool oldOutputCodeCase = pOutputCodeCase;
	
	if( pOutputCode ){
		pOutputCode = result;
		pOutputCodeCase = ! result;
		
	}else{
		pOutputCode = false;
		pOutputCodeCase = false;
	}
	
	pProcessSources(); // returns after #endif. can pass by #else and #elif
	
	pEndDirectiveBlock = false;
	pOutputCode = oldOutputCode;
	pOutputCodeCase = oldOutputCodeCase;
}

void deoglShaderPreprocessor::pProcessDirectiveElseIf() {
	// #elif condition
	const char * const beginCondition = pInputNext;
	const bool result = pProcessDirectiveCondition( "elif", false );
	
	if( pDebugLogParsing ){
		pSetResolveSymbolName( beginCondition, ( int )( pInputNext - beginCondition ) - 1 );
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #elif:"
			" condition '%s' = %d (output %d)", pResolveSymbolName, result, pOutputCodeCase );
	}
	
	if( pOutputCodeCase ){
		pOutputCode = result;
		pOutputCodeCase = ! result;
		
	}else{
		pOutputCode = false;
	}
	
	pProcessSources(); // returns after #endif. can pass by #else and #elif
}

void deoglShaderPreprocessor::pProcessDirectiveElse() {
	// #else
	pExpectDirectiveToken( edtNewline, "else" );
	
	if( pDebugLogParsing ){
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #else: (output %d)", pOutputCodeCase );
	}
	
	pOutputCode = pOutputCodeCase;
	pOutputCodeCase = false;
}

bool deoglShaderPreprocessor::pProcessDirectiveCondition( const char *directive, bool groupOpen ){
	// {'!'} defined {'('} symbol {')'}
	// condition1 && condition2
	// condition1 || condition2
	// (condition)
	// number
	// condition1 == number
	sToken token;
	bool result = false;
	bool negate = false;
	bool firstResult = true;
	eDirectiveTokens op = edtInvalid;
	
	while( true ){
		const eDirectiveTokens updateOp = op;
		const bool updateNegate = negate;
		
		bool updateResultValue = false;
		bool updateResult = false;
		
		switch( pParseDirectiveToken( token ) ){
		case edtNumber:
			switch( op ){
			case edtInvalid: // begin of condition
			case edtLogicAnd:
			case edtLogicOr:
			case edtLogicNot:
				updateResultValue = pDirectiveTokenString( token ).ToInt() != 0;
				updateResult = true;
				op = edtIdentifier;
				negate = false;
				break;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		case edtIdentifier:
			switch( op ){
			case edtInvalid: // begin of condition
			case edtLogicAnd:
			case edtLogicOr:
			case edtLogicNot:{
				// WARNING symbol value can contain symbols. it is required to also parse to resolve
				//         the identifier to obtain a correct result
				const decString identifier( pDirectiveTokenString( token ) );
				
				if( identifier == "defined" ){
					switch( pParseDirectiveToken( token ) ){
					case edtIdentifier:
						updateResultValue = HasSymbolNamed( pDirectiveTokenString( token ) );
						break;
						
					case edtLeftParanthesis:
						pExpectDirectiveToken( token, edtIdentifier, directive );
						updateResultValue = HasSymbolNamed( pDirectiveTokenString( token ) );
						pExpectDirectiveToken( edtRightParanthesis, directive );
						break;
						
					default:
						pErrorInvalidToken( token, directive );
						// never gets here since pErrorInvalidToken throws an exception
					}
					
				}else{
					const deoglShaderPreprocessorSymbol * const symbol = GetSymbolNamed( identifier );
					const char * const oldInputNext = pInputNext;
					const int oldInputLine = pInputLine;
					
					switch( pParseDirectiveToken( token ) ){
					case edtEquals:
						pExpectDirectiveToken( token, edtNumber, directive );
						updateResultValue = symbol && symbol->GetValue().ToInt() == pDirectiveTokenString( token ).ToInt();
						break;
						
					case edtNotEquals:
						pExpectDirectiveToken( token, edtNumber, directive );
						updateResultValue = ! symbol || symbol->GetValue().ToInt() != pDirectiveTokenString( token ).ToInt();
						break;
						
					case edtLessThan:
						pExpectDirectiveToken( token, edtNumber, directive );
						updateResultValue = symbol && symbol->GetValue().ToInt() < pDirectiveTokenString( token ).ToInt();
						break;
						
					case edtLessThanOrEquals:
						pExpectDirectiveToken( token, edtNumber, directive );
						updateResultValue = symbol && symbol->GetValue().ToInt() <= pDirectiveTokenString( token ).ToInt();
						break;
						
					case edtGreaterThan:
						pExpectDirectiveToken( token, edtNumber, directive );
						updateResultValue = symbol && symbol->GetValue().ToInt() > pDirectiveTokenString( token ).ToInt();
						break;
						
					case edtGreaterThanOrEquals:
						pExpectDirectiveToken( token, edtNumber, directive );
						updateResultValue = symbol && symbol->GetValue().ToInt() >= pDirectiveTokenString( token ).ToInt();
						break;
						
					default:
						pInputNext = oldInputNext;
						pInputLine = oldInputLine;
						updateResultValue = symbol != NULL;
					}
				}
				
				updateResult = true;
				op = edtIdentifier;
				negate = false;
				}break;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		case edtLineSplicer:
			// line splicer is ignored
			break;
			
		case edtNewline:
			switch( op ){
			case edtInvalid: // begin of line
			case edtIdentifier:
				if( groupOpen ){
					pErrorInvalidToken( token, directive );
					// never gets here since pErrorInvalidToken throws an exception
				}
				return result;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		case edtRightParanthesis:
			switch( op ){
			case edtIdentifier:
				if( ! groupOpen ){
					pErrorInvalidToken( token, directive );
					// never gets here since pErrorInvalidToken throws an exception
				}
				return result;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		case edtLeftParanthesis:
			switch( op ){
			case edtInvalid: // begin of line
			case edtLogicAnd:
			case edtLogicOr:
			case edtLogicNot:
				updateResultValue = pProcessDirectiveCondition( directive, true );
				updateResult = true;
				op = edtIdentifier;
				negate = false;
				break;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		case edtLogicAnd:
			switch( op ){
			case edtIdentifier:
				op = edtLogicAnd;
				negate = false;
				break;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		case edtLogicOr:
			switch( op ){
			case edtIdentifier:
				op = edtLogicOr;
				negate = false;
				break;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		case edtLogicNot:
			switch( op ){
			case edtInvalid: // begin of line
			case edtLogicAnd:
			case edtLogicOr:
			case edtLogicNot:
				negate = ! negate;
				break;
				
			default:
				pErrorInvalidToken( token, directive );
				// never gets here since pErrorInvalidToken throws an exception
			}
			break;
			
		default:
			pErrorInvalidToken( token, directive );
			// never gets here since pErrorInvalidToken throws an exception
		}
		
		// update result
		if( ! updateResult ){
			continue;
		}
		
		if( updateNegate ){
			updateResultValue = ! updateResultValue;
		}
		
		if( firstResult ){
			result = updateResultValue;
			firstResult = false;
			continue;
		}
		
		switch( updateOp ){
		case edtLogicAnd:
			result &= updateResultValue;
			break;
			
		case edtLogicOr:
			result |= updateResultValue;
			break;
			
		default:
			break;
		}
	}
	
	return result;
}

void deoglShaderPreprocessor::pProcessDirectiveEndIf() {
	// #endif
	pExpectDirectiveToken( edtNewline, "endif" );
	
	if( pDebugLogParsing ){
		pRenderThread.GetLogger().LogInfoFormat( "Shader Preprocessor: #endif: (output %d)", pOutputCode );
	}
	
	pEndDirectiveBlock = true;
}

static const char vTokenEOS[] = "<EOS>";

deoglShaderPreprocessor::eDirectiveTokens deoglShaderPreprocessor::pParseDirectiveToken( sToken &token ){
	int mode = 0;
	
	token.begin = NULL;
	token.end = NULL;
	
	while( *pInputNext ){
		// find token begin
		if( mode == 0 ){
			if( *pInputNext == ' ' || *pInputNext == '\t' ){
				pInputNext++;
				continue;
				
			}else if( *pInputNext == '\n' ){
				token.begin = pInputNext++;
				token.end = pInputNext;
				token.length = 1;
				token.line = pInputLine++;
				token.type = edtNewline;
				return edtNewline;
				
			}else if( *pInputNext == '(' ){
				token.begin = pInputNext++;
				token.end = pInputNext;
				token.length = 1;
				token.line = pInputLine;
				token.type = edtLeftParanthesis;
				return edtLeftParanthesis;
				
			}else if( *pInputNext == ')' ){
				token.begin = pInputNext++;
				token.end = pInputNext;
				token.length = 1;
				token.line = pInputLine;
				token.type = edtRightParanthesis;
				return edtRightParanthesis;
				
			}else if( *pInputNext == ',' ){
				token.begin = pInputNext++;
				token.end = pInputNext;
				token.length = 1;
				token.line = pInputLine;
				token.type = edtComma;
				return edtComma;
				
			}else if( *pInputNext == '"' ){
				token.begin = ++pInputNext;
				token.line = pInputLine;
				token.type = edtString;
				mode = 3;
				continue;
				
			}else if( *pInputNext == '&' && pInputNext[1] == '&' ){
				token.begin = pInputNext;
				pInputNext += 2;
				token.end = pInputNext;
				token.length = 2;
				token.line = pInputLine;
				token.type = edtLogicAnd;
				return edtLogicAnd;
				
			}else if( *pInputNext == '|' && pInputNext[1] == '|' ){
				token.begin = pInputNext;
				pInputNext += 2;
				token.end = pInputNext;
				token.length = 2;
				token.line = pInputLine;
				token.type = edtLogicOr;
				return edtLogicOr;
				
			}else if( *pInputNext == '=' && pInputNext[1] == '=' ){
				token.begin = pInputNext;
				pInputNext += 2;
				token.end = pInputNext;
				token.length = 2;
				token.line = pInputLine;
				token.type = edtEquals;
				return edtEquals;
				
			}else if( *pInputNext == '!' && pInputNext[1] == '=' ){
				token.begin = pInputNext;
				pInputNext += 2;
				token.end = pInputNext;
				token.length = 2;
				token.line = pInputLine;
				token.type = edtNotEquals;
				return edtEquals;
				
			}else if( *pInputNext == '!' ){
				token.begin = pInputNext++;
				token.end = pInputNext;
				token.length = 1;
				token.line = pInputLine;
				token.type = edtLogicNot;
				return edtLogicNot;
				
			}else if( *pInputNext == '<' && pInputNext[1] == '=' ){
				token.begin = pInputNext;
				pInputNext += 2;
				token.end = pInputNext;
				token.length = 2;
				token.line = pInputLine;
				token.type = edtLessThanOrEquals;
				return edtLessThanOrEquals;
				
			}else if( *pInputNext == '<' ){
				token.begin = pInputNext++;
				token.end = pInputNext;
				token.length = 1;
				token.line = pInputLine;
				token.type = edtLessThan;
				return edtLessThan;
				
			}else if( *pInputNext == '>' && pInputNext[1] == '=' ){
				token.begin = pInputNext;
				pInputNext += 2;
				token.end = pInputNext;
				token.length = 2;
				token.line = pInputLine;
				token.type = edtGreaterThanOrEquals;
				return edtGreaterThanOrEquals;
				
			}else if( *pInputNext == '>' ){
				token.begin = pInputNext++;
				token.end = pInputNext;
				token.length = 1;
				token.line = pInputLine;
				token.type = edtGreaterThan;
				return edtGreaterThan;
				
			}else if( *pInputNext == '\\' && pInputNext[1] == '\n' ){
				token.begin = pInputNext;
				pInputNext += 2;
				token.end = pInputNext;
				token.length = 2;
				token.line = pInputLine++;
				token.type = edtLineSplicer;
				return edtLineSplicer;
				
			}else if( *pInputNext == '/' && pInputNext[1] == '/' ){
				// comment
				if( token.begin ){
					token.end = pInputNext;
				}
				pInputNext += 2;
				mode = 2;
				continue;
				
			}else if( pIsNumber( *pInputNext ) ){
				token.begin = pInputNext++;
				token.line = pInputLine;
				token.type = edtNumber;
				mode = 5;
				continue;
				
			}else if( pIsSymbolBegin( *pInputNext ) ){
				token.begin = pInputNext++;
				token.line = pInputLine;
				token.type = edtIdentifier;
				mode = 1;
				continue;
				
			}else{
				token.begin = pInputNext++;
				token.line = pInputLine;
				token.type = edtAnything;
				mode = 4;
				continue;
			}
			
		// find end of identifier
		}else if( mode == 1 ){
			if( pIsSymbol( *pInputNext ) ){
				pInputNext++;
				continue;
				
			}else{
				token.end = pInputNext;
				token.length = ( int )( token.end - token.begin );
				return edtIdentifier;
			}
			
		// find end of comment
		}else if( mode == 2 ){
			if( *pInputNext == '\n' ){
				if( token.begin ){
					pInputNext++;
					pInputLine++;
					return token.type;
					
				}else{
					token.begin = pInputNext++;
					token.end = pInputNext;
					token.length = 1;
					token.line = pInputLine++;
					token.type = edtNewline;
					return edtNewline;
				}
				
			}else if( *pInputNext == '\\' && pInputNext[1] == '\n' ){
				// line splicing
				pInputNext += 2;
				pInputLine++;
				continue;
				
			}else{
				pInputNext++;
				continue;
			}
			
		// find end of string
		}else if( mode == 3 ){
			if( *pInputNext == '"' ){
				token.end = pInputNext++;
				token.length = ( int )( token.end - token.begin );
				return edtString;
				
			}else if( *pInputNext == '\\' && pInputNext[1] == '\n' ){
				// line splicing
				pInputNext += 2;
				pInputLine++;
				continue;
				
			}else{
				pInputNext++;
				continue;
			}
			
		// find end of anything
		}else if( mode == 4 ){
			if( *pInputNext == '\n' ){
				token.end = pInputNext++;
				pInputLine++;
				token.length = ( int )( token.end - token.begin );
				return edtAnything;
				
			}else if( *pInputNext == '/' && pInputNext[1] == '/' ){
				// comment
				token.end = pInputNext;
				token.length = ( int )( token.end - token.begin );
				pInputNext += 2;
				mode = 2;
				continue;
				
			}else{
				pInputNext++;
				continue;
			}
			
		// find end of number
		}else if( mode == 5 ){
			if( pIsNumber( *pInputNext ) ){
				pInputNext++;
				continue;
				
			}else{
				token.end = pInputNext;
				token.length = ( int )( token.end - token.begin );
				return edtNumber;
			}
		}
		
		if( ! token.begin ){
			token.begin = pInputNext;
			token.line = pInputLine;
		}
		if( ! token.end ){
			token.end = pInputNext + 1;
		}
		token.length = ( int )( token.end - token.begin );
		token.type = edtInvalid;
		return edtInvalid;
	}
	
	if( ! token.begin ){
		token.begin = vTokenEOS;
		token.end = token.begin + 5;
		token.length = 5;
		token.line = pInputLine;
		
	}else if( ! token.end ){
		token.end = pInputNext;
		token.length = ( int )( token.end - token.begin );
	}
	
	if( mode == 0 ){
		// act like end of line
		token.type = edtNewline;
		
	}else if( mode == 2 ){
		// act as end of comment
		if( token.type != edtAnything ){
			token.type = edtNewline;
		}
		
	}else if( mode == 4 ){
		token.type = edtAnything;
		
	}else{
		token.type = edtInvalid;
	}
	
	return token.type;
}

void deoglShaderPreprocessor::pExpectDirectiveToken( sToken &token,
eDirectiveTokens expectedType, const char *directive ){
	if( pParseDirectiveToken( token ) != expectedType ){
		pErrorInvalidToken( token, directive );
	}
}

void deoglShaderPreprocessor::pExpectDirectiveToken(
eDirectiveTokens expectedType, const char *directive ){
	sToken token;
	if( pParseDirectiveToken( token ) != expectedType ){
		pErrorInvalidToken( token, directive );
	}
}

bool deoglShaderPreprocessor::pParseDirectiveAnything( sToken &token ){
	int mode = 0;
	
	token.begin = NULL;
	token.end = NULL;
	token.type = edtAnything;
	
	while( *pInputNext ){
		// find begin of anything
		if( mode == 0 ){
			if( *pInputNext == '\n' ){
				token.begin = token.end = pInputNext++;
				token.length = 0;
				token.line = pInputLine++;
				return false;
				
			}else if( *pInputNext == ' ' || *pInputNext == '\t' ){
				pInputNext++;
				
			}else if( *pInputNext == '/' && pInputNext[1] == '/' ){
				// comment
				token.begin = token.end = pInputNext;
				token.length = 0;
				token.line = pInputLine;
				pInputNext += 2;
				mode = 2;
				
			}else{
				token.begin = pInputNext++;
				token.line = pInputLine;
				mode = 1;
			}
			
		// find end of anything
		}else if( mode == 1 ){
			if( *pInputNext == '\n' ){
				token.end = pInputNext++;
				token.length = ( int )( token.end - token.begin );
				pInputLine++;
				return token.length > 0;
				
			}else if( *pInputNext == '/' && pInputNext[1] == '/' ){
				// comment
				token.end = pInputNext;
				token.length = ( int )( token.end - token.begin );
				pInputNext += 2;
				mode = 2;
				
			}else{
				pInputNext++;
			}
			
		// find end of comment
		}else if( mode == 2 ){
			if( *pInputNext == '\n' ){
				pInputLine++;
				return token.length > 0;
				
			}else if( *pInputNext == '\\' && pInputNext[1] == '\n' ){
				// line splicing
				pInputNext += 2;
				pInputLine++;
				
			}else{
				pInputNext++;
			}
		}
	}
	
	if( ! token.begin ){
		token.begin = pInputNext;
		token.end = pInputNext;
		token.length = 0;
		token.line = pInputLine;
		
	}else if( ! token.end ){
		token.end = pInputNext;
		token.length = ( int )( token.end - token.begin );
	}
	return token.length > 0;
}

decString deoglShaderPreprocessor::pDirectiveTokenString( const sToken &token ) const{
	decString string;
	string.Set( ' ', token.length );
	#ifdef OS_W32_VS
		strncpy_s( (char*)string.GetString(), token.length + 1, token.begin, token.length );
	#else
		strncpy( (char*)string.GetString(), token.begin, token.length );
	#endif
	return string;
}

void deoglShaderPreprocessor::pErrorInvalidToken( const sToken &token, const char *directive ) const{
	/*
	#ifdef OS_BEOS
	// compiler bug protection
	if( ! token.begin ){
		pRenderThread.GetLogger().LogErrorFormat("Shader Preprocessor: #%s: Invalid token <COMPILER-BUG> at %s:%d",
			directive, pInputFile != NULL ? pInputFile : "?", token.line );
		DETHROW( deeInvalidParam );
	}
	#endif
	*/
	
	pRenderThread.GetLogger().LogErrorFormat(
		"Shader Preprocessor: #%s: Invalid token '%s' at %s:%d", directive,
		pDirectiveTokenString( token ).GetString(),
		pInputFile != NULL ? pInputFile : "?", token.line );
	
	DETHROW( deeInvalidParam );
}



void deoglShaderPreprocessor::pResolveString( const char *text, int length ){
	const char *next = text;
	const char *last = text + length;
	const char *end = last + 1;
	const char *codeBegin = next;
	const char *tokenBegin = NULL;
	decString result;
	int mode = 0;
	
	pResolveBufferLen = 0;
	
	while( next != end ){
		const int c = next != last ? *next : '\0';
		
		switch( mode ){
		// find begin token
		case 0:
			if( pIsSymbolBegin( c ) ){
				if( next != codeBegin ){
					pResolveBufferAppend( codeBegin, ( int )( next - codeBegin ) );
				}
				tokenBegin = next++;
				mode = 1;
				
			}else{
				next++;
			}
			break;
			
		// find end of token
		case 1:
			if( pIsSymbol( c ) ){
				next++;
				codeBegin = next;
				
			}else{
				pSetResolveSymbolName( tokenBegin, ( int )( next - tokenBegin ) );
				
				const deoglShaderPreprocessorSymbol * const symbol = GetSymbolNamed( pResolveSymbolName );
				
				if( symbol ){
					pResolveBufferAppend( symbol->GetValue(), symbol->GetValue().GetLength() );
					
				}else{
					pResolveBufferAppend( tokenBegin, ( int )( next - tokenBegin ) );
				}
				
				codeBegin = next++;
				mode = 0;
			}
			break;
		}
	};
	
	pResolveBuffer[ pResolveBufferLen ] = '\0';
	
	if( last != codeBegin ){
		pResolveBufferAppend( codeBegin, ( int )( last - codeBegin ) );
	}
}

void deoglShaderPreprocessor::pResolveBufferAppend( const char *text, int length ){
	if( length == 0 ){
		return;
	}
	
	if( pResolveBufferLen + length > pResolveBufferSize ){
		const int newSize = pResolveBufferLen + length + 1024;  // increment by steps of 1k
		char * const newResolveBuffer = ( char* )realloc( pResolveBuffer, newSize + 1 );
		if( ! newResolveBuffer ){
			DETHROW( deeOutOfMemory );
		}
		pResolveBuffer = newResolveBuffer;
		pResolveBufferSize = newSize;
	}
	
	#ifdef OS_W32_VS
		strncpy_s( pResolveBuffer + pResolveBufferLen, length + 1, text, length );
	#else
		strncpy( pResolveBuffer + pResolveBufferLen, text, length );
	#endif
	
	pResolveBufferLen += length;
	pResolveBuffer[ pResolveBufferLen ] = '\0';
}

void deoglShaderPreprocessor::pSetResolveSymbolName( const char *name, int length ){
	if( pResolveSymbolNameLen + length > pResolveSymbolNameSize ){
		const int newSize = pResolveSymbolNameLen + length + 50;
		char * const newName = ( char* )realloc( pResolveSymbolName, newSize + 1 );
		if( ! newName ){
			DETHROW( deeOutOfMemory );
		}
		pResolveSymbolName = newName;
		pResolveSymbolNameSize = newSize;
	}
	
	#ifdef OS_W32_VS
		strncpy_s( pResolveSymbolName, length + 1, name, length );
	#else
		strncpy( pResolveSymbolName, name, length );
	#endif
	pResolveSymbolName[ length ] = '\0';
}



bool deoglShaderPreprocessor::pIsSymbol( int character ) const{
	return ( character >= 'a' && character <= 'z' )
		|| ( character >= 'A' && character <= 'Z' )
		|| ( character >= '0' && character <= '9' )
		|| character == '_';
}

bool deoglShaderPreprocessor::pIsSymbolBegin( int character ) const{
	return ( character >= 'a' && character <= 'z' )
		|| ( character >= 'A' && character <= 'Z' )
		|| character == '_';
}

bool deoglShaderPreprocessor::pIsNumber( int character ) const{
	return character >= '0' && character <= '9';
}
