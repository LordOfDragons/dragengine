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

#include "deBaseAnimatorAnimatorInstance.h"



// Class deBaseAnimatorAnimatorInstance
/////////////////////////////////////////

// Constructres, destructors
//////////////////////////////

deBaseAnimatorAnimatorInstance::deBaseAnimatorAnimatorInstance(){
}

deBaseAnimatorAnimatorInstance::~deBaseAnimatorAnimatorInstance(){
}



// Notifications
//////////////////

void deBaseAnimatorAnimatorInstance::Apply( bool direct ){
}

void deBaseAnimatorAnimatorInstance::CaptureStateInto( int identifier ){
}

void deBaseAnimatorAnimatorInstance::StoreFrameInto( int identifier, const char *moveName, float moveTime ){
}



void deBaseAnimatorAnimatorInstance::AnimatorChanged(){
}

void deBaseAnimatorAnimatorInstance::ComponentChanged(){
}

void deBaseAnimatorAnimatorInstance::AnimationChanged(){
}

void deBaseAnimatorAnimatorInstance::BlendFactorChanged(){
}

void deBaseAnimatorAnimatorInstance::EnableRetargetingChanged(){
}

void deBaseAnimatorAnimatorInstance::ProtectDynamicBonesChanged(){
}

void deBaseAnimatorAnimatorInstance::ControllerChanged( int index ){
}
