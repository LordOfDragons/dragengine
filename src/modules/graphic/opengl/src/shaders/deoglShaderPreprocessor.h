/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSHADERPREPROCESSOR_H_
#define _DEOGLSHADERPREPROCESSOR_H_

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglRenderThread;
class deoglShaderPreprocessorSymbol;
class deoglShaderSourceLocation;

class deoglShaderDefines;
class deoglShaderProgram;
#ifdef ANDROID
class deoglShaderBindingList;
#endif



/**
 * GLSL shader preprocessor.
 *
 * Applies preprocessing to GLSL shaders. There are a couple of reasons why
 * the preprocessing is not done by the GPU driver:
 * 
 * - Some GPU drivers have buggy implementations leading to compile failures
 * - Some GPU drivers do not support all preprocessing defined in OpenGL
 * - GLSL does not support #include pragma as required by this module
 * - GLSL pragmas are quite limited. Allows to add improved pragmas
 * - Allows to optimize shaders by stripping unused code
 * - Allows to build a line mapping table to help locate errors
 * - Allows to preprocess shaders once and then cache them to disk
 */
class deoglShaderPreprocessor{
private:
	enum eDirectiveTokens{
		edtIdentifier,
		edtNewline,
		edtLineSplicer,
		edtLeftParanthesis,
		edtRightParanthesis,
		edtComma,
		edtLogicAnd,
		edtLogicOr,
		edtLogicNot,
		edtEquals,
		edtNotEquals,
		edtLessThan,
		edtLessThanOrEquals,
		edtGreaterThan,
		edtGreaterThanOrEquals,
		edtString,
		edtNumber,
		edtAnything,
		edtInvalid
	};
	
	deoglRenderThread &pRenderThread;
	
	char *pSources;
	int pSourcesLen;
	int pSourcesSize;
	
	decObjectDictionary pSymbolTable;
	const char *pInputNext;
	const char *pInputFile;
	int pInputLine;
	int pOutputLine;
	bool pOutputCode;
	bool pOutputCodeCase;
	bool pEndDirectiveBlock;
	decObjectList pSourceLocations;
	int pLastMappedOutputLine;
	
	char *pResolveBuffer;
	int pResolveBufferLen;
	int pResolveBufferSize;
	
	char *pResolveSymbolName;
	int pResolveSymbolNameLen;
	int pResolveSymbolNameSize;
	
	bool pDisablePreprocessing;
	bool pDebugLogParsing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader preprocessor. */
	deoglShaderPreprocessor( deoglRenderThread &renderThread );
	
	/** Clean up shader preprocessor. */
	~deoglShaderPreprocessor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear. */
	void Clear();
	
	/** Log line map to renderthread logger. */
	void LogSourceLocationMap();
	
	/** Resolve source location or NULL. */
	const deoglShaderSourceLocation *ResolveSourceLocation( int line ) const;
	/*@}*/
	
	
	
	/** \name Sources */
	/*@{*/
	/** Sources. */
	inline const char *GetSources() const{ return pSources; }
	
	/** Sources length. */
	inline int GetSourcesLength() const{ return pSourcesLen; }
	
	/** Append to sources. */
	void SourcesAppend( const char *text, bool mapLines );
	
	/** Append to sources. */
	void SourcesAppend( const char *text, int length, bool mapLines );
	
	/** Process source code. */
	void SourcesAppendProcessed( const char *sourceCode );
	
	/** Process source code. */
	void SourcesAppendProcessed( const char *sourceCode, const char *inputFile,
		bool resetState = false );
	/*@}*/
	
	
	
	/** \name Symbol Table */
	/*@{*/
	/** Number of symbols. */
	int GetSymbolCount() const;
	
	/** Named symbol is present. */
	bool HasSymbolNamed( const char *name ) const;
	
	/** Named symbol or \em NULL if absent. */
	deoglShaderPreprocessorSymbol *GetSymbolNamed( const char *name ) const;
	
	/** Set symbol. */
	void SetSymbol( deoglShaderPreprocessorSymbol *symbol );
	
	/** Set symbol. */
	void SetSymbol( const char *name, const char *value );
	
	/** Clear symbol. */
	void ClearSymbol( const char *name );
	
	/** Clear all symbols. */
	void ClearAllSymbols();
	
	/** Set symbols from defines. */
	void SetSymbolsFromDefines( const deoglShaderDefines &defines );
	/*@}*/
	
	
	
private:
	struct sToken{
		const char *begin;
		const char *end;
		int length;
		int line;
		eDirectiveTokens type;
	};
	
	void pProcessSources();
	void pProcessSingleLineComment();
	void pProcessMultiLineComment();
	void pProcessDirective( const char *beginLine );
	void pProcessDirectiveInclude();
	void pProcessDirectiveDefine( const char *beginLine );
	void pProcessDirectiveUndefine( const char *beginLine );
	void pProcessDirectiveIfDef();
	void pProcessDirectiveIfNotDef();
	void pProcessDirectiveIf();
	void pProcessDirectiveElseIf();
	void pProcessDirectiveElse();
	void pProcessDirectiveEndIf();
	bool pProcessDirectiveCondition( const char *directive, bool groupOpen );
	
	eDirectiveTokens pParseDirectiveToken( sToken &token );
	void pExpectDirectiveToken( sToken &token, eDirectiveTokens expectedType, const char *directive );
	void pExpectDirectiveToken( eDirectiveTokens expectedType, const char *directive );
	bool pParseDirectiveAnything( sToken &token );
	decString pDirectiveTokenString( const sToken &token ) const;
	void pErrorInvalidToken( const sToken &token, const char *directive ) const;
	
	void pResolveString( const char *text, int length );
	void pResolveBufferAppend( const char *text, int length );
	void pSetResolveSymbolName( const char *name, int length );
	
	bool pIsSymbol( int character ) const;
	bool pIsSymbolBegin( int character ) const;
	bool pIsNumber( int character ) const;
};

#endif
