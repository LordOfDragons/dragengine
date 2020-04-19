/* 
 * Drag[en]gine GUI Launcher
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

#include "deglGuiBuilder.h"

#include <dragengine/common/exceptions.h>



// Class deglGuiBuilder
/////////////////////////

// Constructor, destructor
////////////////////////////

deglGuiBuilder::deglGuiBuilder(){
	pFont = NULL;
	
	pColonsFloat = 7;
	pColonsDouble = 12;
	pColonsTextShort = 5; // 15
	pColonsComboBox = 5; // 10
	pRowsComboBox = 10;
	
	pPaddingGroupBox = 3;
	pPaddingLabel = 15;
	pSpacingGroupBox = 3;
	pPaddingMatrixRows = 1;
	pPaddingTextHorz = 3;
	pPaddingTextVert = 2;
	pPaddingButtonHorz = 3;
	pPaddingButtonVert = 2;
}

deglGuiBuilder::deglGuiBuilder( const deglGuiBuilder &builder ){
	pFont = builder.pFont;
	
	pColonsFloat = builder.pColonsFloat;
	pColonsDouble = builder.pColonsDouble;
	pColonsTextShort = builder.pColonsTextShort;
	pColonsComboBox = builder.pColonsTextShort;
	pRowsComboBox = builder.pRowsComboBox;
	
	pPaddingGroupBox = builder.pPaddingGroupBox;
	pPaddingLabel = builder.pPaddingLabel;
	pSpacingGroupBox = builder.pSpacingGroupBox;
	pPaddingMatrixRows = builder.pPaddingMatrixRows;
	pPaddingTextHorz = builder.pPaddingTextHorz;
	pPaddingTextVert = builder.pPaddingTextVert;
	pPaddingButtonHorz = builder.pPaddingButtonHorz;
	pPaddingButtonVert = builder.pPaddingButtonVert;
}

deglGuiBuilder::~deglGuiBuilder(){
}



// Parameters
///////////////

void deglGuiBuilder::SetFont( FXFont *font ){
	pFont = font;
}



void deglGuiBuilder::SetColonsFloat( int colons ){
	if( colons < 0 ) DETHROW( deeInvalidParam );
	
	pColonsFloat = colons;
}

void deglGuiBuilder::SetColonsDouble( int colons ){
	if( colons < 0 ) DETHROW( deeInvalidParam );
	
	pColonsDouble = colons;
}

void deglGuiBuilder::SetColonsTextShort( int colons ){
	if( colons < 0 ) DETHROW( deeInvalidParam );
	
	pColonsTextShort = colons;
}

void deglGuiBuilder::SetColonsComboBox( int colons ){
	if( colons < 0 ) DETHROW( deeInvalidParam );
	
	pColonsComboBox = colons;
}

void deglGuiBuilder::SetRowsComboBox( int rows ){
	if( rows < 1 ) DETHROW( deeInvalidParam );
	
	pRowsComboBox = rows;
}



void deglGuiBuilder::SetPaddingGroupBox( int padding ){
	if( padding < 0 ) DETHROW( deeInvalidParam );
	
	pPaddingGroupBox = padding;
}

void deglGuiBuilder::SetPaddingLabel( int padding ){
	if( padding < 0 ) DETHROW( deeInvalidParam );
	
	pPaddingLabel = padding;
}

void deglGuiBuilder::SetSpacingGroupBox( int spacing ){
	if( spacing < 0 ) DETHROW( deeInvalidParam );
	
	pSpacingGroupBox = spacing;
}

void deglGuiBuilder::SetPaddingMatrixRows( int padding ){
	if( padding < 0 ) DETHROW( deeInvalidParam );
	
	pPaddingMatrixRows = padding;
}

void deglGuiBuilder::SetPaddingTextHorz( int padding ){
	if( padding < 0 ) DETHROW( deeInvalidParam );
	
	pPaddingTextHorz = padding;
}

void deglGuiBuilder::SetPaddingTextVert( int padding ){
	if( padding < 0 ) DETHROW( deeInvalidParam );
	
	pPaddingTextVert = padding;
}

void deglGuiBuilder::SetPaddingButtonHorz( int padding ){
	if( padding < 0 ) DETHROW( deeInvalidParam );
	
	pPaddingButtonHorz = padding;
}

void deglGuiBuilder::SetPaddingButtonVert( int padding ){
	if( padding < 0 ) DETHROW( deeInvalidParam );
	
	pPaddingButtonVert = padding;
}



// Creation
/////////////

FXScrollWindow *deglGuiBuilder::CreateScrollWindow( FXComposite *container ) const{
	if( ! container ) DETHROW( deeInvalidParam );
	
	FXScrollWindow *scrollWindow = new FXScrollWindow( container, LAYOUT_FILL_X | LAYOUT_FILL_Y
		| SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK );
	
	if( ! scrollWindow ) DETHROW( deeOutOfMemory );
	
	return scrollWindow;
}

FXVerticalFrame *deglGuiBuilder::CreateScrollContainer( FXComposite *container ) const{
	FXScrollWindow *scrollWindow = CreateScrollWindow( container );
	
	FXVerticalFrame *verticalFrame = new FXVerticalFrame( scrollWindow, LAYOUT_FILL_X
		| LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	if( ! verticalFrame ) DETHROW( deeOutOfMemory );
	
	return verticalFrame;
}

FXVerticalFrame *deglGuiBuilder::CreateGroupBox( FXComposite *container, const char *caption, bool fillY ) const{
	if( ! container || ! caption ) DETHROW( deeInvalidParam );
	int fillStyle = LAYOUT_FILL_X;
	
	if( fillY ){
		fillStyle |= LAYOUT_FILL_Y;
	}
	
	FXGroupBox *groupBox = new FXGroupBox( container, caption, GROUPBOX_TITLE_LEFT | FRAME_RIDGE | fillStyle,
		0, 0, 0, 0, pPaddingGroupBox, pPaddingGroupBox, pPaddingGroupBox, pPaddingGroupBox );
	if( ! groupBox ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		groupBox->setFont( pFont );
	}
	
	FXVerticalFrame *frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | fillStyle,
		0, 0, 0, 0, 0, 0, 0, 0, pSpacingGroupBox, pSpacingGroupBox );
	
	if( ! frameBox ) DETHROW( deeOutOfMemory );
	
	return frameBox;
}

FXMatrix *deglGuiBuilder::CreateMatrixPacker( FXComposite *container ) const{
	return CreateMatrixPacker( container, pPaddingMatrixRows );
}

FXMatrix *deglGuiBuilder::CreateMatrixPacker( FXComposite *container, int paddingRow ) const{
	if( ! container ) DETHROW( deeInvalidParam );
	
	FXMatrix *packer = new FXMatrix( container, 2, MATRIX_BY_COLUMNS | LAYOUT_SIDE_BOTTOM
		| LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, paddingRow );
	
	if( ! packer ) DETHROW( deeOutOfMemory );
	
	return packer;
}

FXHorizontalFrame *deglGuiBuilder::CreateHFrame( FXComposite *container ) const{
	return CreateHFrame( container, 1 );
}

FXHorizontalFrame *deglGuiBuilder::CreateHFrame( FXComposite *container, int gap ) const{
	if( ! container || gap < 0 ) DETHROW( deeInvalidParam );
	
	FXHorizontalFrame *hframe = new FXHorizontalFrame( container, LAYOUT_SIDE_TOP | LAYOUT_FILL_X
		| LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, gap, 1 );
	
	if( ! hframe ) DETHROW( deeOutOfMemory );
	
	return hframe;
}

FXVerticalFrame *deglGuiBuilder::CreateVFrame( FXComposite *container ) const{
	return CreateVFrame( container, 1 );
}

FXVerticalFrame *deglGuiBuilder::CreateVFrame( FXComposite *container, int gap ) const{
	if( ! container || gap < 0 ) DETHROW( deeInvalidParam );
	
	FXVerticalFrame *vframe = new FXVerticalFrame( container, LAYOUT_SIDE_TOP | LAYOUT_FILL_Y
		| LAYOUT_FILL_ROW, 0, 0, 0, 0, 0, 0, 0, 0, gap, 1 );
	
	if( ! vframe ) DETHROW( deeOutOfMemory );
	
	return vframe;
}

FXLabel *deglGuiBuilder::CreateLabel( FXComposite *container, const char *text, const char *toolTip ) const{
	return CreateLabel( container, text, toolTip, pPaddingLabel );
}

FXLabel *deglGuiBuilder::CreateLabel( FXComposite *container, const char *text, const char *toolTip, int padding ) const{
	if( ! container || ! text || ! toolTip || padding < 0 ) DETHROW( deeInvalidParam );
	
	FXLabel *label = new FXLabel( container, text, NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, padding, 0, 0 );
	if( ! label ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		label->setFont( pFont );
	}
	label->setTipText( toolTip );
	
	return label;
}

FXText *deglGuiBuilder::CreateTextArea( FXComposite *container, FXObject *target, FXSelector selector,
const char *toolTip, bool fixedSizeX, bool fixedSizeY ) const{
	if( ! container || ! toolTip ) DETHROW( deeInvalidParam );
	
	FXPacker *frame;
	FXText *textArea;
	FXint options = 0;
	
	if( ! fixedSizeX ) options |= LAYOUT_FILL_X;
	if( ! fixedSizeY ) options |= LAYOUT_FILL_Y;
	
	frame = new FXPacker( container, FRAME_SUNKEN | options, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	textArea = new FXText( frame, target, selector, TEXT_WORDWRAP | options, 0, 0, 0, 0,
		pPaddingTextHorz, pPaddingTextHorz, pPaddingTextVert, pPaddingTextVert );
	if( ! textArea ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		textArea->setFont( pFont );
	}
	textArea->setTipText( toolTip );
	
	return textArea;
}

FXTextField *deglGuiBuilder::CreateTextField( FXComposite *container, FXObject *target, FXSelector selector,
const char *toolTip, bool fixedSize ) const{
	return CreateTextField( container, target, selector, toolTip, pColonsTextShort, fixedSize );
}

FXTextField *deglGuiBuilder::CreateTextField( FXComposite *container, FXObject *target, FXSelector selector,
const char *toolTip, int colons, bool fixedSize ) const{
	if( ! container || ! toolTip || colons < 0 ) DETHROW( deeInvalidParam );
	
	FXTextField *textField;
	
	if( fixedSize ){
		textField = new FXTextField( container, colons, target, selector, FRAME_SUNKEN, 0, 0, 0, 0,
			pPaddingTextHorz, pPaddingTextHorz, pPaddingTextVert, pPaddingTextVert );
		
	}else{
		textField = new FXTextField( container, colons, target, selector, FRAME_SUNKEN | LAYOUT_FILL_X
		| LAYOUT_FILL_COLUMN, 0, 0, 0, 0, pPaddingTextHorz, pPaddingTextHorz, pPaddingTextVert, pPaddingTextVert );
	}
	
	if( ! textField ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		textField->setFont( pFont );
	}
	textField->setTipText( toolTip );
	
	return textField;
}

FXTextField *deglGuiBuilder::CreateNumberField( FXComposite *container, FXObject *target, FXSelector selector,
const char *toolTip, int colons, bool fixedSize ) const{
	if( ! container || ! toolTip || colons < 0 ) DETHROW( deeInvalidParam );
	
	FXTextField *textField;
	
	if( fixedSize ){
		textField = new FXTextField( container, colons, target, selector, FRAME_SUNKEN,
			0, 0, 0, 0, pPaddingTextHorz, pPaddingTextHorz, pPaddingTextVert, pPaddingTextVert );
		
	}else{
		textField = new FXTextField( container, colons, target, selector, FRAME_SUNKEN
			| LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0,
			pPaddingTextHorz, pPaddingTextHorz, pPaddingTextVert, pPaddingTextVert );
	}
	if( ! textField ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		textField->setFont( pFont );
	}
	textField->setTipText( toolTip );
	textField->setJustify( JUSTIFY_RIGHT );
	
	return textField;
}

FXButton *deglGuiBuilder::CreateButton( FXComposite *container, const char *text, FXObject *target,
FXSelector selector, const char *toolTip ) const{
	return CreateButton( container, text, NULL, target, selector, toolTip );
}

FXButton *deglGuiBuilder::CreateButton( FXComposite *container, const char *text, FXIcon *icon, FXObject *target,
FXSelector selector, const char *toolTip ) const{
	if( ! container || ! text || ! toolTip ) DETHROW( deeInvalidParam );
	
	FXButton *button = new FXButton( container, text, icon, target, selector, FRAME_RAISED
		| JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, pPaddingButtonHorz, pPaddingButtonHorz,
		pPaddingButtonVert, pPaddingButtonVert );
	
	if( ! button ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		button->setFont( pFont );
	}
	button->setTipText( toolTip );
	
	return button;
}

FXComboBox *deglGuiBuilder::CreateComboBox( FXComposite *container, FXObject *target, FXSelector selector,
const char *toolTip, bool editable ) const{
	return CreateComboBox( container, target, selector, toolTip, editable, pColonsComboBox, pRowsComboBox, false );
}

FXComboBox *deglGuiBuilder::CreateComboBox( FXComposite *container, FXObject *target, FXSelector selector,
const char *toolTip, bool editable, int colons, int rows, bool fixedSize ) const{
	if( ! container || ! toolTip || colons < 0 || rows < 1 ) DETHROW( deeInvalidParam );
	FXComboBox *combobox;
	
	if( fixedSize ){
		combobox = new FXComboBox( container, colons, target, selector, FRAME_SUNKEN, 0, 0, 0, 0,
			pPaddingTextHorz, pPaddingTextHorz, pPaddingTextVert, pPaddingTextVert );
		
	}else{
		combobox = new FXComboBox( container, colons, target, selector, FRAME_SUNKEN | LAYOUT_FILL_X
			| LAYOUT_FILL_COLUMN, 0, 0, 0, 0, pPaddingTextHorz, pPaddingTextHorz, pPaddingTextVert, pPaddingTextVert );
	}
	if( ! combobox ) DETHROW( deeOutOfMemory );
	
	if( editable ){
		combobox->setEditable( true );
		
	}else{
		combobox->setEditable( false );
	}
	combobox->setNumVisible( rows );
	if( pFont ){
		combobox->setFont( pFont );
	}
	combobox->setTipText( toolTip );
	
	return combobox;
}

FXCheckButton *deglGuiBuilder::CreateCheckBox( FXComposite *container, const char *text, FXObject *target, FXSelector selector,
const char *toolTip ) const{
	if( ! container || ! text || ! toolTip ) DETHROW( deeInvalidParam );
	
	FXCheckButton *checkbox = new FXCheckButton( container, text, target, selector, CHECKBUTTON_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! checkbox ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		checkbox->setFont( pFont );
	}
	checkbox->setTipText( toolTip );
	
	return checkbox;
}

FXSlider *deglGuiBuilder::CreateSlider( FXComposite *container, FXObject *target, FXSelector selector, const char *toolTip ) const{
	if( ! container || ! toolTip ) DETHROW( deeInvalidParam );
	
	FXSlider *slider = new FXSlider( container, target, selector, SLIDER_HORIZONTAL | SLIDER_ARROW_DOWN
		| SLIDER_TICKS_BOTTOM | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	
	if( ! slider ) DETHROW( deeOutOfMemory );
	
	slider->setTipText( toolTip );
	
	return slider;
}

FXIconList *deglGuiBuilder::CreateIconList( FXComposite *container, FXObject *target, FXSelector selector ) const{
	if( ! container ) DETHROW( deeInvalidParam );
	
	FXIconList *iconList = new FXIconList( container, target, selector, FOLDINGLIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN );
	if( ! iconList ) DETHROW( deeOutOfMemory );
	
	if( pFont ){
		iconList->setFont( pFont );
	}
	
	FXHeader *header = iconList->getHeader();
	if( pFont ){
		header->setFont( pFont );
	}
	header->setPadLeft( pPaddingTextHorz );
	header->setPadTop( pPaddingTextVert );
	header->setPadRight( pPaddingTextHorz );
	header->setPadBottom( pPaddingTextVert );
	
	return iconList;
}



FXint deglGuiBuilder::SortListItemByName( const FXListItem *item1, const FXListItem *item2 ){
	return strcmp( item1->getText().text(), item2->getText().text() );
}

FXint deglGuiBuilder::SortIconItemByName( const FXIconItem *item1, const FXIconItem *item2 ){
	return strcmp( item1->getText().text(), item2->getText().text() );
}
