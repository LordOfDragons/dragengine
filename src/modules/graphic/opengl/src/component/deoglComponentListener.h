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

#ifndef _DEOGLCOMPONENTLISTENER_H_
#define _DEOGLCOMPONENTLISTENER_H_

#include <dragengine/deObject.h>

class deoglRComponent;


/**
 * Render component listener.
 */
class deoglComponentListener : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglComponentListener> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component listener. */
	deoglComponentListener();
	
	/** Clean up component listener. */
	virtual ~deoglComponentListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Component has been destroyed. */
	virtual void ComponentDestroyed(deoglRComponent &component);
	
	/** Component changed parent world. */
	virtual void ParentWorldChanged(deoglRComponent &component);
	
	/** Component changed layer mask. */
	virtual void LayerMaskChanged(deoglRComponent &component);
	
	/** Component changed render static. */
	virtual void RenderStaticChanged(deoglRComponent &component);
	
	/** Component geometry changed or dislocated. */
	virtual void BoundariesChanged(deoglRComponent &component);
	
	/** Occlusion mesh geometry changed. */
	virtual void OcclusionMeshChanged(deoglRComponent &component);
	
	/** Textures changed. */
	virtual void TexturesChanged(deoglRComponent &component);
	
	/** TUC changed. */
	virtual void TUCChanged(deoglRComponent &component);
	
	/** Movement hint changed. */
	virtual void MovementHintChanged(deoglRComponent &component);
	
	/** GI importance changed. */
	virtual void GIImportanceChanged(deoglRComponent &component);
	
	/** Visibility changed. */
	virtual void VisibilityChanged(deoglRComponent &component);
	
	/** Model changed. */
	virtual void ModelChanged(deoglRComponent &component);
	/*@}*/
};

#endif
