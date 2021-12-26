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

#ifndef _DEDEBUGBLOCKINFO_H_
#define _DEDEBUGBLOCKINFO_H_

#include "../resources/canvas/deCanvasViewReference.h"
#include "../resources/canvas/deCanvasTextReference.h"
#include "../resources/canvas/deCanvasPaintReference.h"
#include "../resources/font/deFontReference.h"
#include "../common/collection/decPointerList.h"
#include "../common/math/decMath.h"
#include "../common/string/decString.h"


class deEngine;


/**
 * \brief Debug overlay block displaying vertically aligned list of key value pairs.
 * 
 * The individual entries can be color coded.
 */
class DE_DLL_EXPORT deDebugBlockInfo{
private:
	struct sEntry{
		decString label;
		decString text;
		decColor color;
		decColor bgColor;
		deCanvasViewReference view;
		deCanvasPaintReference viewBg;
		deCanvasTextReference viewLabel;
		deCanvasTextReference viewText;
	};
	
	
	deEngine &pEngine;
	decPointerList pEntries;
	deCanvasViewReference pView;
	deCanvasPaintReference pViewTitleBg;
	deCanvasTextReference pViewTitle;
	deFontReference pFont;
	int pPadding;
	int pSpacing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug overlay block info. */
	deDebugBlockInfo( deEngine &engine );
	
	/** \brief Clean up debug overlay block info. */
	~deDebugBlockInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas view to be added to debug overlay. */
	inline deCanvasView *GetView() const{ return pView; }
	
	/** \brief Font. */
	inline deFont *GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont( deFont *font );
	
	/** \brief Padding. */
	inline int GetPadding() const{ return pPadding; }
	
	/** \brief Set padding. */
	void SetPadding( int padding );
	
	/** \brief Spacing. */
	inline int GetSpacing() const{ return pSpacing; }
	
	/** \brief Set spacing. */
	void SetSpacing( int spacing );
	
	
	
	/** \brief Add to debug overlay. */
	void AddToOverlay();
	
	/** \brief Remove from debug overlay. */
	void RemoveFromOverlay();
	
	
	
	/** \brief Set title. */
	void SetTitle( const char *title );
	
	/** \brief Set title color. */
	void SetTitleColor( const decColor &color );
	
	/** \brief Set title background color. */
	void SetTitleBgColor( const decColor &color );
	
	
	
	/** \brief Number of entries. */
	int GetEntryCount() const;
	
	/** \brief Add entry. */
	void AddEntry( const char *label, const char *text,
		const decColor &color = decColor( 1.0f, 1.0f, 1.0f ), 
		const decColor &bgColor = decColor( 0.0f, 0.0f, 0.0f, 0.5f ) );
	
	/** \brief Set entry label. */
	void SetEntryLabel( int index, const char *label );
	
	/** \brief Set entry text. */
	void SetEntryText( int index, const char *text );
	
	/** \brief Set entry text color. */
	void SetEntryColor( int index, const decColor &color );
	
	/** \brief Set entry background color. */
	void SetEntryBgColor( int index, const decColor &color );
	
	/** \brief Remove entry. */
	void RemoveEntry( int index );
	
	/** \brief Remove all entries. */
	void RemoveAllEntries();
	
	
	
	/**
	 * \brief Update view.
	 * 
	 * Call this after changing information.
	 */
	void UpdateView();
	
	
	
	/** \brief Text size. */
	static decPoint pTextSize( const deFont &font, const char *text );
	/*@}*/
};

#endif
