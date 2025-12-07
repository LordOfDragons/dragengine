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

// includes
#include "decrbPanelErrorTrace.h"
#include "decrbWindowMain.h"
#include "dragengine/deEngine.h"
#include "dragengine/errortracing/deErrorTrace.h"
#include "dragengine/errortracing/deErrorTracePoint.h"
#include "dragengine/errortracing/deErrorTraceValue.h"
#include "dragengine/systems/modules/deLoadableModule.h"
#include "dragengine/common/exceptions.h"



// Events
///////////

//FXDEFMAP( decrbPanelErrorTrace ) decrbPanelErrorTraceMap[] = {
//};



// Class decrbPanelErrorTrace
///////////////////////////////
	
//FXIMPLEMENT( decrbPanelErrorTrace, FXVerticalFrame, decrbPanelErrorTraceMap, ARRAYNUMBER( decrbPanelErrorTraceMap ) )
FXIMPLEMENT(decrbPanelErrorTrace, FXVerticalFrame, nullptr, 0)

// Constructor, destructor
////////////////////////////

decrbPanelErrorTrace::decrbPanelErrorTrace(){}

decrbPanelErrorTrace::decrbPanelErrorTrace(decrbWindowMain *windowMain, FXComposite *container) :
FXVerticalFrame(container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 5, 5, 5, 5){
	if(! windowMain) DETHROW(deeInvalidParam);
	int padding = 3;
	int spacing = 3;
	
	// prepare
	pWndMain = windowMain;
	
	// engine status
	FXHorizontalFrame *frameLine = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Error:");
	pEditError = new FXTextField(frameLine, 10, NULL, 0, TEXTFIELD_READONLY | FRAME_SUNKEN | LAYOUT_FILL_X);
	
	FXGroupBox *groupBox = new FXGroupBox(this, "Trace:", GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X
		| LAYOUT_FILL_Y, 0, 0, 0, 0,
		padding, padding, padding, padding);
	FXVerticalFrame *frameBox = new FXVerticalFrame(groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	pTreeTrace = new FXTreeList(frameBox, NULL, ID_TREETRACE, TREELIST_EXTENDEDSELECT
		| TREELIST_SHOWS_LINES | TREELIST_SHOWS_BOXES | TREELIST_ROOT_BOXES
		| FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	UpdateTrace();
}

decrbPanelErrorTrace::~decrbPanelErrorTrace(){
}



// Management
///////////////

void decrbPanelErrorTrace::UpdateTrace(){
	deErrorTrace *trace = pWndMain->GetEngine()->GetErrorTrace();
	int i, pointCount = trace->GetPointCount();
	deErrorTracePoint *tracePoint;
	FXTreeItem *treeItem;
	FXString traceText;
	int j, valueCount;
	
	pEditError->setText(decrbWindowMain::GetTextForError(trace->GetError()));
	
	for(i=0; i<pointCount; i++){
		tracePoint = trace->GetPoint(i);
		
		/*
		if(tracePoint->GetSourceModule()){
			traceText.format("%i) %s, %s at %i\n", i + 1, tracePoint->GetSourceModule()->GetName(),
				tracePoint->GetSourceFunction(), tracePoint->GetSourceLine());
		}else{
			traceText.format("%i) Game Engine, %s at %i\n", i + 1, tracePoint->GetSourceFunction(),
				tracePoint->GetSourceLine());
		}
		pEditTrace->appendText(traceText.text(), traceText.length());
		*/
		
		if(tracePoint->GetSourceModule()){
			traceText.format("%i. %s, %s at %i", i + 1,
				tracePoint->GetSourceModule()->GetName().GetString(),
				tracePoint->GetSourceFunction().GetString(),
				tracePoint->GetSourceLine());
			
		}else{
			traceText.format("%i. Game Engine, %s at %i", i + 1,
				tracePoint->GetSourceFunction().GetString(),
				tracePoint->GetSourceLine());
		}
		
		treeItem = pTreeTrace->appendItem(NULL, traceText, NULL, NULL, NULL, false);
		valueCount = tracePoint->GetValueCount();
		for(j=0; j<valueCount; j++){
			pAddErrorTraceSubValues(0, treeItem, tracePoint->GetValue(j));
		}
		
		//pEditTrace->appendText( "\n", 1 );
	}
}



// Events
///////////



// Private Functions
//////////////////////

void decrbPanelErrorTrace::pAddErrorTraceSubValues(int level, FXTreeItem *treeItem, deErrorTraceValue *traceValue){
	int i, valueCount = traceValue->GetSubValueCount();
	FXTreeItem *subTreeItem;
	FXString traceText;
	
	// add value
	/*
	if(traceValue->GetSubValueCount() > 0){
		boxChar = '+';
	}else{
		boxChar = '-';
	}
	traceText.format("  %c %s = '%s'\n", boxChar, traceValue->GetName(), traceValue->GetValue());
	for(i=0; i<level; i++) traceText.prepend("  ");
	pEditTrace->appendText(traceText.text(), traceText.length());
	*/
	
	traceText.format("%s = %s", traceValue->GetName().GetString(), traceValue->GetValue().GetString());
	subTreeItem = pTreeTrace->appendItem(treeItem, traceText, NULL, NULL, NULL, false);
	
	for(i=0; i<valueCount; i++){
		pAddErrorTraceSubValues(level + 1, subTreeItem, traceValue->GetSubValue(i));
	}
}
