/* 
 * Drag[en]gine IGDE World Editor
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

#if 0

// includes
/////////////
#include "meWEFColorMatrix.h"
#include "meWindowEffects.h"
#include "meWTColorMatrix.h"
#include "../meWindowMain.h"
#include "../../worldedit.h"
#include "dragengine/deEngine.h"
#include "dragengine/resources/effect/deEffectColorMatrix.h"
#include "dragengine/common/exceptions.h"



// events
///////////
FXDEFMAP( meWEFColorMatrix ) meWEFColorMatrixMap[] = {
	FXMAPFUNC( SEL_REPLACED, meWEFColorMatrix::ID_TABLEMATRIX, meWEFColorMatrix::onTableMatrixChanged ),
	FXMAPFUNC( SEL_COMMAND, meWEFColorMatrix::ID_CHKENABLE, meWEFColorMatrix::onCheckEnableCommand ),
	FXMAPFUNC( SEL_CHANGED, meWEFColorMatrix::ID_CBTEMPLATES, meWEFColorMatrix::onCBTemplatesChanged ),
	FXMAPFUNC( SEL_COMMAND, meWEFColorMatrix::ID_BTNAPPLYTEMPLATE, meWEFColorMatrix::onBtnApplyTemplateCommand ),
	FXMAPFUNC( SEL_COMMAND, meWEFColorMatrix::ID_BTNSAVETEMPLATE, meWEFColorMatrix::onBtnSaveTemplateCommand ),
};




// class meWEFColorMatrix
//////////////////////

FXIMPLEMENT( meWEFColorMatrix, FXVerticalFrame, meWEFColorMatrixMap, ARRAYNUMBER( meWEFColorMatrixMap ) )

// Constructor, destructor
////////////////////////////

meWEFColorMatrix::meWEFColorMatrix(){ }

meWEFColorMatrix::meWEFColorMatrix( deEffectColorMatrix *effectColorMatrix, meWindowEffects *windowEffects, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT,
0, 0, 0, 0, 0, 0, 0, 0 ){
	if( ! effectColorMatrix || ! windowEffects ) DETHROW( deeInvalidParam );
	int i, padding = 3, spacing = 3;
	FXString text;
	
	// prepare
	pWndEffects = windowEffects;
	pEffectColorMatrix = effectColorMatrix;
	pTemplates = NULL;
	pTemplateCount = 0;
	pTemplateSize = 0;
	
	// add templates
	pAddTemplates();
	
	// group box
	FXGroupBox *groupBox = new FXGroupBox( this, "Color Matrix Parameters:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	FXVerticalFrame *frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	// matrix table
	new FXLabel( frameBox, "Matrix Values:" );
	pTblMatrix = new FXTable( frameBox, this, ID_TABLEMATRIX, TABLE_COL_SIZABLE
		| TABLE_NO_COLSELECT | TABLE_NO_ROWSELECT | FRAME_SUNKEN | LAYOUT_FILL_X );
	pTblMatrix->showHorzGrid( true );
	pTblMatrix->showVertGrid( true );
	pTblMatrix->setDefColumnWidth( 50 );
	pTblMatrix->setRowHeaderWidth( 50 );
	pTblMatrix->setTableSize( 4, 5 );
	pTblMatrix->setVisibleRows( 4 );
	
	pTblMatrix->setColumnText( 0, "Red" );
	pTblMatrix->setColumnText( 1, "Green" );
	pTblMatrix->setColumnText( 2, "Blue" );
	pTblMatrix->setColumnText( 3, "Alpha" );
	pTblMatrix->setColumnText( 4, "Translation" );
	pTblMatrix->setRowText( 0, "Red" );
	pTblMatrix->setRowText( 1, "Green" );
	pTblMatrix->setRowText( 2, "Blue" );
	pTblMatrix->setRowText( 3, "Alpha" );
	
	pTblMatrix->getColumnHeader()->setItemSize( 4, 90 );
	
	UpdateColorMatrix();
	
	// disabled
	FXHorizontalFrame *frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
 	pChkEnable = new FXCheckButton( frameLine, "Enable Effect", this, ID_CHKENABLE, CHECKBUTTON_NORMAL );
	
	// group box
	groupBox = new FXGroupBox( this, "Color Matrix Templates:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	// template selection
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	pCBTemplates = new FXComboBox( frameLine, 10, this, ID_CBTEMPLATES, FRAME_SUNKEN | LAYOUT_FILL_X );
	pCBTemplates->setEditable( true );
	pCBTemplates->setNumVisible( 6 );
 	new FXButton( frameLine, "Apply", NULL, this, ID_BTNAPPLYTEMPLATE, BUTTON_NORMAL );
 	new FXButton( frameLine, "Save", NULL, this, ID_BTNSAVETEMPLATE, BUTTON_NORMAL );
	for( i=0; i<pTemplateCount; i++ ){
		pCBTemplates->appendItem( pTemplates[ i ]->GetName(), pTemplates[ i ] );
	}
	
	// add values
	pChkEnable->setCheck( pEffectColorMatrix->GetEnabled() );
	
	// realize
	create();
}

meWEFColorMatrix::~meWEFColorMatrix(){
	if( pTemplates ){
		while( pTemplateCount > 0 ){
			delete pTemplates[ pTemplateCount - 1 ];
			pTemplateCount--;
		}
		delete [] pTemplates;
	}
}



// Management
///////////////

void meWEFColorMatrix::UpdateColorMatrix(){
	const decColorMatrix &colorMatrix = pEffectColorMatrix->GetColorMatrix();
	FXString text;
	
	// red
	text.format( "%g", colorMatrix.a11 );
	pTblMatrix->setItemText( 0, 0, text );
	text.format( "%g", colorMatrix.a12 );
	pTblMatrix->setItemText( 0, 1, text );
	text.format( "%g", colorMatrix.a13 );
	pTblMatrix->setItemText( 0, 2, text );
	text.format( "%g", colorMatrix.a14 );
	pTblMatrix->setItemText( 0, 3, text );
	text.format( "%g", colorMatrix.a15 );
	pTblMatrix->setItemText( 0, 4, text );
	
	// green
	text.format( "%g", colorMatrix.a21 );
	pTblMatrix->setItemText( 1, 0, text );
	text.format( "%g", colorMatrix.a22 );
	pTblMatrix->setItemText( 1, 1, text );
	text.format( "%g", colorMatrix.a23 );
	pTblMatrix->setItemText( 1, 2, text );
	text.format( "%g", colorMatrix.a24 );
	pTblMatrix->setItemText( 1, 3, text );
	text.format( "%g", colorMatrix.a25 );
	pTblMatrix->setItemText( 1, 4, text );
	
	// blue
	text.format( "%g", colorMatrix.a31 );
	pTblMatrix->setItemText( 2, 0, text );
	text.format( "%g", colorMatrix.a32 );
	pTblMatrix->setItemText( 2, 1, text );
	text.format( "%g", colorMatrix.a33 );
	pTblMatrix->setItemText( 2, 2, text );
	text.format( "%g", colorMatrix.a34 );
	pTblMatrix->setItemText( 2, 3, text );
	text.format( "%g", colorMatrix.a35 );
	pTblMatrix->setItemText( 2, 4, text );
	
	// alpha
	text.format( "%g", colorMatrix.a41 );
	pTblMatrix->setItemText( 3, 0, text );
	text.format( "%g", colorMatrix.a42 );
	pTblMatrix->setItemText( 3, 1, text );
	text.format( "%g", colorMatrix.a43 );
	pTblMatrix->setItemText( 3, 2, text );
	text.format( "%g", colorMatrix.a44 );
	pTblMatrix->setItemText( 3, 3, text );
	text.format( "%g", colorMatrix.a45 );
	pTblMatrix->setItemText( 3, 4, text );
}

void meWEFColorMatrix::AddTemplate( meWTColorMatrix *aTemplate ){
	if( ! aTemplate ) DETHROW( deeInvalidParam );
	
	if( pTemplateCount == pTemplateSize ){
		int i, newSize = pTemplateCount * 3 / 2 + 1;
		meWTColorMatrix **newArray = new meWTColorMatrix*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		if( pTemplates ){
			for( i=0; i<pTemplateCount; i++ ) newArray[ i ] = pTemplates[ i ];
			delete [] pTemplates;
		}
		
		pTemplates = newArray;
		pTemplateSize = newSize;
	}
	
	pTemplates[ pTemplateCount ] = aTemplate;
	pTemplateCount++;
}



// Events
///////////

long meWEFColorMatrix::onTableMatrixChanged( FXObject *sender, FXSelector selector, void *data ){
	decColorMatrix colorMatrix = pEffectColorMatrix->GetColorMatrix();
	int row = pTblMatrix->getCurrentRow();
	int col = pTblMatrix->getCurrentColumn();
	float value;
	
	if( row == -1 || col == -1 ) return 1;
	
	value = strtof( pTblMatrix->getItemText( row, col ).text(), NULL );
	
	if( row == 0 ){
		if( col == 0 ){
			colorMatrix.a11 = value;
		}else if( col == 1 ){
			colorMatrix.a12 = value;
		}else if( col == 2 ){
			colorMatrix.a13 = value;
		}else if( col == 3 ){
			colorMatrix.a14 = value;
		}else{
			colorMatrix.a15 = value;
		}
		
	}else if( row == 1 ){
		if( col == 0 ){
			colorMatrix.a21 = value;
		}else if( col == 1 ){
			colorMatrix.a22 = value;
		}else if( col == 2 ){
			colorMatrix.a23 = value;
		}else if( col == 3 ){
			colorMatrix.a24 = value;
		}else{
			colorMatrix.a25 = value;
		}
		
	}else if( row == 2 ){
		if( col == 0 ){
			colorMatrix.a31 = value;
		}else if( col == 1 ){
			colorMatrix.a32 = value;
		}else if( col == 2 ){
			colorMatrix.a33 = value;
		}else if( col == 3 ){
			colorMatrix.a34 = value;
		}else{
			colorMatrix.a35 = value;
		}
		
	}else if( row == 3 ){
		if( col == 0 ){
			colorMatrix.a41 = value;
		}else if( col == 1 ){
			colorMatrix.a42 = value;
		}else if( col == 2 ){
			colorMatrix.a43 = value;
		}else if( col == 3 ){
			colorMatrix.a44 = value;
		}else{
			colorMatrix.a45 = value;
		}
	}
	
	pEffectColorMatrix->SetColorMatrix( colorMatrix );
	
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}

long meWEFColorMatrix::onCheckEnableCommand( FXObject *sender, FXSelector selector, void *data ){
	pEffectColorMatrix->SetEnabled( pChkEnable->getCheck() );
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}

long meWEFColorMatrix::onCBTemplatesChanged( FXObject *sender, FXSelector selector, void *data ){
	return 1;
}

long meWEFColorMatrix::onBtnApplyTemplateCommand( FXObject *sender, FXSelector selector, void *data ){
	int selection = pCBTemplates->getCurrentItem();
	FXString text;
	
	if( selection == -1 ) return 1;
	
	pEffectColorMatrix->SetColorMatrix( pTemplates[ selection ]->GetColorMatrix() );
	
	UpdateColorMatrix();
	
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}

long meWEFColorMatrix::onBtnSaveTemplateCommand( FXObject *sender, FXSelector selector, void *data ){
	//pEffectColorMatrix->SetEnabled( pChkEnable->getCheck() );
	//pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}



// Private Functions
//////////////////////

void meWEFColorMatrix::pAddTemplates(){
	float luminanceRed = 0.3086f;
	float luminanceGreen = 0.6094f;
	float luminanceBlue = 0.0820;
	meWTColorMatrix *aTemplate = NULL;
	decColorMatrix cmDesaturate;
	decColorMatrix colorMatrix;
	
	try{
		// Identity
		aTemplate = new meWTColorMatrix( "Null Matrix", decColorMatrix() );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Raise intensity
		aTemplate = new meWTColorMatrix( "Raise Intensity 25%",
			decColorMatrix::CreateTranslation( 0.25f, 0.25f, 0.25f, 0.0f ) );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Lower intensity
		aTemplate = new meWTColorMatrix( "Lower Intensity 25%",
			decColorMatrix::CreateTranslation( -0.25f, -0.25f, -0.25f, 0.0f ) );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Raise Brightness
		aTemplate = new meWTColorMatrix( "Raise Brightness 25%",
			decColorMatrix::CreateScaling( 1.25f, 1.25f, 1.25f, 0.0f ) );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Lower Brightness
		aTemplate = new meWTColorMatrix( "Lower Brightness 25%",
			decColorMatrix::CreateScaling( 0.75f, 0.75f, 0.75f, 0.0f ) );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Desaturate
		cmDesaturate.SetFrom( decColor( luminanceRed, luminanceRed, luminanceRed, 0.0f ),
				decColor( luminanceGreen, luminanceGreen, luminanceGreen, 0.0f ),
				decColor( luminanceBlue, luminanceBlue, luminanceBlue, 0.0f ),
				decColor( 0.0f, 0.0f, 0.0f, 1.0f ), decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		
		aTemplate = new meWTColorMatrix( "Desaturate", cmDesaturate );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Night Vision Googles
		colorMatrix = cmDesaturate * decColorMatrix::CreateScaling( 0.0f, 2.0f, 0.0f, 1.0f )
			* decColorMatrix::CreateTranslation( 0.0f, 0.1f, 0.0f, 0.0f );
		
		aTemplate = new meWTColorMatrix( "Night Vision Googles", colorMatrix );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
	}catch( const deException & ){
		if( aTemplate ) delete aTemplate;
		throw;
	}
}

#endif
