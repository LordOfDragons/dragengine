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

#include "igdeContainerBox.h"
#include "../igdeWidget.h"
#include "../native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeContainerBox
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerBox::igdeContainerBox( igdeEnvironment &environment, eAxis axis, int spacing ) :
igdeContainer( environment ),
pAxis( axis ),
pSpacing( spacing )
{
	if( spacing < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeContainerBox::~igdeContainerBox(){
}



// Management
///////////////

void igdeContainerBox::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	void * const native = igdeNativeContainerBox::CreateNativeWidget( *this  );
	SetNativeWidget( native );
	igdeNativeContainerBox::PostCreateNativeWidget( *this, native );
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	void * const native = GetNativeWidget();
	DropNativeWidget();
	igdeNativeContainerBox::DestroyNativeWidget( *this, native );
}
