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

#ifndef _DECANVASRENDERWORLD_H_
#define _DECANVASRENDERWORLD_H_

#include "deCanvas.h"
#include "../camera/deCamera.h"


/**
 * \brief Canvas rendering a 3d world.
 */
class DE_DLL_EXPORT deCanvasRenderWorld : public deCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCanvasRenderWorld> Ref;
	
	
private:
	deCamera::Ref pCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	deCanvasRenderWorld(deCanvasManager *manager);
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCanvasRenderWorld();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Camera or NULL if not set. */
	inline deCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or NULL to unset. */
	void SetCamera(deCamera *camera);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void Visit(deCanvasVisitor &visitor);
	/*@}*/
};

#endif
