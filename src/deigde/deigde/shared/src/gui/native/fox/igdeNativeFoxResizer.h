/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENATIVEFOXRESIZER_H_
#define _IGDENATIVEFOXRESIZER_H_

#include "foxtoolkit.h"


/**
 * \brief FOX toolkit Native resizer.
 * 
 * When dragged sends SEL_COMMAND with the drag distance as integer parameter.
 * The receiver is responsible to resize according to the dragged distance.
 */
class igdeNativeFoxResizer : public FXWindow{
	FXDECLARE( igdeNativeFoxResizer )
protected:
	   igdeNativeFoxResizer();
	
private:
	bool pIsDragging;
	FXCursor *pCursor;
	int pGripSize;
	int pGripWidth;
	FXColor pActiveColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resizer. */
	igdeNativeFoxResizer( FXComposite *parent, FXObject *target, FXSelector selector );
	
	/** \brief Clean up resizer. */
	virtual ~igdeNativeFoxResizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default height. */
	virtual FXint getDefaultHeight();
	
	/** \brief Dragged distance from SEL_COMMAND event data object. */
	static int SelCommandDraggedDistance( void *data );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onPaint( FXObject *sender, FXSelector selector, void *data );
	long onEnter( FXObject *sender, FXSelector selector, void *data );
	long onLeave( FXObject *sender, FXSelector selector, void *data );
	long onLeftMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onLeftMouseUp( FXObject *sender, FXSelector selector, void *data );
	long onMouseMoved( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
