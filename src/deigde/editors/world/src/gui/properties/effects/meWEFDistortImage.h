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
#ifndef _MEWEFDISTORTIMAGE_H_
#define _MEWEFDISTORTIMAGE_H_

// includes
#include <stdio.h>
#include <stdlib.h>

#include <deigde/gui/foxtoolkit.h>

// predefinitions
class meWindowEffects;
class deEffectDistortImage;
class deEngine;



/**
 * Panel for editing an overlay image effect.
 */
class meWEFDistortImage : public FXVerticalFrame{
	FXDECLARE( meWEFDistortImage )
protected:
	meWEFDistortImage();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_EDITIMAGE,
		ID_EDITSTRENGTHU,
		ID_EDITSTRENGTHV,
		ID_CHKENABLE,
		ID_LAST
	};
	
private:
	meWindowEffects *pWndEffects;
	deEffectDistortImage *pEffectDistort;
	deEngine *pEngine;
	
	FXTextField *pEditImage;
	FXTextField *pEditStrengthU;
	FXTextField *pEditStrengthV;
	FXCheckButton *pChkEnable;
	
public:
	// constructor, destructor
	meWEFDistortImage( deEffectDistortImage *effectDistort, meWindowEffects *windowEffects, FXComposite *container );
	virtual ~meWEFDistortImage();
	
	// management
	
	// events
	long onEditImageCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditStrengthUCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditStrengthVCommand( FXObject *sender, FXSelector selector, void *data );
	long onCheckEnableCommand( FXObject *sender, FXSelector selector, void *data );
};

// end of include only once
#endif

#endif
