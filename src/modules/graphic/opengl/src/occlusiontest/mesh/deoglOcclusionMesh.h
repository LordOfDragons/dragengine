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

#ifndef _DEOGLOCCLUSIONMESH_H_
#define _DEOGLOCCLUSIONMESH_H_

#include "deoglROcclusionMesh.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicOcclusionMesh.h>

class deOcclusionMesh;
class deGraphicOpenGl;



/**
 * Occlusion mesh peer.
 */
class deoglOcclusionMesh : public deBaseGraphicOcclusionMesh{
public:
	deGraphicOpenGl &pOgl;
	const deOcclusionMesh &pOcclusionMesh;
	deoglROcclusionMesh::Ref pROcclusionMesh;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new peer. */
	deoglOcclusionMesh(deGraphicOpenGl &ogl, const deOcclusionMesh &occlusionMesh);
	
	/** Clean up peer. */
	virtual ~deoglOcclusionMesh();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Occlusion mesh. */
	inline const deOcclusionMesh &GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** Render occlusion mesh. */
	inline const deoglROcclusionMesh::Ref &GetROcclusionMesh() const{ return pROcclusionMesh; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
