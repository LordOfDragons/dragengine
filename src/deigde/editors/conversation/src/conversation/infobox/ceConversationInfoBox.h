/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CECONVERSATIONINFOBOX_H_
#define _CECONVERSATIONINFOBOX_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/collection/decIntList.h>

class deCanvasView;
class deFont;
class deGraphicContext;
class ceConversation;



/**
 * \brief Shows an information box for special actions in a conversation playback.
 */
class ceConversationInfoBox{
private:
	ceConversation &pConversation;
	
	deFont *pEngFont;
	
	decString pPathFont;
	decColor pBackgroundColor;
	decColor pTextColor;
	int pTextSize;
	int pPadding;
	int pWidth;
	int pHeight;
	
	decString pText;
	
	decStringList pLayoutTexts;
	decIntList pLayoutWidths;
	
	deCanvasView *pCanvasView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create info box. */
	ceConversationInfoBox( ceConversation &conversation );
	
	/** \brief Clean up text box. */
	~ceConversationInfoBox();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return pConversation; }
	
	/** \brief Engine font. */
	inline deFont *GetFont() const{ return pEngFont; }
	
	/** \brief Font path. */
	inline const decString &GetPathFont() const{ return pPathFont; }
	
	/** \brief Set font path. */
	void SetPathFont( const char *path );
	
	/** \brief Background color. */
	inline const decColor &GetBackgroundColor() const{ return pBackgroundColor; }
	
	/** \brief Set background color. */
	void SetBackgroundColor( const decColor &color );
	
	/** \brief Text color. */
	inline const decColor &GetTextColor() const{ return pTextColor; }
	
	/** \brief Set text color. */
	void SetTextColor( const decColor &color );
	
	/** \brief Text size. */
	inline int GetTextSize() const{ return pTextSize; }
	
	/** \brief Set text size. */
	void SetTextSize( int size );
	
	/** \brief Text padding. */
	inline int GetPadding() const{ return pPadding; }
	
	/** \brief Set text padding. */
	void SetPadding( int padding );
	
	/** \brief Text or an empty string to hide the box. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text or an empty string to hide the box. */
	void SetText( const char *text );
	
	/** \brief Clear info box. */
	void Clear();
	
	/** \brief Canvas view. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/**
	 * \brief Update canvas.
	 * \details Call if:
	 *          - Text changed
	 *          - Parent size changed
	 *          - Canvas has been added to a parent.
	 */
	void UpdateCanvas();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateFont();
	void pUpdateText();
};

#endif
