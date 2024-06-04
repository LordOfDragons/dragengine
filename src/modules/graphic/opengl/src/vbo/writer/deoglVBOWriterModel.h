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

#ifndef _DEOGLVBOMODELWRITER_H_
#define _DEOGLVBOMODELWRITER_H_

#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglSharedVBOBlock;



/**
 * Model VBO Writer.
 */
class deoglVBOWriterModel{
public:
	deoglRenderThread &pRenderThread;
	GLfloat *pDataPoints;
	GLuint *pDataIndices;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vbo writer. */
	deoglVBOWriterModel( deoglRenderThread &renderThread );
	
	/** Clean up vbo writer. */
	~deoglVBOWriterModel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set data pointers to start writing points and indices to. */
	void Reset( deoglSharedVBOBlock *vboBlock );
	
	/** Write point and advances the pointer for the next write. */
	void WritePoint( const decVector &position, const decVector &normal, const decVector &tangent,
		bool negateTangent, const decVector2 &texCoord, const decVector &realNormal );
	
	/** Write point and advances the pointer for the next write. */
	void WritePoint( const decVector &position, const decVector &normal, const decVector &tangent,
		bool negateTangent, const decVector2 &texCoord, const decVector &realNormal, int weights );
	
	/** Write texture coordinate set point and advance the pointer for the next write. */
	void WriteTexCoordSetPoint( const decVector &tangent, bool negateTangent, const decVector2 &texCoord );
	
	/** Write 3 indices and advances the pointer for the next write. */
	void WriteIndices( int index1, int index2, int index3 );
	/*@}*/
};

#endif
