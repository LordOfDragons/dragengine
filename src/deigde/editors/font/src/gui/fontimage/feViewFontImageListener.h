/* 
 * Drag[en]gine IGDE Font Editor
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

#ifndef _FEVIEWFONTIMAGELISTENER_H_
#define _FEVIEWFONTIMAGELISTENER_H_

#include "../../font/feFontNotifier.h"

class feViewFontImage;



/**
 * \brief View font image listener.
 */
class feViewFontImageListener : public feFontNotifier{
private:
	feViewFontImage &pPanel;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	feViewFontImageListener( feViewFontImage &panel );
	
	/** \brief Clean up listener. */
	virtual ~feViewFontImageListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font parameters changed. */
	virtual void FontChanged( feFont *font );
	
	/** \brief An image changed. */
    virtual void ImageChanged( feFont *font, feFontImage *image );
	
	/** \brief Glyphs have been added or removed. */
	virtual void GlyphStructureChanged ( feFont *font );
	
	/** \brief A glyph changed. */
	virtual void GlyphChanged( feFont *font, feFontGlyph *glyph );
	
	/** \brief Glyph selection changed. */
	virtual void GlyphSelectionChanged( feFont *font );
	
	/** \brief Active glyph changed. */
	virtual void ActiveGlyphChanged( feFont *font );
	/*@}*/
};

#endif
