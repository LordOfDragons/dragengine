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

#include "deCanvas.h"
#include "deCanvasImage.h"
#include "deCanvasPaint.h"
#include "deCanvasCanvasView.h"
#include "deCanvasRenderWorld.h"
#include "deCanvasText.h"
#include "deCanvasVideoPlayer.h"
#include "deCanvasView.h"
#include "deCanvasVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deCanvasVisitor
//////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasVisitor::deCanvasVisitor(){
}

deCanvasVisitor::~deCanvasVisitor(){
}



// Visiting
/////////////

void deCanvasVisitor::VisitCanvas(deCanvas &canvas){
}

void deCanvasVisitor::VisitImage(deCanvasImage &canvas){
	VisitCanvas(canvas);
}

void deCanvasVisitor::VisitPaint(deCanvasPaint &canvas){
	VisitCanvas(canvas);
}

void deCanvasVisitor::VisitCanvasView(deCanvasCanvasView &canvas){
	VisitCanvas(canvas);
}

void deCanvasVisitor::VisitRenderWorld(deCanvasRenderWorld &canvas){
	VisitCanvas(canvas);
}

void deCanvasVisitor::VisitText(deCanvasText &canvas){
	VisitCanvas(canvas);
}

void deCanvasVisitor::VisitVideoPlayer(deCanvasVideoPlayer &canvas){
	VisitCanvas(canvas);
}

void deCanvasVisitor::VisitView(deCanvasView &canvas){
	VisitCanvas(canvas);
}
