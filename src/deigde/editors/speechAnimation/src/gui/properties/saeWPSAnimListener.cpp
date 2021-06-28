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

#include "saeWPSAnim.h"
#include "saeWPSAnimListener.h"
#include "../../sanimation/saeSAnimation.h"

#include <dragengine/common/exceptions.h>



// Class saeWPSAnimListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

saeWPSAnimListener::saeWPSAnimListener( saeWPSAnim &panel ) :
pPanel( panel ){
}

saeWPSAnimListener::~saeWPSAnimListener(){
}



// Management
///////////////

void saeWPSAnimListener::SAnimationChanged( saeSAnimation *sanimation ){
	pPanel.UpdateSAnimation();
}



void saeWPSAnimListener::PhonemeStructureChanged( saeSAnimation *sanimation ){
	pPanel.UpdatePhonemeList();
}

void saeWPSAnimListener::PhonemeChanged( saeSAnimation *sanimation, saePhoneme *phoneme ){
	pPanel.UpdatePhonemeList();
}

void saeWPSAnimListener::ActivePhonemeChanged( saeSAnimation *sanimation ){
	pPanel.SelectActivePhoneme();
}



void saeWPSAnimListener::WordStructureChanged( saeSAnimation *sanimation ){
	pPanel.UpdateWordList();
}

void saeWPSAnimListener::WordNameChanged( saeSAnimation *sanimation, saeWord *word ){
	pPanel.UpdateWordList();
}

void saeWPSAnimListener::WordChanged( saeSAnimation *sanimation, saeWord *word ){
	pPanel.UpdateWord();
}

void saeWPSAnimListener::ActiveWordChanged( saeSAnimation *sanimation ){
	pPanel.SelectActiveWord();
}
