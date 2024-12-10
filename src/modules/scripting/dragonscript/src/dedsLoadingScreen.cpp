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

#include "dedsLoadingScreen.h"
#include "deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deGraphicSystem.h>



// Class dedsLoadingScreen
////////////////////////////

// Constructor
////////////////

dedsLoadingScreen::dedsLoadingScreen( deScriptingDragonScript &ds ) :
pDS( ds )
{
	try{
		pCreateScreen();
		Update();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dedsLoadingScreen::~dedsLoadingScreen(){
	pCleanUp();
}



// Management
///////////////

void dedsLoadingScreen::Update(){
	deCanvasView * const content = pDS.GetGameEngine()->
		GetGraphicSystem()->GetRenderWindow()->GetCanvasView();
	if(!content){
		return;
	}
	
	const decPoint &size = content->GetSize();
	
	if(pCanvasBackground){
		pCanvasBackground->SetSize(size);
	}
	
	if(pCanvasImage){
		const decPoint &imageSize = pCanvasImage->GetSize();
		pCanvasImage->SetPosition(decPoint((size.x - imageSize.x) / 2, size.y - imageSize.y - 20));
	}
}



// Private Functions
//////////////////////

void dedsLoadingScreen::pCreateScreen(){
	pDS.LogInfo( "Create loading screen" );
	deEngine &engine = *pDS.GetGameEngine();
	deCanvasManager &canvasManager = *engine.GetCanvasManager();
	deImageManager &imageManager = *engine.GetImageManager();
	
	deCanvasView * const content = engine.GetGraphicSystem()->GetRenderWindow()->GetCanvasView();
	DEASSERT_NOTNULL( content )
	
	pCanvasBackground.TakeOver( canvasManager.CreateCanvasPaint() );
	pCanvasBackground->SetFillColor( decColor( 0.0f, 0.0f, 0.0f ) );
	pCanvasBackground->SetShapeType( deCanvasPaint::estRectangle );
	pCanvasBackground->SetThickness( 0.0f );
	pCanvasBackground->SetOrder( ( float )content->GetCanvasCount() );
	content->AddCanvas( pCanvasBackground );
	
	const deImage::Ref image( deImage::Ref::New(
		imageManager.LoadImage( "loadingScreen.webp", "/shareddata/images" ) ) );
	
	pCanvasImage.TakeOver( canvasManager.CreateCanvasImage() );
	pCanvasImage->SetImage( image );
	pCanvasImage->SetSize( decPoint( image->GetWidth(), image->GetHeight() ) );
	pCanvasImage->SetOrder( ( float )content->GetCanvasCount() );
	pCanvasImage->SetTransparency( 0.5f );
	content->AddCanvas( pCanvasImage );
}

void dedsLoadingScreen::pCleanUp(){
	pDS.LogInfo( "Clean up loading screen" );
	deCanvasView * const content = pDS.GetGameEngine()->GetGraphicSystem()->GetRenderWindow()->GetCanvasView();
	
	if( pCanvasImage ){
		if( content ){
			try{
				content->RemoveCanvas( pCanvasImage );
			}catch( ... ){}
		}
		pCanvasImage = nullptr;
	}
	
	if( pCanvasBackground ){
		if( content ){
			try{
				content->RemoveCanvas( pCanvasBackground );
			}catch( ... ){}
		}
		pCanvasBackground = nullptr;
	}
}
