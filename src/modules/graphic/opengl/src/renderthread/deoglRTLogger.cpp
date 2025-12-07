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

deoglRTLogger::deoglRTLogger(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
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



void deoglRTLogger::LogInfo(const char *message){
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogInfo(LOGGING_SOURCE, message);
		
	}else{
		pRenderThread.GetOgl().LogInfo(message);
	}
}

void deoglRTLogger::LogInfoFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogInfoFormatUsing(LOGGING_SOURCE, message, list);
		
	}else{
		pRenderThread.GetOgl().LogInfoFormatUsing(message, list);
	}
	
	va_end(list);
}

void deoglRTLogger::LogInfoFormatUsing(const char *message, va_list args){
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogInfoFormatUsing(LOGGING_SOURCE, message, args);
		
	}else{
		pRenderThread.GetOgl().LogInfoFormatUsing(message, args);
	}
}

void deoglRTLogger::LogWarn(const char *message){
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogWarn(LOGGING_SOURCE, message);
		
	}else{
		pRenderThread.GetOgl().LogWarn(message);
	}
}

void deoglRTLogger::LogWarnFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogWarnFormatUsing(LOGGING_SOURCE, message, list);
		
	}else{
		pRenderThread.GetOgl().LogWarnFormatUsing(message, list);
	}
	
	va_end(list);
}

void deoglRTLogger::LogWarnFormatUsing(const char *message, va_list args){
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogWarnFormatUsing(LOGGING_SOURCE, message, args);
		
	}else{
		pRenderThread.GetOgl().LogWarnFormatUsing(message, args);
	}
}

void deoglRTLogger::LogError(const char *message){
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogError(LOGGING_SOURCE, message);
		
	}else{
		pRenderThread.GetOgl().LogError(message);
	}
}

void deoglRTLogger::LogErrorFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogErrorFormatUsing(LOGGING_SOURCE, message, list);
		
	}else{
		pRenderThread.GetOgl().LogErrorFormatUsing(message, list);
	}
	
	va_end(list);
}

void deoglRTLogger::LogErrorFormatUsing(const char *message, va_list args){
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogErrorFormatUsing(LOGGING_SOURCE, message, args);
		
	}else{
		pRenderThread.GetOgl().LogErrorFormatUsing(message, args);
	}
}

void deoglRTLogger::LogException(const deException &exception){
	if(pRenderThread.GetAsyncRendering()){
		GetLogger().LogException(LOGGING_SOURCE, exception);
		
	}else{
		pRenderThread.GetOgl().LogException(exception);
	}
}
