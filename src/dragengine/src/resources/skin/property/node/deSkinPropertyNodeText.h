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

#ifndef _DESKINPROPERTYNODETEXT_H_
#define _DESKINPROPERTYNODETEXT_H_

#include "deSkinPropertyNode.h"
#include "../../../font/deFont.h"
#include "../../../../common/string/decString.h"


/**
 * \brief Skin property text node for constructed texture property.
 */
class DE_DLL_EXPORT deSkinPropertyNodeText : public deSkinPropertyNode{
public:
	/** \brief Mapped. */
	enum eTextMapped{
		etmFontSize, //<! Font size
		etmColorRed, //<! Color red component
		etmColorGreen, //<! Color green component
		etmColorBlue //<! Color blue component
	};
	
	static const int TextMappedCount = etmColorBlue + 1;
	
	
	
private:
	decString pPath;
	deFont::Ref pFont;
	float pFontSize;
	decString pText;
	decColor pColor;
	
	int pTextMapped[ TextMappedCount ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNodeText();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNodeText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font path or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set font path or empty path if not set. */
	void SetPath( const char *path );
	
	/** \brief Font or nullptr if not set. */
	inline const deFont::Ref &GetFont() const{ return pFont; }
	
	/** \brief Set font or nullptr to unset. */
	void SetFont( deFont *font );
	
	/** \brief Font size in canvas units. */
	inline float GetFontSize() const{ return pFontSize; }
	
	/**
	 * \brief Set font size in canvas units.
	 * 
	 * Size is clamped to 0 or larger.
	 */
	void SetFontSize( float size );
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set text color. */
	void SetColor( const decColor &color );
	
	
	
	/** \brief Index of mapped value or -1 to use static value. */
	int GetTextMappedFor( eTextMapped mapped ) const;
	
	/** \brief Set index of mapped value or -1 to use static value. */
	void SetTextMappedFor( eTextMapped mapped, int index );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit( deSkinPropertyNodeVisitor &visitor );
	/*@}*/
};

#endif
