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

#if 0

// includes
/////////////
#include "meWEFFilterKernel.h"
#include "meWindowEffects.h"
#include "meWTFilterKernel.h"
#include "../meWindowMain.h"
#include "../../worldedit.h"
#include "dragengine/deEngine.h"
#include "dragengine/resources/effect/deEffectFilterKernel.h"
#include "dragengine/common/exceptions.h"



// events
///////////
FXDEFMAP( meWEFFilterKernel ) meWEFFilterKernelMap[] = {
	FXMAPFUNC( SEL_COMMAND, meWEFFilterKernel::ID_EDITROWS, meWEFFilterKernel::onEditRowsCommand ),
	FXMAPFUNC( SEL_COMMAND, meWEFFilterKernel::ID_EDITCOLS, meWEFFilterKernel::onEditColsCommand ),
	FXMAPFUNC( SEL_REPLACED, meWEFFilterKernel::ID_TABLEKERNEL, meWEFFilterKernel::onTableKernelChanged ),
	FXMAPFUNC( SEL_COMMAND, meWEFFilterKernel::ID_EDIT_SCALE, meWEFFilterKernel::onEditScaleCommand ),
	FXMAPFUNC( SEL_COMMAND, meWEFFilterKernel::ID_CHKENABLE, meWEFFilterKernel::onCheckEnableCommand ),
	FXMAPFUNC( SEL_CHANGED, meWEFFilterKernel::ID_CBTEMPLATES, meWEFFilterKernel::onCBTemplatesChanged ),
	FXMAPFUNC( SEL_COMMAND, meWEFFilterKernel::ID_BTNAPPLYTEMPLATE, meWEFFilterKernel::onBtnApplyTemplateCommand ),
	FXMAPFUNC( SEL_COMMAND, meWEFFilterKernel::ID_BTNSAVETEMPLATE, meWEFFilterKernel::onBtnSaveTemplateCommand ),
};




// class meWEFFilterKernel
//////////////////////

FXIMPLEMENT( meWEFFilterKernel, FXVerticalFrame, meWEFFilterKernelMap, ARRAYNUMBER( meWEFFilterKernelMap ) )

// Constructor, destructor
////////////////////////////

meWEFFilterKernel::meWEFFilterKernel(){ }

