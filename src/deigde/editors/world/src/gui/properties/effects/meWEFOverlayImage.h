/* 
 * Drag[en]gine IGDE World Editor
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

#if 0

// include only once
#ifndef _MEWEFOVERLAYIMAGE_H_
#define _MEWEFOVERLAYIMAGE_H_

// includes
#include <stdio.h>
#include <stdlib.h>

#include <deigde/gui/foxtoolkit.h>

// predefinitions
class meWindowEffects;
class deEffectOverlayImage;
class deEngine;



/**
 * Panel for editing an overlay image effect.
 */
class meWEFOverlayImage : public FXVerticalFrame{
	FXDECLARE( meWEFOverlayImage )
protected:
	meWEFOverlayImage();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_EDITIMAGE,
		ID_EDITTRANSPARENCY,
		ID_EDITCORNER1X,
		ID_EDITCORNER1Y,
		ID_EDITCORNER2X,
		ID_EDITCORNER2Y,
		ID_EDITCORNER3X,
		ID_EDITCORNER3Y,
		ID_EDITCORNER4X,
		ID_EDITCORNER4Y,
		ID_CHKENABLE,
		ID_LAST
	};
	
private:
	meWindowEffects *pWndEffects;
	deEffectOverlayImage *pEffectOverlay;
	deEngine *pEngine;
	
	FXTextField *pEditImage;
	FXTextField *pEditTransparency;
	FXTextField *pEditCorner1X;
	FXTextField *pEditCorner1Y;
	FXTextField *pEditCorner2X;
	FXTextField *pEditCorner2Y;
	FXTextField *pEditCorner3X;
	FXTextField *pEditCorner3Y;
	FXTextField *pEditCorner4X;
	FXTextField *pEditCorner4Y;
	FXCheckButton *pChkEnable;
	
public:
	// constructor, destructor
	meWEFOverlayImage( deEffectOverlayImage *effectOverlay, meWindowEffects *windowEffects, FXComposite *container );
	virtual ~meWEFOverlayImage();
	
	// management
	
	// events
	long onEditImageCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditTransparencyCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner1XCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner1YCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner2XCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner2YCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner3XCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner3YCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner4XCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditCorner4YCommand( FXObject *sender, FXSelector selector, void *data );
	long onCheckEnableCommand( FXObject *sender, FXSelector selector, void *data );
};

// end of include only once
#endif

#endif
