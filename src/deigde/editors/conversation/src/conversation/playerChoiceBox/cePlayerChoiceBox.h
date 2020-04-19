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

#ifndef _CEPLAYERCHOICEBOX_H_
#define _CEPLAYERCHOICEBOX_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#include "cePCBOptionList.h"

class deCanvasView;
class deFont;
class deGraphicContext;
class ceConversation;



/**
 * \brief Player choice box in a conversation.
 */
class cePlayerChoiceBox{
private:
	ceConversation &pConversation;
	
	deFont *pEngFont;
	
	decString pPathFont;
	decColor pBackgroundColor;
	decColor pTextColor;
	decColor pSelectedBackgroundColor;
	decColor pSelectedTextColor;
	int pTextSize;
	int pPadding;
	int pPlaybackStackDepth;
	
	cePCBOptionList pOptions;
	int pSelectedOption;
	
	deCanvasView *pCanvasView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text box. */
	cePlayerChoiceBox( ceConversation &conversation );
	
	/** \brief Clean up text box. */
	~cePlayerChoiceBox();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return pConversation; }
	
	/** \brief Engine font. */
	inline deFont *GetFont() const{ return pEngFont; }
	
	/** \brief Canvas view. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
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
	
	/** \brief Selected background color. */
	inline const decColor &GetSelectedBackgroundColor() const{ return pSelectedBackgroundColor; }
	
	/** \brief Set selected background color. */
	void SetSelectedBackgroundColor( const decColor &color );
	
	/** \brief Selected text color. */
	inline const decColor &GetSelectedTextColor() const{ return pSelectedTextColor; }
	
	/** \brief Set selected text color. */
	void SetSelectedTextColor( const decColor &color );
	
	/** \brief Text size. */
	inline int GetTextSize() const{ return pTextSize; }
	
	/** \brief Set text size. */
	void SetTextSize( int size );
	
	/** \brief Text padding. */
	inline int GetPadding() const{ return pPadding; }
	
	/** \brief Set text padding. */
	void SetPadding( int padding );
	
	/** \brief Playback stack depth to return to after selecting an option. */
	inline int GetPlaybackStackDepth() const{ return pPlaybackStackDepth; }
	
	/** \brief Set playback stack depth to return to after selecting an option. */
	void SetPlaybackStackDepth( int depth );
	
	/** \brief List of options. */
	inline cePCBOptionList &GetOptionList(){ return pOptions; }
	inline const cePCBOptionList &GetOptionList() const{ return pOptions; }
	
	/** \brief Index of the selected option or -1 if there is none. */
	inline int GetSelectedOption() const{ return pSelectedOption; }
	
	/** \brief Set index of the selected option or -1 if there is none. */
	void SetSelectedOption( int index );
	
	/** \brief Index of the option under the given cursor position or -1 if there is none. */
	int IndexOfOptionAt( int x, int y ) const;
	
	/** \brief Select option under the given cursor position if located over an option. */
	void SelectOptionAt( int x, int y );
	
	/** \brief Clear player choice box. */
	void Clear();
	
	/**
	 * \brief Update canvas.
	 * \details Call if:
	 *          - Text changed
	 *          - Selection changed
	 *          - Parent size changed
	 *          - Canvas has been added to a parent.
	 */
	void UpdateCanvas();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateFont();
};

#endif
