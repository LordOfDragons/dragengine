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

#ifndef _DEDSRENDERABLECAMERA_H_
#define _DEDSRENDERABLECAMERA_H_

#include "deDSRenderable.h"
#include "../../../camera/deCamera.h"


/**
 * \brief Dynamic Skin Renderable Camera.
 *
 * All pixels covered by this renderable are rendered using the given
 * camera. Post processing effects are not applied since they are
 * screen space which is not given here.
 */
class DE_DLL_EXPORT deDSRenderableCamera : public deDSRenderable{
private:
	deCamera::Ref pCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderableCamera( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Camera or NULL if not set. */
	inline deCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or NULL if not set. */
	void SetCamera( deCamera *camera );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
