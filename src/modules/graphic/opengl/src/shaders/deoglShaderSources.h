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

#ifndef _DEOSLSHADERSOURCES_H_
#define _DEOSLSHADERSOURCES_H_

#include "deoglShaderBindingList.h"

#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/deObject.h>

class deLogger;
class decBaseFileReader;
class decXmlAttValue;
class decXmlElementTag;



/**
 * Stores the source code of a shader program. Typically this is a vertex program source
 * code and a fragment program source code. The Name is used to reference shader sources.
 */
class deoglShaderSources : public deObject{
public:
	typedef deTObjectReference<deoglShaderSources> Ref;
	
	
private:
	decString pName;
	decString pFilename;
	
	decString pPathSCCompute;
	decString pPathSCTessellationControl;
	decString pPathSCTessellationEvaluation;
	decString pPathSCGeometry;
	decString pPathSCVertex;
	decString pPathSCFragment;
	
	deoglShaderBindingList pTextureList;
	decStringList pParameterList;
	decIntList pParameterLocations;
	bool pFeedbackInterleaved;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader sources. */
	deoglShaderSources();
	
	/** Load shader sources. */
	deoglShaderSources(deLogger &logger, decBaseFileReader &reader);
	
protected:
	/** Clean up shader sources. */
	virtual ~deoglShaderSources();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Retrieves the texture binding list. */
	inline deoglShaderBindingList &GetTextureList(){ return pTextureList; }
	inline const deoglShaderBindingList &GetTextureList() const{ return pTextureList; }
	
	/** Retrieves the parameter list. */
	inline const decStringList &GetParameterList() const{ return pParameterList; }
	
	/** Parameter locations. */
	inline const decIntList &GetParameterLocations() const{ return pParameterLocations; }
	
	/** Add parameter. */
	void AddParameter(const char *name, int location);
	
	/** Path to compute unit source code file or empty string. */
	inline const decString &GetPathComputeSourceCode() const{ return pPathSCCompute; }
	
	/** Set path to compute unit source code file or empty string. */
	void SetPathComputeSourceCode(const char *path);
	
	/** Retrieves the path to the tessellation control unit source code file or an empty string if not used. */
	inline const decString &GetPathTessellationControlSourceCode() const{ return pPathSCTessellationControl; }
	
	/** Sets the path to the tessellation control unit source code file or an empty string if not used. */
	void SetPathTessellationControlSourceCode(const char *path);
	
	/** Retrieves the path to the tessellation evaluation unit source code file or an empty string if not used. */
	inline const decString &GetPathTessellationEvaluationSourceCode() const{ return pPathSCTessellationEvaluation; }
	/** Sets the path to the tessellation evaluation unit source code file or an empty string if not used. */
	void SetPathTessellationEvaluationSourceCode(const char *path);
	/** Retrieves the path to the geometry unit source code file or an empty string if not used. */
	inline const decString &GetPathGeometrySourceCode() const{ return pPathSCGeometry; }
	/** Sets the path to the geometry unit source code file or an empty string if not used. */
	void SetPathGeometrySourceCode(const char *path);
	/** Retrieves the path to the vertex unit source code file or an empty string if not used. */
	inline const decString &GetPathVertexSourceCode() const{ return pPathSCVertex; }
	/** Sets the path to the vertex unit source code file or an empty string if not used. */
	void SetPathVertexSourceCode(const char *path);
	/** Retrieves the path to the fragment unit source code file or an empty string if not used. */
	inline const decString &GetPathFragmentSourceCode() const{ return pPathSCFragment; }
	/** Sets the path to the fragment unit source code file or an empty string if not used. */
	void SetPathFragmentSourceCode(const char *path);
	/*@}*/
	
private:
	decXmlElementTag *pGetTagAt(const decXmlElementTag &tag, int index);
	decXmlAttValue *pFindAttribute(const decXmlElementTag &tag, const char *name);
	
	void pParseShader(deLogger &logger, const decXmlElementTag &root);
};

#endif
