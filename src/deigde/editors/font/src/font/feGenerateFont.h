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

#ifndef _FEGENERATEFONT_H_
#define _FEGENERATEFONT_H_

#include <deigde/gui/resources/igdeFont.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/resources/font/deFontReference.h>

class feFontImage;
class feFontGlyph;
class feFont;

class igdeEnvironment;



/**
 * \brief Generate Font.
 */
class feGenerateFont{
private:
	igdeEnvironment &pEnvironment;
	
	int pFirstCode;
	int pLastCode;
	int pMinImageWidth;
	int pMaxImageWidth;
	int pBorderSize;
	int pEnlargeGlyph;
	igdeFont::sConfiguration pFontConfig;
	deFontReference pSystemFont;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font generator. */
	feGenerateFont( igdeEnvironment &environment );
	
	/** \brief Clean up font generator. */
	~feGenerateFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set font configuration to use for generating. */
	void SetFontConfig( const igdeFont::sConfiguration &config );
	
	/** \brief Set range to generate glyphs for. */
	void SetCodeRange( int first, int last );
	
	/** \brief Border size in pixels. */
	inline int GetBorderSize() const{ return pBorderSize; }
	
	/** \brief Set border size in pixels. */
	void SetBorderSize( int borderSize );
	
	/** \brief Amount of pixels to enlarge the glyph box with. */
	inline int GetEnlargeGlyph() const{ return pEnlargeGlyph; }
	
	/** \brief Set amount of pixels to enlarge the glyph box with. */
	void SetEnlargeGlpyh( int enlarge );
	
	/** \brief Generate font with properties. */
	feFont *GenerateFont();
	/*@}*/
	
	
	
private:
	void pAddGlyphs( feFont &font );
	void pCalcLayout( feFont &font );
	void pRenderImage( feFont &font );
};

#endif
