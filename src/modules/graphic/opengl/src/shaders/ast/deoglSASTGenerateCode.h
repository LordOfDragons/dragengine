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

#ifndef _DEOGLSASTGENERATECODE_H_
#define _DEOGLSASTGENERATECODE_H_

class deoglShaderAST;
class decString;



/**
 * @brief Shader Abstract Syntax Tree Generate Code State.
 */
class deoglSASTGenerateCode{
private:
	deoglShaderAST *pAST;
	decString *pSourceCode;
	int pIndentTabs;
	bool pRequiresParanthesis;
	bool pRequiresEndOfCommand;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree generate code state. */
	deoglSASTGenerateCode( deoglShaderAST *ast, decString *sourceCode );
	/** Cleans up the shader abstract syntax tree generate code state. */
	virtual ~deoglSASTGenerateCode();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the ast. */
	inline deoglShaderAST *GetAST() const{ return pAST; }
	/** Retrieves the source code string. */
	inline decString &GetSourceCode(){ return *pSourceCode; }
	
	/** Retrieves the number of tabs to indent generated code. */
	inline int GetIndentTabs() const{ return pIndentTabs; }
	/** Increment the number of tabs to indent generated code by one. */
	inline void IncrementIndentTabs(){ SetIndentTabs( pIndentTabs + 1 ); }
	/** Decrements the number of tabs to indent generated code by one. */
	inline void DecrementIndentTabs(){ SetIndentTabs( pIndentTabs - 1 ); }
	/** Sets the number of tabs to indent generated code. */
	void SetIndentTabs( int tabs );
	/** Determines if paranthesis are required. */
	inline bool GetRequiresParanthesis() const{ return pRequiresParanthesis; }
	/** Sets if paranthesis are required. */
	void SetRequiresParanthesis( bool requiresParanthesis );
	/** Determines if end of command semicolon is required. */
	inline bool GetRequiresEndOfCommand() const{ return pRequiresEndOfCommand; }
	/** Sets if end of command semicolon is required. */
	void SetRequiresEndOfCommand( bool requiresEndOfCommand );
	
	/** Add indent tabs to the source code. */
	void AddIndentTabsToSourceCode();
	/*@}*/
};

#endif
