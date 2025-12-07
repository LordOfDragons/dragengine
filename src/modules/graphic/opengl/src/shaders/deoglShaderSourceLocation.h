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

#ifndef _DEOGLSHADERSOURCELOCATION_H_
#define _DEOGLSHADERSOURCELOCATION_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * GLSL shader source location to help debugging.
 */
class deoglShaderSourceLocation : public deObject{
private:
	decString pInputFile;
	int pInputLine;
	int pOutputLine;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglShaderSourceLocation> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader source location. */
	deoglShaderSourceLocation(const char *inputFile, int inputLine, int outputLine);
	
	/** Clean up shader source location. */
	virtual ~deoglShaderSourceLocation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Input file. */
	inline const decString &GetInputFile() const{ return pInputFile; }
	
	/** Input line. */
	inline int GetInputLine() const{ return pInputLine; }
	
	/** Output line. */
	inline int GetOutputLine() const{ return pOutputLine; }
	/*@}*/
};

#endif
