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

#include "dealWidgetLogFile.h"
#include "dealWidgetLabel.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../../dealLauncher.h"
#include "../../logger/deLogger.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetLogFile::LogEntry
//////////////////////////////////////

dealWidgetLogFile::LogEntry::LogEntry(eLogTypes type, const char *source, const char *message) :
pType(type),
pSource(source),
pMessage(message){
}

dealWidgetLogFile::LogEntry::~LogEntry(){
}



// Class dealWidgetLogFile
////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetLogFile::dealWidgetLogFile(dealDisplay &display) :
dealWidgetLayoutFlow(display, false, 0, eltFill, eltBottom),
pColorInfo(0.0f, 0.0f, 0.0f),
pColorWarn(0.5f, 0.0f, 1.0f),
pColorError(1.0f, 0.0f, 0.0f),
pBgLinesAlternate(0.0f, 0.0f, 0.0f, 0.1f),
pMaxLogCount(50),
pLastFileSize(0),
pTimerNextCheck(0),
pVisibleLineCount(10)
{
	SetBackgroundColor(decColor(0.9f, 0.9f, 0.9f));
}

dealWidgetLogFile::~dealWidgetLogFile(){
}



// Management
///////////////

void dealWidgetLogFile::SetFilename(const char *filename){
	if(pFilename == filename){
		return;
	}
	
	RemoveAllWidgets();
	
	pLogs.RemoveAll();
	
	pFilename = filename;
	pPath.SetFromUnix(filename);
	
	pLastFileSize = 0;
	pTimerNextCheck = 0;
	
	CheckFile();
}



void dealWidgetLogFile::CheckFile(){
	if(pPath.IsEmpty()){
		return;
	}
	
	deVirtualFileSystem &vfs = *GetDisplay().GetLauncher().GetFileSystem();
	if(!vfs.ExistsFile(pPath)){
		return;
	}
	
	const uint64_t size = vfs.GetFileSize(pPath);
	if(size <= pLastFileSize){
		return;
	}
	
	if(size - pLastFileSize < 10000){
		decBaseFileReader *reader = NULL;
		decString data;
		
		data.Set(' ', size - pLastFileSize);
		
		try{
			reader = vfs.OpenFileForReading(pPath);
			reader->MovePosition(pLastFileSize);
			reader->Read((char*)data.GetString(), size - pLastFileSize);
			reader->FreeReference();
			
		}catch(const deException &){
			if(reader){
				reader->FreeReference();
			}
		}
		
		const decStringList lines = data.Split('\n');
		const int count = lines.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			AddLogLine(lines.GetAt(i));
		}
		
		// this is required to build the widgets for layout later
		UpdateContent();
	}
	
	pLastFileSize = size;
}

void dealWidgetLogFile::AddLogLine(const decString &line){
	LogEntry::eLogTypes logType = LogEntry::eltInfo;
	LogEntry *logEntry = NULL;
	decString sline(line.GetTrimmed());
	decString source;
	
	if(sline.GetLeft(2) == "WW"){
		logType = LogEntry::eltWarn;
		
	}else if(sline.GetLeft(2) == "EE"){
		logType = LogEntry::eltError;
		
	} // sline.GetLeft(2) == "II"
	
	sline = sline.GetMiddle(3);
	
	if(sline.GetLeft(1) == "["){
		const int position = sline.Find("]");
		if(position != -1){
			source = sline.GetMiddle(1, position).GetTrimmed();
			sline = sline.GetMiddle(position + 2);
		}
	}
	
	while(pLogs.GetCount() >= pMaxLogCount){
		pLogs.RemoveFrom(0);
	}
	
	try{
		logEntry = new LogEntry(logType, source, sline);
		pLogs.Add(logEntry);
		logEntry->FreeReference();
		
	}catch(const deException &){
		if(logEntry){
			logEntry->FreeReference();
		}
	}
}

void dealWidgetLogFile::UpdateContent(){
	const int padding = GetDisplay().GetDefaultFontSize() / 2;
	const int maxLineWidth = decMath::max(GetContentArea().x - padding * 2, 0);
	const int count = pLogs.GetCount();
	dealWidgetLabel *label = NULL;
	decString text;
	int i;
	
	RemoveAllWidgets();
	
	try{
		for(i=0; i<count; i++){
			const LogEntry &logEntry = *((LogEntry*)pLogs.GetAt(i));
			
			switch(logEntry.GetType()){
			case LogEntry::eltInfo:
				text.Format("I [%s] %s", logEntry.GetSource().GetString(), logEntry.GetMessage().GetString());
				label = new dealWidgetLabel(GetDisplay(), text);
				label->SetColor(pColorInfo);
				break;
				
			case LogEntry::eltWarn:
				text.Format("W [%s] %s", logEntry.GetSource().GetString(), logEntry.GetMessage().GetString());
				label = new dealWidgetLabel(GetDisplay(), text);
				label->SetColor(pColorWarn);
				break;
				
			case LogEntry::eltError:
				text.Format("E [%s] %s", logEntry.GetSource().GetString(), logEntry.GetMessage().GetString());
				label = new dealWidgetLabel(GetDisplay(), text);
				label->SetColor(pColorError);
				break;
			}
			
			label->SetMaxLineWidth(maxLineWidth);
			label->SetAlignment(dealWidgetLabel::eaLeft | dealWidgetLabel::eaTop);
			label->SetFontSize(GetDisplay().GetDefaultFontSize() / 2);
			label->SetPadding(GetDisplay().GetDefaultFontSize() / 2, 0);
			if((i % 2) == 1){
				label->SetBackgroundColor(pBgLinesAlternate);
			}
			
			AddWidget(label);
			label->FreeReference();
			label = NULL;
		}
		
	}catch(const deException &){
		if(label){
			label->FreeReference();
		}
	}
}



void dealWidgetLogFile::SetVisibleLineCount(int lineCount){
	lineCount = decMath::max(lineCount, 1);
	
	if(lineCount == pVisibleLineCount){
		return;
	}
	
	pVisibleLineCount = lineCount;
	DirtyLayout();
}



decPoint dealWidgetLogFile::GetMinimumSize(){
	if(GetHasExplicitMinimumSize()){
		return GetExplicitMinimumSize();
	}
	
	return decPoint(50, GetDisplay().GetDefaultFontSize() * pVisibleLineCount);
}

void dealWidgetLogFile::RenderContent(const sRenderContext &context){
	pTimerNextCheck--;
	
	if(pTimerNextCheck <= 0){
		CheckFile();
		pTimerNextCheck = 30; // assuming 30 FPS
	}
	
	dealWidgetLayoutFlow::RenderContent(context);
}
