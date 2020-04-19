/* 
 * Drag[en]gine Android Launcher
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
