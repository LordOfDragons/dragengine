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

#ifndef _DEOGLRDYNAMICSKIN_H_
#define _DEOGLRDYNAMICSKIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglRDSRenderable;
class deoglRenderThread;



/**
 * Render dynamic skin.
 */
class deoglRDynamicSkin : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	decObjectList pRenderables;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin. */
	deoglRDynamicSkin( deoglRenderThread &renderThread );
	
protected:
	/** Clean up render dynamic skin. */
	virtual ~deoglRDynamicSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Number of renderables. */
	int GetRenderableCount() const;
	
	/** Renderable at index. */
	deoglRDSRenderable *GetRenderableAt( int index ) const;
	
	/** Named renderable or \em NULL if absent. */
	deoglRDSRenderable *GetRenderableNamed( const char *name ) const;
	
	/** Index of named renderable or -1 if absent. */
	int IndexOfRenderableNamed( const char *name ) const;
	
	/** Remove all renderables. */
	void RemoveAllRenderables();
	
	/** Add renderable. */
	void AddRenderable( deoglRDSRenderable *renderable );
	/*@}*/
};

#endif
