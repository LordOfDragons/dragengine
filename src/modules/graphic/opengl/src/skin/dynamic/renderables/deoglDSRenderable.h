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

#ifndef _DEOGLDSRENDERABLE_H_
#define _DEOGLDSRENDERABLE_H_

class deDSRenderable;
class deoglDynamicSkin;
class deoglRDSRenderable;



/**
 * Dynamic skin renderable.
 */
class deoglDSRenderable{
public:
	deoglDynamicSkin &pDynamicSkin;
	const deDSRenderable &pRenderable;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable. */
	deoglDSRenderable( deoglDynamicSkin &dynamicSkin, const deDSRenderable &renderable );
	
	/** Clean up peer. */
	virtual ~deoglDSRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Dynamic skin. */
	inline deoglDynamicSkin &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Renderable. */
	inline const deDSRenderable &GetRenderable() const{ return pRenderable; }
	
	/** Render renderable. */
	virtual deoglRDSRenderable *GetRRenderable() const = 0;
	
	/** Renderable changed. */
	virtual void RenderableChanged() = 0;
	
	/** Update render thread counterpart if required. */
	virtual void SyncToRender() = 0;
	/*@}*/
};

#endif
