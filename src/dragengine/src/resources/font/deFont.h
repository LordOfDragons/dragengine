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

#ifndef _DEFONT_H_
#define _DEFONT_H_

#include "deFontGlyph.h"
#include "../deFileResource.h"
#include "../image/deImageReference.h"
#include "../../common/math/decMath.h"

class deFontManager;
class deBaseGraphicFont;
class decUnicodeString;


/**
 * \brief Font for use by text output.
 * 
 * Fonts are image based and can be colored or not colored in which case they can be
 * colorized. Colored fonts can not be colorized.
 */
class deFont : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deFont> Ref;
	
	
	
private:
	deFontGlyph pUndefinedGlyph;
	deFontGlyph *pGlyphs;
	int pGlyphCount;
	unsigned short *pGlyphGroups;
	int pGlyphGroupCount;
	unsigned short *pGlyphMap;
	int pGlyphMapCount;
	int pFontWidth;
	int pLineHeight;
	bool pColorFont;
	
	decString pImagePath;
	deImageReference pImage;
	
	deBaseGraphicFont *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font resource. */
	deFont( deFontManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up font.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deFont();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Height of line of text in pixels. */
	inline int GetLineHeight() const{ return pLineHeight; }
	
	/** \brief Set height of line of text in pixels. */
	void SetLineHeight( int lineHeight );
	
	/** \brief Font is colored and can not be tinted. */
	inline bool GetIsColorFont() const{ return pColorFont; }
	
	/** \brief Set if font is colored and can not be tinted. */
	void SetIsColorFont( bool colorFont );
	
	/** \brief Maximum width of all glyphs in pixels. */
	inline int GetFontWidth() const{ return pFontWidth; }
	
	
	
	/** \brief Undefined glyph. */
	inline deFontGlyph &GetUndefinedGlyph(){ return pUndefinedGlyph; }
	inline const deFontGlyph &GetUndefinedGlyph() const{ return pUndefinedGlyph; }
	
	/** \brief Number of glyphs. */
	inline int GetGlyphCount() const{ return pGlyphCount; }
	
	/** \brief Sets number of glyphs resetting all to default values. */
	void SetGlyphCount( int count );
	
	/** \brief Glyph for unicode is defined. */
	bool HasGlyph( int unicode ) const;
	
	/** \brief Glyph at index. */
	deFontGlyph &GetGlyphAt( int index );
	const deFontGlyph &GetGlyphAt( int index ) const;
	
	/**
	 * \brief Glyph for Unicode.
	 * 
	 * If glyph is not defined the undefined glyph is returned.
	 */
	const deFontGlyph &GetGlyph( int unicode ) const;
	
	
	
	/** \brief Update glyphs. */
	void UpdateGlyphs();
	
	/** \brief Verify settings of font are all valid. */
	bool Verify();
	
	
	
	/** \brief Path to image resource or empty string is not set. */
	inline const decString &GetImagePath() const{ return pImagePath; }
	
	/** \brief Set path to image if existing. */
	void SetImagePath( const char *path );
	
	/** \brief Image or NULL. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or NULL. */
	void SetImage( deImage *image );
	
	
	
	/** \brief Measure size of text. */
	decPoint TextSize( const decUnicodeString &text ) const;
	
	/** \brief Measure size of text. */
	decPoint TextSize( const char *text ) const;
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicFont *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicFont *peer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pFreeGlyphMap();
	void pCreateGlyphMap();
};

#endif
