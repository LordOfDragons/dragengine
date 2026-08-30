/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeNativeBeosVFSListItem.h"
#include "../../../igdeUIHelper.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVFSContainer.h>


// Class igdeNativeBeosVFSListItem
//////////////////////////////////

igdeNativeBeosVFSListItem::igdeNativeBeosVFSListItem(const decPath &path, const char *filename) :
pPath(path),
pFileName(filename),
pFileType(deVFSContainer::eftRegularFile),
pFileSize(0),
pFileModificationTime(0){
}

igdeNativeBeosVFSListItem::~igdeNativeBeosVFSListItem() = default;


// Management
//////////////

void igdeNativeBeosVFSListItem::SetFileName(const char *filename){
	pFileName = filename;
}

void igdeNativeBeosVFSListItem::SetFileType(deVFSContainer::eFileTypes fileType){
	pFileType = fileType;
}

void igdeNativeBeosVFSListItem::SetFileSize(int fileSize){
	DEASSERT_TRUE(fileSize >= 0)
	pFileSize = fileSize;
}

void igdeNativeBeosVFSListItem::SetFileModificationTime(TIME_SYSTEM time){
	pFileModificationTime = time;
}

bool igdeNativeBeosVFSListItem::IsDirectory() const{
	return pFileType == deVFSContainer::eftDirectory;
}

decString igdeNativeBeosVFSListItem::UpdateText(const char *pattern) const{
	if(!pattern){
		return pFileName;
	}
	
	decString text;
	const char *p = pattern;
	
	while(*p){
		switch(*p){
		case 'n':
			text += pFileName;
			break;
			
		case 's':
			if(IsDirectory()){
				text += "<dir>";
				
			}else{
				text = igdeUIHelper::FormatSize1024(pFileSize);
			}
			break;
			
		case 'm': {
			decDateTime dateTime(pFileModificationTime);
			text.AppendFormat("%04d-%02d-%02d %02d:%02d:%02d",
				dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay(),
				dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
			}
			break;
			
		case 't':
			switch(pFileType){
			case deVFSContainer::eftDirectory:
				text += "<DIR>";
				break;
				
			case deVFSContainer::eftRegularFile:
				text += "File";
				break;
				
			default:
				text += "Unknown";
			}
			break;
			
		case '|':
			text += " | ";
			break;
			
		default:
			text.AppendCharacter(*p);
		}
		p++;
	}
	
	return text;
}
