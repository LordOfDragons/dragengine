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

#ifndef _DEOGLFONT_H_
#define _DEOGLFONT_H_

#include "../deoglBasics.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicFont.h>

class deFont;
class deGraphicOpenGl;
class deoglRFont;



/**
 * \brief Font peer.
 */
class deoglFont : public deBaseGraphicFont{
private:
	const deFont &pFont;
	
	deoglRFont *pRFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font. */
	deoglFont( deGraphicOpenGl &ogl, const deFont &font );
	
	/** \brief Clean up font. */
	virtual ~deoglFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font resource. */
	inline const deFont &GetFont() const{ return pFont; }
	
	/** \brief Render font. */
	inline deoglRFont *GetRFont() const{ return pRFont; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Update font. */
	virtual void Update( float elapsed );
	/*@}*/
};

#endif
