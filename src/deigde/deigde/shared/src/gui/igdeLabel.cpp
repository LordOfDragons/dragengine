/* 
 * Drag[en]gine IGDE
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

#include "igdeLabel.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeLabel : public FXLabel{
	FXDECLARE( cNativeIgdeLabel )
	
protected:
	cNativeIgdeLabel();
	
private:
	igdeLabel *pOwner;
	igdeFontReference pFont;
	
public:
	cNativeIgdeLabel( igdeLabel &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeLabel();
	
	static FXIcon *LabelIcon( const igdeLabel &owner );
	static int LabelFlags( const igdeLabel &owner );
	static igdeFont *LabelFont( const igdeLabel &owner, const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeLabel ) cNativeIgdeLabelMap[] = { };

FXIMPLEMENT( cNativeIgdeLabel, FXLabel, cNativeIgdeLabelMap, ARRAYNUMBER( cNativeIgdeLabelMap ) )

cNativeIgdeLabel::cNativeIgdeLabel(){ }

cNativeIgdeLabel::cNativeIgdeLabel( igdeLabel &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXLabel( parent, owner.GetText().GetString(), LabelIcon( owner ),
	layoutFlags | LabelFlags( owner ), 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pFont( LabelFont( owner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
}

cNativeIgdeLabel::~cNativeIgdeLabel(){
}

FXIcon *cNativeIgdeLabel::LabelIcon( const igdeLabel &owner ){
	if( owner.GetIcon() ){
		return ( FXIcon* )owner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

int cNativeIgdeLabel::LabelFlags( const igdeLabel &owner ){
	const int alignment = owner.GetAlignment();
	int flags = ICON_BEFORE_TEXT;
	
	if( ( alignment & igdeLabel::eaLeft ) == igdeLabel::eaLeft ){
		flags |= JUSTIFY_LEFT;
		
	}else if( ( alignment & igdeLabel::eaRight ) == igdeLabel::eaRight ){
		flags |= JUSTIFY_RIGHT;
		
	}else{
		flags |=  JUSTIFY_CENTER_X;
	}
	
	if( ( alignment & igdeLabel::eaTop ) == igdeLabel::eaTop ){
		flags |= JUSTIFY_TOP;
		
	}else if( ( alignment & igdeLabel::eaBottom ) == igdeLabel::eaBottom ){
		flags |= JUSTIFY_BOTTOM;
		
	}else{
		flags |=  JUSTIFY_CENTER_Y;
	}
	
	return flags;
}

igdeFont *cNativeIgdeLabel::LabelFont( const igdeLabel &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::labelFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::labelFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::labelFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::labelFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}



// Class igdeLabel
/////////////////////

// Constructor, destructor
////////////////////////////

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ){
}

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text,
	const char *description, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ),
pDescription( description ){
}

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ),
pIcon( icon ){
}

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ),
pDescription( description ),
pIcon( icon ){
}

igdeLabel::~igdeLabel(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeLabel::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeLabel::SetAlignment( int alignment ){
	if( pAlignment == alignment ){
		return;
	}
	
	pAlignment = alignment;
	OnAlignmentChanged();
}

void igdeLabel::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeLabel::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}



void igdeLabel::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	cNativeIgdeLabel * const foxWidget = new cNativeIgdeLabel(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeLabel::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeLabel* )GetNativeWidget();
	DropNativeWidget();
}



void igdeLabel::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	
	cNativeIgdeLabel &label = *( ( cNativeIgdeLabel* )GetNativeWidget() );
	label.setText( pText.GetString() );
}

void igdeLabel::OnAlignmentChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeLabel &label = *( ( cNativeIgdeLabel* )GetNativeWidget() );
	label.setFrameStyle( cNativeIgdeLabel::LabelFlags( *this ) );
}

void igdeLabel::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeLabel &label = *( ( cNativeIgdeLabel* )GetNativeWidget() );
	label.setTipText( pDescription.GetString() );
	label.setHelpText( pDescription.GetString() );
}

void igdeLabel::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeLabel &label = *( ( cNativeIgdeLabel* )GetNativeWidget() );
	label.setIcon( cNativeIgdeLabel::LabelIcon( *this ) );
}
