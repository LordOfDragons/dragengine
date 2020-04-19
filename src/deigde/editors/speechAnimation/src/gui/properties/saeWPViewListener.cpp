/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include <stdlib.h>
#include <string.h>

#include "saeWPView.h"
#include "saeWPViewListener.h"



// Class saeWPViewListener
////////////////////////////

// Constructor, destructor
////////////////////////////

saeWPViewListener::saeWPViewListener( saeWPView &panel ) :
pPanel( panel ){
}

saeWPViewListener::~saeWPViewListener(){
}



// Management
///////////////

void saeWPViewListener::SAnimationChanged( saeSAnimation *sanimation ){
	pPanel.UpdateView();
}

void saeWPViewListener::ModelChanged( saeSAnimation *sanimation ){
	pPanel.UpdateView();
}

void saeWPViewListener::SkyChanged( saeSAnimation *sanimation ){
	pPanel.UpdateSky();
}

void saeWPViewListener::CameraChanged( saeSAnimation *sanimation ){
	pPanel.UpdateCamera();
}
