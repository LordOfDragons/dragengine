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

#ifndef _DEOGLSHAPE_H_
#define _DEOGLSHAPE_H_

#include "../deoglBasics.h"

class deoglRenderThread;
class deoglSharedVBOBlock;
class deoglVAO;
class deoglRenderPlan;


/**
 * @brief Shape.
 */
class deoglShape{
public:
	/** Structure to write VBO data. */
	struct sVBOData{
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat selector;
		
		/** Helper functions to set the data in a more tidy way. */
		void Set( float nx, float ny, float nz, bool nselector );
		void SetSelTrue( float nx, float ny, float nz );
		void SetSelFalse( float nx, float ny, float nz );
		void SetFrom( const sVBOData &data );
		void SetFromSelTrue( const sVBOData &data );
		void SetFromSelFalse( const sVBOData &data );
	};
	
private:
	deoglRenderThread &pRenderThread;
	deoglSharedVBOBlock *pVBOBlock;
	int pPointOffsetFaces;
	int pPointCountFaces;
	int pPointOffsetLines;
	int pPointCountLines;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	deoglShape( deoglRenderThread &renderThread );
	/** Cleans up the shape. */
	virtual ~deoglShape();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the VBO block. */
	deoglSharedVBOBlock *GetVBOBlock();
	/** Add lines data. */
	virtual void AddVBOLines( sVBOData *data ) = 0;
	/** Add faces data. */
	virtual void AddVBOFaces( sVBOData *data ) = 0;
	
	/** Retrieves the faces point offset. */
	inline int GetPointOffsetFaces() const{ return pPointOffsetFaces; }
	/** Sets the faces point offset. */
	void SetPointOffsetFaces( int pointOffset );
	/** Retrieves the faces point count. */
	inline int GetPointCountFaces() const{ return pPointCountFaces; }
	/** Sets the faces point count. */
	void SetPointCountFaces( int pointCount );
	/** Retrieves the lines point offset. */
	inline int GetPointOffsetLines() const{ return pPointOffsetLines; }
	/** Sets the lines point offset. */
	void SetPointOffsetLines( int pointLines );
	/** Retrieves the lines point count. */
	inline int GetPointCountLines() const{ return pPointCountLines; }
	/** Sets the lines point count. */
	void SetPointCountLines( int pointCount );
	
	/** Retrieves the VAO from the vbo block. */
	deoglVAO *GetVAO();
	/** Activates the VAO from the vbo block. */
	void ActivateVAO();
	
	/** Render lines. */
	void RenderLines();
	
	/** Render lines. */
	void RenderLines( const deoglRenderPlan &plan );
	
	/** Render faces. */
	void RenderFaces();
	
	/** Render faces. */
	void RenderFaces( const deoglRenderPlan &plan );
	/*@}*/
};

#endif
