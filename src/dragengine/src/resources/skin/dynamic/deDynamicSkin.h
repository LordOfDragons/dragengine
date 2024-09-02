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

#ifndef _DEDYNAMICSKIN_H_
#define _DEDYNAMICSKIN_H_

#include "../../deResource.h"

class deDSRenderable;
class deDynamicSkinManager;
class deBaseGraphicDynamicSkin;


/**
 * \brief Dynamic Skin.
 *
 * Defines dynamic properties of a skin resource.
 */
class DE_DLL_EXPORT deDynamicSkin : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deDynamicSkin> Ref;
	
	
	
private:
	deDSRenderable **pRenderables;
	int pRenderableCount;
	int pRenderableSize;
	
	deBaseGraphicDynamicSkin *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new resource. */
	deDynamicSkin( deDynamicSkinManager *manager );
	
protected:
	/**
	 * \brief Clean up resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deDynamicSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of renderables. */
	inline int GetRenderableCount() const{ return pRenderableCount; }
	
	/** \brief Renderable at the given index. */
	deDSRenderable *GetRenderableAt( int index ) const;
	
	/** \brief Renderable with the given name or NULL if not found. */
	deDSRenderable *GetRenderableNamed( const char *name ) const;
	
	/** \brief Determines if a renderable exists. */
	bool HasRenderable( deDSRenderable *renderable ) const;
	
	/** \brief Determines if a renderable with the given name exists. */
	bool HasRenderableNamed( const char *name ) const;
	
	/** \brief Index of the renderable or -1 if not found. */
	int IndexOfRenderable( deDSRenderable *renderable ) const;
	
	/** \brief Index of the renderable with the given name or -1 if not found. */
	int IndexOfRenderableNamed( const char *name ) const;
	
	/** \brief Adds a renderable. */
	void AddRenderable( deDSRenderable *renderable );
	
	/** \brief Removes a renderable. */
	void RemoveRenderable( deDSRenderable *renderable );
	
	/** \brief Removes a renderable with the given name if existing. */
	void RemoveRenderableNamed( const char *name );
	
	/** \brief Removes all renderables. */
	void RemoveAllRenderables();
	
	/** \brief Notifies the peer that the given renderable changed. */
	void NotifyRenderableChanged( int index );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicDynamicSkin *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic( deBaseGraphicDynamicSkin *peer );
	/*@}*/
};

#endif
