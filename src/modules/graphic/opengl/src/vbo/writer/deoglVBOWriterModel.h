/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
