/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeTaskSyncGameDefinition.h"
#include "aeWindowMain.h"
#include "../animator/aeAnimator.h"
#include "../animator/attachment/aeAttachment.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/exceptions.h>



// Class aeTaskSyncGameDefinition
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeTaskSyncGameDefinition::aeTaskSyncGameDefinition(aeWindowMain &windowMain) :
pWindowMain(windowMain)
{
	SetMessage("Animator Editor: Synchronize Game Definition");
}

aeTaskSyncGameDefinition::~aeTaskSyncGameDefinition(){
}



// Management
///////////////

bool aeTaskSyncGameDefinition::Step(){
	if(!pWindowMain.GetAnimator()){
		return false;
	}
	
	aeAnimator &animator = *pWindowMain.GetAnimator();
	
	animator.GetSky()->OnGameDefinitionChanged();
	animator.GetEnvObject()->OnGameDefinitionChanged();
	
	const int attachmentCount = animator.GetAttachmentCount();
	int i;
	
	for(i=0; i<attachmentCount; i++){
		animator.GetAttachmentAt(i)->GetObjectWrapper()->OnGameDefinitionChanged();
	}
	
	return false;
}
