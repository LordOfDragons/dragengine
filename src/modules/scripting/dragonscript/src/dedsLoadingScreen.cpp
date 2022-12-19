/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "dedsLoadingScreen.h"
#include "deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
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
	deCanvasView * const content = pDS.GetGameEngine()->GetGraphicSystem()->GetRenderWindow()->GetCanvasView();
	if( ! content ){
		return;
	}
	
	const decPoint &size = content->GetSize();
	
	if( pCanvasBackground ){
		pDS.LogInfoFormat( "UPDATE %dx%d", size.x, size.y );
		pCanvasBackground->SetSize( size );
	}
}



// Private Functions
//////////////////////

void dedsLoadingScreen::pCreateScreen(){
	pDS.LogInfo( "Create loading screen" );
	deEngine &engine = *pDS.GetGameEngine();
	deCanvasManager &canvasManager = *engine.GetCanvasManager();
	
	deCanvasView * const content = engine.GetGraphicSystem()->GetRenderWindow()->GetCanvasView();
	DEASSERT_NOTNULL( content )
	
	pCanvasBackground.TakeOver( canvasManager.CreateCanvasPaint() );
	pCanvasBackground->SetFillColor( decColor( 0.0f, 0.0f, 0.0f ) );
	pCanvasBackground->SetShapeType( deCanvasPaint::estRectangle );
	pCanvasBackground->SetThickness( 0.0f );
	pCanvasBackground->SetOrder( content->GetCanvasCount() );
	content->AddCanvas( pCanvasBackground );
}

void dedsLoadingScreen::pCleanUp(){
	pDS.LogInfo( "Clean up loading screen" );
	deCanvasView * const content = pDS.GetGameEngine()->GetGraphicSystem()->GetRenderWindow()->GetCanvasView();
	
	if( pCanvasBackground ){
		if( content ){
			try{
				content->RemoveCanvas( pCanvasBackground );
			}catch( ... ){}
		}
		pCanvasBackground = nullptr;
	}
}
