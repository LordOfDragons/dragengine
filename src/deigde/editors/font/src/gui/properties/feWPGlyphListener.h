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

#ifndef _FEWPGLYPHLISTENER_H_
#define _FEWPGLYPHLISTENER_H_

#include "../../font/feFontNotifier.h"

class feWPGlyph;



/**
 * \brief Glyph Properties Panel Listener.
 */
class feWPGlyphListener : public feFontNotifier{
private:
	feWPGlyph &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	feWPGlyphListener( feWPGlyph &panel );
	
	/** \brief Clean up listener. */
	virtual ~feWPGlyphListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
    /** \brief Glyphs have been added or removed. */
    virtual void GlyphStructureChanged ( feFont *font );
	
	/** \brief A glyph changed. */
    virtual void GlyphChanged( feFont *font, feFontGlyph *glyph );
	
	/** \brief Active glyph changed. */
	virtual void ActiveGlyphChanged( feFont *font );
	/*@}*/
};

#endif
