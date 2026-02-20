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

#include <stdlib.h>
#include <string.h>

#include "aeWindowMain.h"
#include "aeWindowMainListener.h"
#include "../animator/aeAnimator.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class aeWindowMainListener
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeWindowMainListener::aeWindowMainListener(aeWindowMain &windowMain) :
pWindowMain(windowMain){
}

aeWindowMainListener::~aeWindowMainListener(){
}



// Notifications
//////////////////

void aeWindowMainListener::StateChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::UndoChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::AnimatorChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::ViewChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::ModelChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::AnimationChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}



void aeWindowMainListener::ControllerChanged(aeAnimator*, aeController*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::ControllerStructureChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}



void aeWindowMainListener::ActiveRuleChanged(aeAnimator*, aeRule*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::RuleChanged(aeAnimator*, aeRule*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::RuleNameChanged(aeAnimator*, aeRule*){
	pWindowMain.UpdateAllActions();
}

void aeWindowMainListener::RuleStructureChanged(aeAnimator*){
	pWindowMain.UpdateAllActions();
}
