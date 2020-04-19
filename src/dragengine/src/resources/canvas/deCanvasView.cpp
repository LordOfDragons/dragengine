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
	if( ! canvas || canvas->GetParentView() ){
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
	if( ! canvas || canvas->GetParentView() != this ){
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
