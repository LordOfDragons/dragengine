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

#ifndef _DEOGLSHAPE_H_
#define _DEOGLSHAPE_H_

#include "../deoglBasics.h"

class deoglRenderThread;
#include "../vbo/deoglSharedVBOBlock.h"
class deoglVAO;
class deoglRenderPlan;


/**
 * Shape.
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
		void Set(float nx, float ny, float nz, bool nselector);
		void SetSelTrue(float nx, float ny, float nz);
		void SetSelFalse(float nx, float ny, float nz);
		void SetFrom(const sVBOData &data);
		void SetFromSelTrue(const sVBOData &data);
		void SetFromSelFalse(const sVBOData &data);
	};
	
private:
	deoglRenderThread &pRenderThread;
	deoglSharedVBOBlock::Ref pVBOBlock;
	int pPointOffsetFaces;
	int pPointCountFaces;
	int pPointOffsetLines;
	int pPointCountLines;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shape. */
	deoglShape(deoglRenderThread &renderThread);
	/** Cleans up the shape. */
	virtual ~deoglShape();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the VBO block. */
	deoglSharedVBOBlock *GetVBOBlock();
	/** Add lines data. */
	virtual void AddVBOLines(sVBOData *data) = 0;
	/** Add faces data. */
	virtual void AddVBOFaces(sVBOData *data) = 0;
	
	/** Retrieves the faces point offset. */
	inline int GetPointOffsetFaces() const{ return pPointOffsetFaces; }
	/** Sets the faces point offset. */
	void SetPointOffsetFaces(int pointOffset);
	/** Retrieves the faces point count. */
	inline int GetPointCountFaces() const{ return pPointCountFaces; }
	/** Sets the faces point count. */
	void SetPointCountFaces(int pointCount);
	/** Retrieves the lines point offset. */
	inline int GetPointOffsetLines() const{ return pPointOffsetLines; }
	/** Sets the lines point offset. */
	void SetPointOffsetLines(int pointLines);
	/** Retrieves the lines point count. */
	inline int GetPointCountLines() const{ return pPointCountLines; }
	/** Sets the lines point count. */
	void SetPointCountLines(int pointCount);
	
	/** Retrieves the VAO from the vbo block. */
	deoglVAO *GetVAO();
	/** Activates the VAO from the vbo block. */
	void ActivateVAO();
	
	/** Render lines. */
	void RenderLines();
	
	/** Render lines. */
	void RenderLines(const deoglRenderPlan &plan);
	
	/** Render faces. */
	void RenderFaces();
	
	/** Render faces. */
	void RenderFaces(const deoglRenderPlan &plan);
	/*@}*/
};

#endif