meWEFFilterKernel::meWEFFilterKernel( deEffectFilterKernel *effectFilterKernel, meWindowEffects *windowEffects, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT,
0, 0, 0, 0, 0, 0, 0, 0 ){
	if( ! effectFilterKernel || ! windowEffects ) DETHROW( deeInvalidParam );
	int kernelRows = effectFilterKernel->GetKernelRows();
	int kernelCols = effectFilterKernel->GetKernelCols();
	int i, padding = 3, spacing = 3;
	FXString text;
	
	// prepare
	pWndEffects = windowEffects;
	pEffectFilterKernel = effectFilterKernel;
	pTemplates = NULL;
	pTemplateCount = 0;
	pTemplateSize = 0;
	
	// add templates
	pAddTemplates();
	
	// group box
	FXGroupBox *groupBox = new FXGroupBox( this, "Filter Kernel Parameters:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding );
	FXVerticalFrame *frameBox = new FXVerticalFrame( groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	
	// rows
	FXHorizontalFrame *frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Rows:" );
	pEditRows = new FXTextField( frameLine, 3, this, ID_EDITROWS, FRAME_SUNKEN );
	new FXLabel( frameLine, "Cols:" );
	pEditCols = new FXTextField( frameLine, 3, this, ID_EDITCOLS, FRAME_SUNKEN );
	
	// kernel table
	new FXLabel( frameBox, "Kernel Values:" );
	pTblKernel = new FXTable( frameBox, this, ID_TABLEKERNEL, TABLE_COL_SIZABLE
		| TABLE_NO_COLSELECT | TABLE_NO_ROWSELECT | FRAME_SUNKEN | LAYOUT_FILL_X );
	pTblKernel->showHorzGrid( true );
	pTblKernel->showVertGrid( true );
	pTblKernel->setDefColumnWidth( 50 );
	pTblKernel->setRowHeaderWidth( 50 );
	
	UpdateFilterKernel();
	
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
	new FXLabel( frameLine, "Scale:" );
	pEditScale = new FXTextField( frameLine, 5, this, ID_EDIT_SCALE, FRAME_SUNKEN );
	
	// disabled
	frameLine = new FXHorizontalFrame( frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing );
 	pChkEnable = new FXCheckButton( frameLine, "Enable Effect", this, ID_CHKENABLE, CHECKBUTTON_NORMAL );
	
	// group box
	groupBox = new FXGroupBox( this, "Filter Kernel Templates:",
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
	text.format( "%i", kernelRows );
	pEditRows->setText( text );
	text.format( "%i", kernelCols );
	pEditCols->setText( text );
	text.format( "%.2f", effectFilterKernel->GetScale() );
	pEditScale->setText( text );
	pChkEnable->setCheck( pEffectFilterKernel->GetEnabled() );
	
	// realize
	create();
}

meWEFFilterKernel::~meWEFFilterKernel(){
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

void meWEFFilterKernel::UpdateFilterKernel(){
	int r, kernelRows = pEffectFilterKernel->GetKernelRows();
	int c, kernelCols = pEffectFilterKernel->GetKernelCols();
	FXHeader *header;
	FXString text;
	
	// resize table if required
	if( pTblKernel->getNumRows() != kernelRows || pTblKernel->getNumColumns() != kernelCols ){
		pTblKernel->setTableSize( kernelRows, kernelCols );
		pTblKernel->setVisibleRows( kernelRows );
		
		// adjust the headers
		header = pTblKernel->getColumnHeader();
		for( c=0; c<kernelCols; c++ ){
			header->setItemSize( c, 50 );
		}
	}
	
	// fill in the filter kernel values
	for( r=0; r<kernelRows; r++ ){
		for( c=0; c<kernelCols; c++ ){
			text.format( "%g", pEffectFilterKernel->GetKernelValueAt( r, c ) );
			pTblKernel->setItemText( r, c, text );
			//pTblKernel->setColumnJustify( r, c,
		}
	}
}

void meWEFFilterKernel::AddTemplate( meWTFilterKernel *aTemplate ){
	if( ! aTemplate ) DETHROW( deeInvalidParam );
	
	if( pTemplateCount == pTemplateSize ){
		int i, newSize = pTemplateCount * 3 / 2 + 1;
		meWTFilterKernel **newArray = new meWTFilterKernel*[ newSize ];
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

long meWEFFilterKernel::onEditRowsCommand( FXObject *sender, FXSelector selector, void *data ){
	int rows = ( int )strtol( pEditRows->getText().text(), NULL, 10 );
	int adjustedRows;
	
	if( rows < 1 ){
		adjustedRows = 1;
	}else if( rows % 2 == 0 ){
		adjustedRows = rows - 1;
	}else{
		adjustedRows = rows;
	}
	
	if( adjustedRows != rows ){
		FXString text;
		text.format( "%i", adjustedRows );
		pEditRows->setText( text );
	}
	
	pEffectFilterKernel->SetKernelSize( adjustedRows, pEffectFilterKernel->GetKernelCols() );
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	UpdateFilterKernel();
	
	return 1;
}

long meWEFFilterKernel::onEditColsCommand( FXObject *sender, FXSelector selector, void *data ){
	int cols = ( int )strtol( pEditCols->getText().text(), NULL, 10 );
	int adjustedCols;
	
	if( cols < 1 ){
		adjustedCols = 1;
	}else if( cols % 2 == 0 ){
		adjustedCols = cols - 1;
	}else{
		adjustedCols = cols;
	}
	
	if( adjustedCols != cols ){
		FXString text;
		text.format( "%i", adjustedCols );
		pEditCols->setText( text );
	}
	
	pEffectFilterKernel->SetKernelSize( pEffectFilterKernel->GetKernelRows(), adjustedCols );
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	UpdateFilterKernel();
	
	return 1;
}

long meWEFFilterKernel::onTableKernelChanged( FXObject *sender, FXSelector selector, void *data ){
	int row = pTblKernel->getCurrentRow();
	int col = pTblKernel->getCurrentColumn();
	
	if( row == -1 || col == -1 ) return 1;
	
	pEffectFilterKernel->SetKernelValueAt( row, col,
		strtof( pTblKernel->getItemText( row, col ).text(), NULL ) );
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	UpdateFilterKernel();
	
	return 1;
}

long meWEFFilterKernel::onEditScaleCommand( FXObject *sender, FXSelector selector, void *data ){
	const float scale = FXFloatVal( pEditScale->getText().text() );
	
	pEffectFilterKernel->SetScale( scale );
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}

long meWEFFilterKernel::onCheckEnableCommand( FXObject *sender, FXSelector selector, void *data ){
	pEffectFilterKernel->SetEnabled( pChkEnable->getCheck() );
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}

long meWEFFilterKernel::onCBTemplatesChanged( FXObject *sender, FXSelector selector, void *data ){
	return 1;
}

long meWEFFilterKernel::onBtnApplyTemplateCommand( FXObject *sender, FXSelector selector, void *data ){
	int selection = pCBTemplates->getCurrentItem();
	int rows, cols, r, c;
	FXString text;
	
	if( selection == -1 ) return 1;
	
	rows = pTemplates[ selection ]->GetKernelRows();
	cols = pTemplates[ selection ]->GetKernelCols();
	
	pEffectFilterKernel->SetKernelSize( rows, cols );
	for( r=0; r<rows; r++ ){
		for( c=0; c<cols; c++ ){
			pEffectFilterKernel->SetKernelValueAt( r, c, pTemplates[ selection ]->GetKernelValueAt( r, c ) );
		}
	}
	pEffectFilterKernel->SetScale( pTemplates[ selection ]->GetScale() );
	
	text.format( "%i", rows );
	pEditRows->setText( text );
	text.format( "%i", cols );
	pEditCols->setText( text );
	UpdateFilterKernel();
	
	pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}

long meWEFFilterKernel::onBtnSaveTemplateCommand( FXObject *sender, FXSelector selector, void *data ){
	//pEffectFilterKernel->SetEnabled( pChkEnable->getCheck() );
	//pWndEffects->GetWindowMain()->UpdateWindows( meWindowMain::eumAllViews );
	
	return 1;
}



// Private Functions
//////////////////////

void meWEFFilterKernel::pAddTemplates(){
	meWTFilterKernel *aTemplate = NULL;
	
	try{
		// Identity
		aTemplate = new meWTFilterKernel( "Null Filter 3x3", 3, 3, 1.0f );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Simple Blur
		aTemplate = new meWTFilterKernel( "Simple Blur 3x3", 3, 3, 1.0f );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		aTemplate->SetKernelValueAt( 0, 0, 0.1f );
		aTemplate->SetKernelValueAt( 1, 0, 0.1f );
		aTemplate->SetKernelValueAt( 2, 0, 0.1f );
		aTemplate->SetKernelValueAt( 0, 1, 0.1f );
		aTemplate->SetKernelValueAt( 1, 1, 0.2f );
		aTemplate->SetKernelValueAt( 2, 1, 0.1f );
		aTemplate->SetKernelValueAt( 0, 2, 0.1f );
		aTemplate->SetKernelValueAt( 1, 2, 0.1f );
		aTemplate->SetKernelValueAt( 2, 2, 0.1f );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Edge detection
		aTemplate = new meWTFilterKernel( "2 Way Edge Detection 3x3", 3, 3, 1.0f );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		aTemplate->SetKernelValueAt( 0, 0, 0.0f );
		aTemplate->SetKernelValueAt( 1, 0, -1.0f );
		aTemplate->SetKernelValueAt( 2, 0, 0.0f );
		aTemplate->SetKernelValueAt( 0, 1, -1.0f );
		aTemplate->SetKernelValueAt( 1, 1, 4.0f );
		aTemplate->SetKernelValueAt( 2, 1, -1.0f );
		aTemplate->SetKernelValueAt( 0, 2, 0.0f );
		aTemplate->SetKernelValueAt( 1, 2, -1.0f );
		aTemplate->SetKernelValueAt( 2, 2, 0.0f );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
		// Edge detection
		aTemplate = new meWTFilterKernel( "3 Way Edge Detection 3x3", 3, 3, 1.0f );
		if( ! aTemplate ) DETHROW( deeOutOfMemory );
		
		aTemplate->SetKernelValueAt( 0, 0, -1.0f );
		aTemplate->SetKernelValueAt( 1, 0, -1.0f );
		aTemplate->SetKernelValueAt( 2, 0, -1.0f );
		aTemplate->SetKernelValueAt( 0, 1, -1.0f );
		aTemplate->SetKernelValueAt( 1, 1, 8.0f );
		aTemplate->SetKernelValueAt( 2, 1, -1.0f );
		aTemplate->SetKernelValueAt( 0, 2, -1.0f );
		aTemplate->SetKernelValueAt( 1, 2, -1.0f );
		aTemplate->SetKernelValueAt( 2, 2, -1.0f );
		
		AddTemplate( aTemplate );
		aTemplate = NULL;
		
	}catch( const deException & ){
		if( aTemplate ) delete aTemplate;
		throw;
	}
}

#endif
