/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deglbWindowLogger.h"
#include "deglbWindowLoggerListener.h"

#include <delauncher/logger/delLoggerHistoryEntry.h>

#include <dragengine/common/exceptions.h>



// Class deglbWindowLoggerListener
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbWindowLoggerListener::deglbWindowLoggerListener(deglbWindowLogger &window) :
pWindow(window){
}

deglbWindowLoggerListener::~deglbWindowLoggerListener(){
}



// Management
///////////////

void deglbWindowLoggerListener::MessageAdded(const delLoggerHistory &,
const delLoggerHistoryEntry &entry){
	// Called from a non-UI thread with history mutex locked.
	// Post a message to the logger window to handle it safely.
	if(pWindow.LockLooper()){
		pWindow.AddLogEntry(entry);
		pWindow.UnlockLooper();
	}
}

void deglbWindowLoggerListener::HistoryCleared(const delLoggerHistory &){
	// Called from a non-UI thread with history mutex locked.
	if(pWindow.LockLooper()){
		pWindow.ClearLog();
		pWindow.UnlockLooper();
	}
}
