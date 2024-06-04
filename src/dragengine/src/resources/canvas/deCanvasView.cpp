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

#include "deCanvasView.h"
#include "deCanvasVisitor.h"
#include "../../common/exceptions.h"



// Class deCanvasView
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvasView::deCanvasView( deCanvasManager *manager ) :
deCanvas( manager ),

pCanvasRoot( NULL ),
pCanvasTail( NULL ),
pCanvasCount( 0 ){
}

deCanvasView::~deCanvasView(){
	RemoveAllCanvas();
}



// Management
///////////////

void deCanvasView::AddCanvas( deCanvas *canvas ){
	if( ! canvas || canvas->GetParentMask() || canvas->GetParentView()){
		DETHROW( deeInvalidParam );
	}
	
	if( pCanvasTail ){
		pCanvasTail->SetLLViewNext( canvas );
		canvas->SetLLViewPrev( pCanvasTail );
		canvas->SetLLViewNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		canvas->SetLLViewPrev( NULL ); // not required by definition, just to make sure...
		canvas->SetLLViewNext( NULL ); // not required by definition, just to make sure...
		pCanvasRoot = canvas;
	}
	
	pCanvasTail = canvas;
	pCanvasCount++;
	canvas->SetParentView( this );
	canvas->AddReference();
	
	NotifyContentChanged();
}

void deCanvasView::RemoveCanvas( deCanvas *canvas ){
	if( ! canvas || canvas->GetParentMask() || canvas->GetParentView() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( canvas->GetLLViewPrev() ){
		canvas->GetLLViewPrev()->SetLLViewNext( canvas->GetLLViewNext() );
	}
	if( canvas->GetLLViewNext() ){
		canvas->GetLLViewNext()->SetLLViewPrev( canvas->GetLLViewPrev() );
	}
	if( canvas == pCanvasRoot ){
		pCanvasRoot = canvas->GetLLViewNext();
	}
	if( canvas == pCanvasTail ){
		pCanvasTail = canvas->GetLLViewPrev();
	}
	pCanvasCount--;
	
	canvas->SetParentView( NULL );
	canvas->SetLLViewPrev( NULL );
	canvas->SetLLViewNext( NULL );
	canvas->FreeReference();
	
	NotifyContentChanged();
}

void deCanvasView::RemoveAllCanvas(){
	while( pCanvasTail ){
		deCanvas * const next = pCanvasTail->GetLLViewPrev();
		pCanvasTail->SetParentView( NULL );
		pCanvasTail->FreeReference();
		pCanvasTail = next;
		pCanvasCount--;
	}
	pCanvasRoot = NULL;
	
	NotifyContentChanged();
}



// Visiting
/////////////

void deCanvasView::Visit( deCanvasVisitor &visitor ){
	visitor.VisitView( *this );
}
