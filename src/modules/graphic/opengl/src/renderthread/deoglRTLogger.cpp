/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRTLogger.h"
#include "deoglRenderThread.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>



// Definitions
////////////////

#define LOGGING_SOURCE "OpenGL-RT"



// Class deoglRTLogger
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTLogger::deoglRTLogger( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglRTLogger::~deoglRTLogger(){
}



// Management
///////////////

deLogger &deoglRTLogger::GetLogger(){
	return *pRenderThread.GetOgl().GetGameEngine()->GetLogger();
}

void deoglRTLogger::Synchronize(){
}



void deoglRTLogger::LogInfo( const char *message ){
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogInfo( LOGGING_SOURCE, message );
		
	}else{
		pRenderThread.GetOgl().LogInfo( message );
	}
}

void deoglRTLogger::LogInfoFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogInfoFormatUsing( LOGGING_SOURCE, message, list );
		
	}else{
		pRenderThread.GetOgl().LogInfoFormatUsing( message, list );
	}
	
	va_end( list );
}

void deoglRTLogger::LogInfoFormatUsing( const char *message, va_list args ){
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogInfoFormatUsing( LOGGING_SOURCE, message, args );
		
	}else{
		pRenderThread.GetOgl().LogInfoFormatUsing( message, args );
	}
}

void deoglRTLogger::LogWarn( const char *message ){
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogWarn( LOGGING_SOURCE, message );
		
	}else{
		pRenderThread.GetOgl().LogWarn( message );
	}
}

void deoglRTLogger::LogWarnFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogWarnFormatUsing( LOGGING_SOURCE, message, list );
		
	}else{
		pRenderThread.GetOgl().LogWarnFormatUsing( message, list );
	}
	
	va_end( list );
}

void deoglRTLogger::LogWarnFormatUsing( const char *message, va_list args ){
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogWarnFormatUsing( LOGGING_SOURCE, message, args );
		
	}else{
		pRenderThread.GetOgl().LogWarnFormatUsing( message, args );
	}
}

void deoglRTLogger::LogError( const char *message ){
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogError( LOGGING_SOURCE, message );
		
	}else{
		pRenderThread.GetOgl().LogError( message );
	}
}

void deoglRTLogger::LogErrorFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogErrorFormatUsing( LOGGING_SOURCE, message, list );
		
	}else{
		pRenderThread.GetOgl().LogErrorFormatUsing( message, list );
	}
	
	va_end( list );
}

void deoglRTLogger::LogErrorFormatUsing( const char *message, va_list args ){
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogErrorFormatUsing( LOGGING_SOURCE, message, args );
		
	}else{
		pRenderThread.GetOgl().LogErrorFormatUsing( message, args );
	}
}

void deoglRTLogger::LogException( const deException &exception ){
	if( pRenderThread.GetAsyncRendering() ){
		GetLogger().LogException( LOGGING_SOURCE, exception );
		
	}else{
		pRenderThread.GetOgl().LogException( exception );
	}
}
