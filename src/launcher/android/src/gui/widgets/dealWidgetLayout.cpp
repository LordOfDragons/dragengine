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

#include "dealWidget.h"
#include "dealWidgetLayout.h"
#include "../dealDisplay.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetLayout
///////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetLayout::dealWidgetLayout( dealDisplay &display ) :
dealWidget( display ),
pDirtyLayout( true ){
}

dealWidgetLayout::dealWidgetLayout( dealDisplay &display, const decPoint &position, const decPoint &size ) :
dealWidget( display, position, size ),
pDirtyLayout( true ){
}

dealWidgetLayout::~dealWidgetLayout(){
}



// Management
///////////////

int dealWidgetLayout::GetWidgetCount() const{
	return pWidgets.GetCount();
}

dealWidget *dealWidgetLayout::GetWidgetAt( int index ) const{
	return ( dealWidget* )pWidgets.GetAt( index );
}

void dealWidgetLayout::AddWidget( dealWidget *widget ){
	if( ! widget ){
		DETHROW( deeInvalidParam );
	}
	if( widget->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	pWidgets.Add( widget );
	widget->SetParent( this );
	
	DirtyLayout();
}

void dealWidgetLayout::RemoveWidget( dealWidget *widget ){
	if( ! pWidgets.Has( widget ) ){
		DETHROW( deeInvalidParam );
	}
	
	widget->SetParent( NULL );
	pWidgets.Remove( widget );
	
	DirtyLayout();
}

void dealWidgetLayout::RemoveAllWidgets(){
	if( pWidgets.GetCount() == 0 ){
		return;
	}
	
	const int count = pWidgets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( dealWidget* )pWidgets.GetAt( i ) )->SetParent( NULL );
	}
	
	pWidgets.RemoveAll();
	DirtyLayout();
}



decPoint dealWidgetLayout::GetDialogPosition() const{
	if( GetParent() ){
		return dealWidget::GetDialogPosition();
		
	}else{
		return decPoint();
	}
}

dealWidget *dealWidgetLayout::WidgetAtPosition( const decPoint &point ) const{
	if( ! IsPointInside( point ) ){
		return NULL;
	}
	
	const int count = pWidgets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dealWidget *widget = ( dealWidget* )pWidgets.GetAt( i );
		widget = widget->WidgetAtPosition( point - widget->GetPosition() );
		if( widget ){
			return widget;
		}
	}
	
	return ( dealWidget* )this;
}



void dealWidgetLayout::DirtyLayout(){
	if( pDirtyLayout ){
		return;
	}
	
	pDirtyLayout = true;
	DirtyParentLayout();
}

void dealWidgetLayout::Layout(){
	if( ! pDirtyLayout ){
		return;
	}
	
	pDirtyLayout = false;
	LayoutWidgets();
}

void dealWidgetLayout::LayoutWidgets(){
}



void dealWidgetLayout::RenderContent( const sRenderContext &context ){
	const int count = pWidgets.GetCount();
	if( count == 0 ){
		return;
	}
	
	Layout();
	
	sRenderContext childContext;
	childContext.color = context.color;
	childContext.clipBaseScreenY = context.clipBaseScreenY;
	
	int i;
	for( i=0; i<count; i++ ){
		dealWidget &child = *( ( dealWidget* )pWidgets.GetAt( i ) );
		
		if( ! child.GetVisible() ){
			continue;
		}
		
		const decPoint &childPosition = child.GetPosition();
		const decPoint &childSize = child.GetSize();
		
		if( ! ( childSize > decPoint() ) ){
			continue;
		}
		
		childContext.screenPosition = context.screenPosition + childPosition;
		childContext.viewFrom = childContext.screenPosition.Largest( context.viewFrom );
		childContext.viewTo = ( childContext.screenPosition + childSize ).Smallest( context.viewTo );
		
		if( ! ( ( childContext.viewTo - childContext.viewFrom ) > decPoint() ) ){
			continue;
		}
		
		if( ! ( childContext.viewFrom <= context.viewTo && childContext.viewTo >= context.viewFrom ) ){
			continue;
		}
		
		childContext.transform = decTexMatrix::CreateTranslation( childPosition ) * context.transform;
		
		child.Render( childContext );
	}
}



void dealWidgetLayout::OnPositionChanged(){
	DirtyLayout();
}

void dealWidgetLayout::OnSizeChanged(){
	DirtyLayout();
}

void dealWidgetLayout::OnPaddingChanged(){
	DirtyLayout();
}
