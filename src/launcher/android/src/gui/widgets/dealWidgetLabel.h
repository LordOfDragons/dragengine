/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALWIDGETLABEL_H_
#define _DEALWIDGETLABEL_H_

#include "dealWidget.h"
#include "../../common/string/decString.h"


class dealFont;


/**
 * \brief Label widget.
 */
class dealWidgetLabel : public dealWidget{
public:
	/** \brief Alignment. */
	enum eAlignments{
		/** \brief Left alignment. */
		eaLeft = 0x01,
		
		/** \brief Center alignment. */
		eaCenter = 0x02,
		
		/** \brief Right alignment. */
		eaRight = 0x03,
		
		/** \brief Top alignment. */
		eaTop = 0x10,
		
		/** \brief Middle alignment. */
		eaMiddle = 0x20,
		
		/** \brief Bottom alignment. */
		eaBottom = 0x30
	};
	
private:
	dealFont *pFont;
	int pFontSize;
	decString pText;
	decColor pColor;
	eAlignments pAlignment;
	int pMaxLineWidth;
	
	decPoint pTextSize;
	bool pDirtyTextSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLabel( dealDisplay &display, const char *text );
	
	/** \brief Create widget. */
	dealWidgetLabel( dealDisplay &display, dealFont *font, int fontSize, const char *text );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLabel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font or \em NULL if not set. */
	inline dealFont *GetFont() const{ return pFont; }
	
	/** \brief Set font or \em NULL if not set. */
	void SetFont( dealFont *font );
	
	/** \brief Font size in pixels. */
	inline int GetFontSize() const{
		return pFontSize;
	}
	
	/** \brief Set font size in pixels. */
	void SetFontSize( int fontSize );
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set text color. */
	void SetColor( const decColor &color );
	
	/** \brief Alignment. */
	inline eAlignments GetAlignment() const{ return pAlignment; }
	
	/** \brief Set alignment. */
	void SetAlignment( eAlignments alignment );
	
	/** \brief Maximum line width. */
	inline int GetMaxLineWidth() const{ return pMaxLineWidth; }
	
	/** \brief Set maximum line width. */
	void SetMaxLineWidth( int lineWidth );
	
	
	
	/** \brief Minimum size of widget. */
	virtual decPoint GetMinimumSize();
	
	
	
	/** \brief Render content. */
	virtual void RenderContent( const sRenderContext &context );
	
	
	
	/** \brief Update text size if dirty. */
	void UpdateTextSize();
	/*@}*/
};



/** \brief Helper for combining alignments. */
inline dealWidgetLabel::eAlignments operator|( dealWidgetLabel::eAlignments a, dealWidgetLabel::eAlignments b ){
	return ( dealWidgetLabel::eAlignments )( ( int )a | ( int )b );
}

inline dealWidgetLabel::eAlignments operator&( dealWidgetLabel::eAlignments a, dealWidgetLabel::eAlignments b ){
	return a = ( dealWidgetLabel::eAlignments )( ( int )a & ( int )b );
}

#endif
