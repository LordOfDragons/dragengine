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

#ifndef _DEOGLSKIN_H_
#define _DEOGLSKIN_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicSkin.h>

class deGraphicOpenGl;
class deoglRSkin;
class deSkin;



/**
 * \brief Skin Peer.
 */
class deoglSkin : public deBaseGraphicSkin{
private:
	deGraphicOpenGl &pOgl;
	const deSkin &pSkin;
	
	deoglRSkin *pRSkin;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin. */
	deoglSkin( deGraphicOpenGl &ogl, const deSkin &skin );
	
	/** \brief Clean up skin. */
	virtual ~deoglSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Skin resource. */
	inline const deSkin &GetSkin() const{ return pSkin; }
	
	
	
	/** \brief Render skin. */
	inline deoglRSkin *GetRSkin() const{ return pRSkin; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
