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

#ifndef _DEOGLSHADERUNITSOURCECODE_H_
#define _DEOGLSHADERUNITSOURCECODE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/utils/decDateTime.h>
#include <dragengine/deObject.h>

#include "../deoglBasics.h"

class deGraphicOpenGl;
class decBaseFileReader;
class decXmlElementTag;
class decPath;


/**
 * Shader Unit Source Code.
 *
 * Stores the source code of a shader unit program. The shader unit
 * source code can be compiled into a specialized shader unit program
 * using a set of defines. Each shader unit source object is identified
 * by the file path relative to the shader unit source code directory.
 */
class deoglShaderUnitSourceCode : public deObject{
public:
	typedef deTObjectReference<deoglShaderUnitSourceCode> Ref;
	
	
private:
	decString pName, pSourceCode, pValidationString;
	GLenum pStage;
	decStringSet pDefines;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create source code. */
	deoglShaderUnitSourceCode(deGraphicOpenGl &ogl, const decPath &path);
	
protected:
	/** Cleans up the source code object. */
	~deoglShaderUnitSourceCode() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{return pName;}
	
	/** Source code. */
	inline const decString &GetSourceCode() const{return pSourceCode;}
	
	/** Used defines for filtering. */
	inline const decStringSet &GetDefines() const{return pDefines;}
	
	/** Validation string. */
	inline const decString &GetValidationString() const{return pValidationString;}
	
	/** Stage. */
	inline GLenum GetStage() const{return pStage;}
	
	/** Log stage name. */
	const char *GetLogStageName() const;
	/*@}*/
	
	
private:
	const decXmlElementTag *pGetTagAt(const decXmlElementTag &tag, int index) const;
};

#endif
