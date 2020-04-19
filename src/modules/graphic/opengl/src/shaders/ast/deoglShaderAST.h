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

#ifndef _DEOGLSHADERAST_H_
#define _DEOGLSHADERAST_H_

#include "deoglSASTFunctionList.h"
#include "deoglSASTDeclarationList.h"

class decString;
class deoglSASTGenerateCode;
class deoglRenderThread;



/**
 * @brief Shader Abstract Syntax Tree.
 */
class deoglShaderAST{
public:
	/** Data types. */
	enum eDataTypes{
		edtVoid,
		edtFloat,
		edtInt,
		edtBool,
		edtVec2,
		edtVec3,
		edtVec4,
		edtIVec2,
		edtIVec3,
		edtIVec4,
		edtBVec2,
		edtBVec3,
		edtBVec4,
		edtMat3x2,
		edtMat3x3,
		edtMat4x3,
		edtMat4x4,
		edtSampler2D,
		edtSampler2DShadow,
		edtSampler2DArray,
		edtSampler2DArrayShadow,
		edtSamplerCube,
		edtCustom,
		EDT_COUNT
	};
	
	/** Swizzles. */
	enum eSwizzles{
		esNone,
		esX,
		esY,
		esZ,
		esW,
		esR,
		esG,
		esB,
		esA,
		ES_COUNT
	};
	
private:
	bool pHighPrecision;
	deoglSASTDeclarationList pUniformList;
	deoglSASTDeclarationList pSamplerList;
	deoglSASTDeclarationList pInputList;
	deoglSASTDeclarationList pOutputList;
	deoglSASTDeclarationList pConstantList;
	deoglSASTFunctionList pFunctionList;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree. */
	deoglShaderAST( deoglRenderThread &renderThread );
	/** Cleans up the shader abstract syntax tree. */
	~deoglShaderAST();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if the high precision declarations have to be generated. */
	inline bool GetHighPrecision() const{ return pHighPrecision; }
	/** Sets if the high precision declarations have to be generated. */
	void SetHighPrecision( bool highPrecision );
	/** Retrieves the list of uniform declarations. */
	inline deoglSASTDeclarationList &GetUniformList(){ return pUniformList; }
	inline const deoglSASTDeclarationList &GetUniformList() const{ return pUniformList; }
	/** Adds a uniform declaration. */
	void AddUniform( const char *name, int dataType );
	/** Retrieves the list of sampler declarations. */
	inline deoglSASTDeclarationList &GetSamplerList(){ return pSamplerList; }
	inline const deoglSASTDeclarationList &GetSamplerList() const{ return pSamplerList; }
	/** Adds a sampler declaration. */
	void AddSampler( const char *name, int dataType );
	/** Retrieves the list of input declarations. */
	inline deoglSASTDeclarationList &GetInputList(){ return pInputList; }
	inline const deoglSASTDeclarationList &GetInputList() const{ return pInputList; }
	/** Adds an input declaration. */
	void AddInput( const char *name, int dataType );
	/** Retrieves the list of output declarations. */
	inline deoglSASTDeclarationList &GetOutputList(){ return pOutputList; }
	inline const deoglSASTDeclarationList &GetOutputList() const{ return pOutputList; }
	/** Adds an output declaration. */
	void AddOutput( const char *name, int dataType );
	/** Retrieves the list of constant declarations. */
	inline deoglSASTDeclarationList &GetConstantList(){ return pConstantList; }
	inline const deoglSASTDeclarationList &GetConstantList() const{ return pConstantList; }
	/** Add a constant with init nodes created from init values. */
	void AddConstantFloat( const char *name, float initValue );
	void AddConstantVec2( const char *name, float initValueX, float initValueY );
	void AddConstantVec2( const char *name, float initValue );
	void AddConstantVec3( const char *name, float initValueX, float initValueY, float initValueZ );
	void AddConstantVec3( const char *name, float initValue );
	void AddConstantVec4( const char *name, float initValueX, float initValueY, float initValueZ, float initValueW );
	void AddConstantVec4( const char *name, float initValue );
	/** Retrieves the list of functions. */
	inline deoglSASTFunctionList &GetFunctionList(){ return pFunctionList; }
	inline const deoglSASTFunctionList &GetFunctionList() const{ return pFunctionList; }
	
	/** Retrieves the string for a data type or NULL if the data type is custom. */
	static const char *GetDataTypeString( int dataType );
	/** Retrieves the string for a swizzle. */
	static const char *GetSwizzleString( int swizzle );
	
	/** Verify the abstract syntax tree. */
	bool Verify() const;
	/** Optimize the abstract syntax tree if possible. */
	void Optimize();
	/** Generate shader source code. */
	void GenerateSourceCode( decString &sourceCode );
	
	/** Generate header source code. */
	void GenerateHeaderSourceCode( deoglSASTGenerateCode &gencode );
	/** Generate declaration source code. */
	void GenerateDeclarationSourceCode( deoglSASTGenerateCode &gencode, const deoglSASTDeclarationList &list, const char *prefix );
	/** Generate function source code. */
	void GenerateFunctionSourceCode( deoglSASTGenerateCode &gencode, const deoglSASTFunctionList &list );
	void GenerateFunctionSourceCode( deoglSASTGenerateCode &gencode, deoglSASTFunction &function );
	/*@}*/
	
	/** @name Debugging */
	/*@{*/
	/** Test AST. */
	static void DebugTestAST( deoglRenderThread &renderThread );
	/** Create a testing ast. */
	static deoglShaderAST *DebugCreateTestAST( deoglRenderThread &renderThread );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
