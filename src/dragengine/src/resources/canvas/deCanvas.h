/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECANVAS_H_
#define _DECANVAS_H_

#include "../../common/math/decMath.h"
#include "../deResource.h"

class deBaseGraphicCanvas;
class deCanvasManager;
class deCanvasVisitor;
class deCanvasView;


/**
 * \brief Canvas rendering 2D content.
 * 
 * Canvas are the basic elements of 2D rendering. At the top stands a deCanvasView
 * representing a view on a set of child canvas. A canvas has a position and size
 * relative to the parent canvas view coordinate system. The position of the canvas
 * is the top left corner and the size spawns to the right bottom corner. An
 * additional transformation matrix allows to apply transformation effects to the
 * canvas and its coordinate system. The canvas coordinate system maps (0,0) to the
 * top left corner of the canvas and (size.x,size.y) to the lower right corner. This
 * coordinate system is then eventually transformed by the transformation matrix.
 * Units are measured as pixels unless transformed. Canvas can be set invisible
 * hiding the canvas and all content inside. The order of the canvas defines the
 * render order with canvas of higher order rendered above those with smaller number.
 * This uncouples the render ordering from the actual ordering of the canvas objects
 * in a deCanvasView. If two canvas have the same order the render order is undefined.
 */
class deCanvas : public deResource{
public:
	/** \brief Blend modes used to blend canvas over previous content. */
	enum eBlendModes{
		/**
		 * \brief Blend canvas over previous content.
		 * 
		 * Pixel color is <code>oldColor * (1-transparency) + newColor * transparency</code>.
		 * This is the default blend mode.
		 */
		ebmBlend,
		
		/**
		 * \brief Add canvas over previous content.
		 * 
		 * Pixel color is <code>oldColor + newColor * transparency</code>.
		 */
		ebmAdd
	};
	
	
	
private:
	decPoint pPosition;
	decPoint pSize;
	decTexMatrix2 pTransform;
	decColorMatrix pColorTransform;
	bool pVisible;
	float pOrder;
	float pTransparency;
	eBlendModes pBlendMode;
	
	deBaseGraphicCanvas *pPeerGraphic;
	
	deCanvasView *pParentView;
	deCanvas *pLLViewPrev;
	deCanvas *pLLViewNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create canvas.
	 * 
	 * The default size is set to 20x20 units.
	 */
	deCanvas( deCanvasManager *manager );
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCanvas();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position in parent canvas view units. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set position in parent canvas view units. */
	void SetPosition( const decPoint &position );
	
	/** \brief Size in parent canvas view units. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Set size in parent canvas view units. */
	void SetSize( const decPoint &size );
	
	/** \brief Transformation matrix affecting the canvas and all its content. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set transformation matrix affecting the canvas and all its content. */
	void SetTransform( const decTexMatrix2 &transform );
	
	/** \brief Transformation matrix affecting the canvas content color. */
	inline const decColorMatrix &GetColorTransform() const{ return pColorTransform; }
	
	/** \brief Set transformation matrix affecting the canvas content color. */
	void SetColorTransform( const decColorMatrix &transform );
	
	/** \brief Canvas is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if canvas is visible. */
	void SetVisible( bool visible );
	
	/** \brief Render order. Larger renders ontop of smaller. */
	inline float GetOrder() const{ return pOrder; }
	
	/** \brief Set render order. Larger renders ontop of smaller. */
	void SetOrder( float order );
	
	/** \brief Transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	void SetTransparency( float transparency );
	
	/** \brief Blend mode used to blend canvas over previous content. */
	inline eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/** \brief Set blend mode used to blend canvas over previous content. */
	void SetBlendMode( eBlendModes blendMode );
	
	
	
	/** \brief Notify peers about changes to the canvas content. */
	void NotifyContentChanged();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object or NULL if not set. */
	inline deBaseGraphicCanvas *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object or NULL if not set. */
	void SetPeerGraphic( deBaseGraphicCanvas *peer );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void Visit( deCanvasVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent view or NULL if not set. */
	inline deCanvasView *GetParentView() const{ return pParentView; }
	
	/** \brief Set parent view or NULL if not set. */
	void SetParentView( deCanvasView *view );
	
	/** \brief Previous canvas in the parent view linked list or NULL if not set. */
	inline deCanvas *GetLLViewPrev() const{ return pLLViewPrev; }
	
	/** \brief Set next canvas in the parent view linked list or NULL if not set. */
	void SetLLViewPrev( deCanvas *canvas );
	
	/** \brief Next canvas in the parent view linked list or NULL if not set. */
	inline deCanvas *GetLLViewNext() const{ return pLLViewNext; }
	
	/** \brief Set next canvas in the parent view linked list NULL if not set. */
	void SetLLViewNext( deCanvas *canvas );
	/*@}*/
};

#endif
