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
#include "meWindowMain.h"
#include "meWindowMainListener.h"
#include "../world/meWorld.h"

#include <deigde/environment/igdeEnvironment.h>

#include "dragengine/common/exceptions.h"



// Class meWindowMainListener
///////////////////////////////

// Constructor, destructor
////////////////////////////

meWindowMainListener::meWindowMainListener( meWindowMain &windowMain ) :
pWindowMain( windowMain ){
}

meWindowMainListener::~meWindowMainListener(){
}



// Notifications
//////////////////

void meWindowMainListener::ModeChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::StateChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::HTStateChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::UndoChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::LightingChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::EditingChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}



void meWindowMainListener::ObjectSelectionChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::ObjectChanged( meWorld*, meObject* ){
	pWindowMain.UpdateAllActions();
}



void meWindowMainListener::DecalSelectionChanged( meWorld* ){
	pWindowMain.UpdateAllActions();
}

void meWindowMainListener::DecalChanged( meWorld*, meDecal* ){
	pWindowMain.UpdateAllActions();
}
