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

#ifndef _DEOGLVBOMODELWRITER_H_
#define _DEOGLVBOMODELWRITER_H_

#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglSharedVBOBlock;



/**
 * @brief Model VBO Writer.
 * Helper class writing data to esvbolStaticModel formatted VBO blocks. Keeps track of the data pointer
 * adding one entry after the other. Call Reset with a vbo block data pointer to start the writing.
 * Each call of WritePoint appends the data and advances the pointer for the next point.
 */
class deoglVBOWriterModel{
public:
	deoglRenderThread &pRenderThread;
	char *pDataPoints;
	GLuint *pDataIndices;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vbo writer. */
	deoglVBOWriterModel( deoglRenderThread &renderThread );
	/** Cleans up the vbo writer. */
	~deoglVBOWriterModel();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Sets the data pointers to start writing points and indices to. */
	void Reset( deoglSharedVBOBlock *vboBlock );
	
	/** Writes a point and advances the pointer for the next write. */
	void WritePoint( const decVector &position, const decVector &normal, const decVector &tangent,
		bool negateTangent, const decVector2 &texCoord );
	/** Writes a texture coordinate set point and advance the pointer for the next write. */
	void WriteTexCoordSetPoint( const decVector &tangent, bool negateTangent, const decVector2 &texCoord );
	/** \brief Write weight index. */
	void WriteWeight( int weight );
	
	/** Writes 3 indices and advances the pointer for the next write. */
	void WriteIndices( int index1, int index2, int index3 );
	/*@}*/
};

#endif
