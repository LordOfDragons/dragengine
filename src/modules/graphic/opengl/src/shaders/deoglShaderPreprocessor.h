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

#ifndef _DEOGLSHADERPREPROCESSOR_H_
#define _DEOGLSHADERPREPROCESSOR_H_

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringSet.h>

class deoglRenderThread;
class deoglShaderPreprocessorSymbol;
class deoglShaderSourceLocation;

class deoglShaderDefines;
class deoglShaderProgram;
#ifdef WITH_OPENGLES
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
	decStringSet pMacroSymbol;
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
	const deoglShaderSourceLocation *ResolveSourceLocation(int line) const;
	const deoglShaderSourceLocation *ResolveSourceLocation(const decObjectList &locations, int line) const;
	/*@}*/
	
	
	
	/** \name Sources */
	/*@{*/
	/** Sources. */
	inline const char *GetSources() const{ return pSources; }
	
	/** Sources length. */
	inline int GetSourcesLength() const{ return pSourcesLen; }
	
	/** Source locations. */
	inline const decObjectList &GetSourceLocations() const{ return pSourceLocations; }
	
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
	
	/** Get symbol names. */
	decStringList GetSymbolNames() const;
	
	/** Named symbol is present. */
	bool HasSymbolNamed( const char *name ) const;
	
	/** Named symbol or \em NULL if absent. */
	deoglShaderPreprocessorSymbol *GetSymbolNamed( const char *name ) const;
	
	/** Set symbol. */
	void SetSymbol( deoglShaderPreprocessorSymbol *symbol );
	
	/** Set symbol. */
	void SetSymbol( const char *name, const char *value );
	
	/** Named macro symbol is present. */
	bool HasMacroSymbolNamed(const char *name) const;
	
	/** Set has symbol. */
	void AddMacroSymbol(const char *name);
	
	/** Named symbol or macro symbol is present. */
	bool HasAnySymbolNamed(const char *name) const;
	
	/** Clear symbol. */
	void ClearSymbol( const char *name );
	
	/** Clear all symbols. */
	void ClearAllSymbols();
	
	/** Set symbols from defines. */
	void SetSymbolsFromDefines( const deoglShaderDefines &defines );
	
	/** Set debug log parsing. */
	void SetDebugLogParsing(bool enable);
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
