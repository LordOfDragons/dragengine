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

#include "saeSAnimationListener.h"



// Class saeSAnimationListener
////////////////////////////////

// Constructor, destructor
////////////////////////////

saeSAnimationListener::saeSAnimationListener(){
}

saeSAnimationListener::~saeSAnimationListener(){
}



// Notifications
//////////////////

void saeSAnimationListener::StateChanged( saeSAnimation* ){
}

void saeSAnimationListener::UndoChanged( saeSAnimation* ){
}

void saeSAnimationListener::ViewChanged( saeSAnimation* ){
}

void saeSAnimationListener::SAnimationChanged( saeSAnimation* ){
}

void saeSAnimationListener::ModelChanged( saeSAnimation* ){
}

void saeSAnimationListener::SkyChanged( saeSAnimation* ){
}

void saeSAnimationListener::CameraChanged( saeSAnimation* ){
}



void saeSAnimationListener::PhonemeStructureChanged( saeSAnimation* ){
}

void saeSAnimationListener::ActivePhonemeChanged( saeSAnimation* ){
}

void saeSAnimationListener::PhonemeChanged( saeSAnimation*, saePhoneme* ){
}



void saeSAnimationListener::WordStructureChanged( saeSAnimation* ){
}

void saeSAnimationListener::WordNameChanged( saeSAnimation*, saeWord* ){
}

void saeSAnimationListener::ActiveWordChanged( saeSAnimation* ){
}

void saeSAnimationListener::WordChanged( saeSAnimation*, saeWord* ){
}
