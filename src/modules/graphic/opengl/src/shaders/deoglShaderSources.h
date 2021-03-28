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

#ifndef _DEOSLSHADERSOURCES_H_
#define _DEOSLSHADERSOURCES_H_

#include <dragengine/common/string/decStringList.h>

#include "deoglShaderBindingList.h"

class deLogger;
class decBaseFileReader;
class decXmlAttValue;
class decXmlElementTag;



/**
 * Stores the source code of a shader program. Typically this is a vertex program source
 * code and a fragment program source code. The Name is used to reference shader sources.
 */
class deoglShaderSources{
private:
	decString pName;
	decString pFilename;
	
	decString pVersion;
	
	decString pPathSCCompute;
	decString pPathSCTessellationControl;
	decString pPathSCTessellationEvaluation;
	decString pPathSCGeometry;
	decString pPathSCVertex;
	decString pPathSCFragment;
	
	decString pInlineSCGeometry;
	decString pInlineSCVertex;
	decString pInlineSCFragment;
	
	deoglShaderBindingList pTextureList;
	deoglShaderBindingList pAttributeList;
	deoglShaderBindingList pOutputList;
	deoglShaderBindingList pUniformBlockList;
	deoglShaderBindingList pShaderStorageBlockList;
	decStringList pParameterList;
	decStringList pFeedbackList;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Create . */
	deoglShaderSources();
	/** Loads . */
	deoglShaderSources( deLogger &logger, decBaseFileReader &reader );
	/** Cleans up the shader source code object. */
	~deoglShaderSources();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Retrieves the version. */
	inline const decString &GetVersion() const{ return pVersion; }
	/** Sets the version. */
	void SetVersion( const char *version );
	
	/** Retrieves the texture binding list. */
	inline deoglShaderBindingList &GetTextureList(){ return pTextureList; }
	inline const deoglShaderBindingList &GetTextureList() const{ return pTextureList; }
	/** Retrieves the attribute binding list. */
	inline deoglShaderBindingList &GetAttributeList(){ return pAttributeList; }
	inline const deoglShaderBindingList &GetAttributeList() const{ return pAttributeList; }
	/** Retrieves the output binding list. */
	inline deoglShaderBindingList &GetOutputList(){ return pOutputList; }
	inline const deoglShaderBindingList &GetOutputList() const{ return pOutputList; }
	
	/** Uniform block binding list. */
	inline deoglShaderBindingList &GetUniformBlockList(){ return pUniformBlockList; }
	inline const deoglShaderBindingList &GetUniformBlockList() const{ return pUniformBlockList; }
	
	/** Shader storage block binding list. */
	inline deoglShaderBindingList &GetShaderStorageBlockList(){ return pShaderStorageBlockList; }
	inline const deoglShaderBindingList &GetShaderStorageBlockList() const{ return pShaderStorageBlockList; }
	
	/** Retrieves the parameter list. */
	inline decStringList &GetParameterList(){ return pParameterList; }
	inline const decStringList &GetParameterList() const{ return pParameterList; }
	/** Retrieves the feedback list. */
	inline decStringList &GetFeedbackList(){ return pFeedbackList; }
	inline const decStringList &GetFeedbackList() const{ return pFeedbackList; }
	
	/** Path to compute unit source code file or empty string. */
	inline const decString &GetPathComputeSourceCode() const{ return pPathSCCompute; }
	
	/** Set path to compute unit source code file or empty string. */
	void SetPathComputeSourceCode( const char *path );
	
	/** Retrieves the path to the tessellation control unit source code file or an empty string if not used. */
	inline const decString &GetPathTessellationControlSourceCode() const{ return pPathSCTessellationControl; }
	
	/** Sets the path to the tessellation control unit source code file or an empty string if not used. */
	void SetPathTessellationControlSourceCode( const char *path );
	
	/** Retrieves the path to the tessellation evaluation unit source code file or an empty string if not used. */
	inline const decString &GetPathTessellationEvaluationSourceCode() const{ return pPathSCTessellationEvaluation; }
	/** Sets the path to the tessellation evaluation unit source code file or an empty string if not used. */
	void SetPathTessellationEvaluationSourceCode( const char *path );
	/** Retrieves the path to the geometry unit source code file or an empty string if not used. */
	inline const decString &GetPathGeometrySourceCode() const{ return pPathSCGeometry; }
	/** Sets the path to the geometry unit source code file or an empty string if not used. */
	void SetPathGeometrySourceCode( const char *path );
	/** Retrieves the path to the vertex unit source code file or an empty string if not used. */
	inline const decString &GetPathVertexSourceCode() const{ return pPathSCVertex; }
	/** Sets the path to the vertex unit source code file or an empty string if not used. */
	void SetPathVertexSourceCode( const char *path );
	/** Retrieves the path to the fragment unit source code file or an empty string if not used. */
	inline const decString &GetPathFragmentSourceCode() const{ return pPathSCFragment; }
	/** Sets the path to the fragment unit source code file or an empty string if not used. */
	void SetPathFragmentSourceCode( const char *path );
	
	/** Retrieves the inline geometry unit source code. */
	inline const decString &GetInlineGeometrySourceCode() const{ return pInlineSCGeometry; }
	/** Sets the inline geometry unit source code. */
	void SetInlineGeometrySourceCode( const char *sourceCode );
	/** Retrieves the inline vertex unit source code. */
	inline const decString &GetInlineVertexSourceCode() const{ return pInlineSCVertex; }
	/** Sets the inline vertex unit source code. */
	void SetInlineVertexSourceCode( const char *sourceCode );
	/** Retrieves the inline fragment unit source code. */
	inline const decString &GetInlineFragmentSourceCode() const{ return pInlineSCFragment; }
	/** Sets the inline fragment unit source code. */
	void SetInlineFragmentSourceCode( const char *sourceCode );
	/*@}*/
	
private:
	decXmlElementTag *pGetTagAt( const decXmlElementTag &tag, int index );
	decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	
	void pParseShader( deLogger &logger, const decXmlElementTag &root );
};

#endif
