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

#include "deoalATLogger.h"
#include "deoalAudioThread.h"
#include "../deAudioOpenAL.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGGING_SOURCE "OpenAL-AT"



// Class deoalATLogger
////////////////////////

// Constructor, destructor
////////////////////////////

deoalATLogger::deoalATLogger(deoalAudioThread &audioThread) :
pAudioThread(audioThread){
}

deoalATLogger::~deoalATLogger(){
}



// Management
///////////////

deLogger &deoalATLogger::GetLogger(){
	return *pAudioThread.GetOal().GetGameEngine()->GetLogger();
}

void deoalATLogger::Synchronize(){
}



void deoalATLogger::LogInfo(const char *message){
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogInfo(LOGGING_SOURCE, message);
		
	}else{
		pAudioThread.GetOal().LogInfo(message);
	}
}

void deoalATLogger::LogInfoFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogInfoFormatUsing(LOGGING_SOURCE, message, list);
		
	}else{
		pAudioThread.GetOal().LogInfoFormatUsing(message, list);
	}
	
	va_end(list);
}

void deoalATLogger::LogInfoFormatUsing(const char *message, va_list args){
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogInfoFormatUsing(LOGGING_SOURCE, message, args);
		
	}else{
		pAudioThread.GetOal().LogInfoFormatUsing(message, args);
	}
}

void deoalATLogger::LogWarn(const char *message){
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogWarn(LOGGING_SOURCE, message);
		
	}else{
		pAudioThread.GetOal().LogWarn(message);
	}
}

void deoalATLogger::LogWarnFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogWarnFormatUsing(LOGGING_SOURCE, message, list);
		
	}else{
		pAudioThread.GetOal().LogWarnFormatUsing(message, list);
	}
	
	va_end(list);
}

void deoalATLogger::LogWarnFormatUsing(const char *message, va_list args){
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogWarnFormatUsing(LOGGING_SOURCE, message, args);
		
	}else{
		pAudioThread.GetOal().LogWarnFormatUsing(message, args);
	}
}

void deoalATLogger::LogError(const char *message){
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogError(LOGGING_SOURCE, message);
		
	}else{
		pAudioThread.GetOal().LogError(message);
	}
}

void deoalATLogger::LogErrorFormat(const char *message, ...){
	va_list list;
	va_start(list, message);
	
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogErrorFormatUsing(LOGGING_SOURCE, message, list);
		
	}else{
		pAudioThread.GetOal().LogErrorFormatUsing(message, list);
	}
	
	va_end(list);
}

void deoalATLogger::LogErrorFormatUsing(const char *message, va_list args){
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogErrorFormatUsing(LOGGING_SOURCE, message, args);
		
	}else{
		pAudioThread.GetOal().LogErrorFormatUsing(message, args);
	}
}

void deoalATLogger::LogException(const deException &exception){
	if(pAudioThread.GetAsyncAudio()){
		GetLogger().LogException(LOGGING_SOURCE, exception);
		
	}else{
		pAudioThread.GetOal().LogException(exception);
	}
}
