/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEPROPERTYNODETEXT_H_
#define _SEPROPERTYNODETEXT_H_

#include "sePropertyNode.h"

class deFont;



/**
 * \brief Texture property text node.
 */
class sePropertyNodeText : public sePropertyNode{
public:
	enum eTextMapped{
		etmFontSize = MappedCount, //<! Font size
		etmColorRed, //<! Color red component
		etmColorGreen, //<! Color green component
		etmColorBlue //<! Color blue component
	};
	
	static const int TextMappedCount = etmColorBlue + 1;
	
	
	
private:
	decString pPath;
	deFont *pFont;
	float pFontSize;
	decString pText;
	decColor pColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	sePropertyNodeText( deEngine &engine );
	
	/** \brief Create copy of node. */
	sePropertyNodeText( const sePropertyNodeText &node );
	
	/** \brief Clean up node. */
	virtual ~sePropertyNodeText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font path or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set font path or empty path if not set. */
	void SetPath( const char *path );
	
	/** \brief Font or \em NULL if not set. */
	inline deFont *GetFont() const{ return pFont; }
	
	/** \brief Font size in canvas units. */
	inline float GetFontSize() const{ return pFontSize; }
	
	/**
	 * \brief Set font size in canvas units.
	 * \details Size is clamped to 0 or larger.
	 */
	void SetFontSize( float size );
	
	/** \brief Update font. */
	void UpdateFont();
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set text color. */
	void SetColor( const decColor &color );
	
	
	
	/** \brief Create copy of node. */
	virtual sePropertyNode *Copy() const;
	
	/** \brief Update resources. */
	virtual void UpdateResources();
	/*@}*/
};

#endif
