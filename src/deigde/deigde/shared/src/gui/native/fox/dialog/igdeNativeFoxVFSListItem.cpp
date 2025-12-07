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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxVFSListItem.h"

#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/common/exceptions.h>



// Class igdeNativeFoxVFSListItem
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxVFSListItem::igdeNativeFoxVFSListItem(const decPath &path, const FXString &text, FXIcon *bi, FXIcon *mi) :
FXIconItem(text, bi, mi),
pPath(path),
pFileType(deVFSContainer::eftRegularFile),
pFileSize(0),
pFileModificationTime(0){
}

igdeNativeFoxVFSListItem::~igdeNativeFoxVFSListItem(){
}



// Management
///////////////

void igdeNativeFoxVFSListItem::SetFileName(const char *filename){
	pFileName = filename;
}

void igdeNativeFoxVFSListItem::SetFileType(deVFSContainer::eFileTypes fileType){
	pFileType = fileType;
}

void igdeNativeFoxVFSListItem::SetFileSize(int fileSize){
	pFileSize = fileSize;
}

void igdeNativeFoxVFSListItem::SetFileModificationTime(TIME_SYSTEM time){
	pFileModificationTime = time;
}

bool igdeNativeFoxVFSListItem::IsDirectory() const{
	return pFileType == deVFSContainer::eftDirectory;
}



void igdeNativeFoxVFSListItem::UpdateText(const char* pattern){
	FXString text, format;
	
	while(*pattern){
		if(! text.empty()){
			text.append("\t");
		}
		
		if(*pattern == 'n'){
			text.append(pFileName);
			
		}else if(*pattern == 's'){
			if(IsDirectory()){
				text.append("-");
				
			}else{
				if(pFileSize < 1024){
					format.format("%i B", pFileSize);
					text.append(format);
					
				}else if(pFileSize < 1024000){
					format.format("%0.1f KiB", (double)pFileSize / 1024.0);
					text.append(format);
					
				}else if(pFileSize < 1024000000){
					format.format("%0.1f MiB", (double)pFileSize / 1024000.0);
					text.append(format);
					
				}else{
					format.format("%0.1f GiB", (double)pFileSize / 1024000000.0);
					text.append(format);
				}
			}
			
		}else if(*pattern == 'm'){
			if(IsDirectory()){
				text.append("-");
				
			}else{
				const decDateTime datetime(pFileModificationTime);
				format.format("%02i:%02i %02i.%02i.%04i", datetime.GetHour(), datetime.GetMinute(),
					datetime.GetDay() + 1, datetime.GetMonth() + 1, datetime.GetYear());
				text.append(format);
			}
			
		}else if(*pattern == 't'){
			if(IsDirectory()){
				text.append("Directory");
				
			}else{
				text.append("File");
			}
		}
		
		pattern++;
	}
	
	setText(text);
}



// Sorting
////////////

/*
#ifdef OLD_STRING_COMPARE_NS
	#define FOX_STRING_COMPARE compare
#else
	#define FOX_STRING_COMPARE FXString::compare
#endif
*/

static FXint foxStringCompare(const FXString &s1, const FXString &s2){
	return strcmp(s1.text(), s2.text());
}
#define FOX_STRING_COMPARE foxStringCompare

FXint igdeNativeFoxVFSListItem::fSortNameAsc(const FXIconItem *item1, const FXIconItem *item2){
	const igdeNativeFoxVFSListItem &file1 = *((igdeNativeFoxVFSListItem*)item1);
	const igdeNativeFoxVFSListItem &file2 = *((igdeNativeFoxVFSListItem*)item2);
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if(isDir1 == isDir2){
		return FOX_STRING_COMPARE(file1.GetFileName(), file2.GetFileName());
		
	}else{
		if(isDir1 && ! isDir2){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortNameDesc(const FXIconItem *item1, const FXIconItem *item2){
	const igdeNativeFoxVFSListItem &file1 = *((igdeNativeFoxVFSListItem*)item1);
	const igdeNativeFoxVFSListItem &file2 = *((igdeNativeFoxVFSListItem*)item2);
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if(isDir1 == isDir2){
		return FOX_STRING_COMPARE(file2.GetFileName(), file1.GetFileName());
		
	}else{
		if(isDir1 && ! isDir2){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortSizeAsc(const FXIconItem *item1, const FXIconItem *item2){
	const igdeNativeFoxVFSListItem &file1 = *((igdeNativeFoxVFSListItem*)item1);
	const igdeNativeFoxVFSListItem &file2 = *((igdeNativeFoxVFSListItem*)item2);
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if(isDir1 == isDir2){
		FXint result = (FXint)(file1.GetFileSize() - file2.GetFileSize());
		if(result == 0){ // same size, sort by name instead
			result = FOX_STRING_COMPARE(file1.GetFileName(), file2.GetFileName());
		}
		return result;
		
	}else{
		if(isDir1 && ! isDir2){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortSizeDesc(const FXIconItem *item1, const FXIconItem *item2){
	const igdeNativeFoxVFSListItem &file1 = *((igdeNativeFoxVFSListItem*)item1);
	const igdeNativeFoxVFSListItem &file2 = *((igdeNativeFoxVFSListItem*)item2);
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if(isDir1 == isDir2){
		FXint result = (FXint)(file2.GetFileSize() - file1.GetFileSize());
		if(result == 0){ // same size, sort by name instead
			result = FOX_STRING_COMPARE(file2.GetFileName(), file1.GetFileName());
		}
		return result;
		
	}else{
		if(isDir1 && ! isDir2){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortModTimeAsc(const FXIconItem *item1, const FXIconItem *item2){
	const igdeNativeFoxVFSListItem &file1 = *((igdeNativeFoxVFSListItem*)item1);
	const igdeNativeFoxVFSListItem &file2 = *((igdeNativeFoxVFSListItem*)item2);
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if(isDir1 == isDir2){
		FXint result = (FXint)(file2.GetFileModificationTime() - file1.GetFileModificationTime());
		if(result == 0){ // same size, sort by name instead
			result = FOX_STRING_COMPARE(file1.GetFileName(), file2.GetFileName());
		}
		return result;
		
	}else{
		if(isDir1 && ! isDir2){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortModTimeDesc(const FXIconItem *item1, const FXIconItem *item2){
	const igdeNativeFoxVFSListItem &file1 = *((igdeNativeFoxVFSListItem*)item1);
	const igdeNativeFoxVFSListItem &file2 = *((igdeNativeFoxVFSListItem*)item2);
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if(isDir1 == isDir2){
		FXint result = (FXint)(file1.GetFileModificationTime() - file2.GetFileModificationTime());
		if(result == 0){ // same size, sort by name instead
			result = FOX_STRING_COMPARE(file2.GetFileName(), file1.GetFileName());
		}
		return result;
		
	}else{
		if(isDir1 && ! isDir2){
			return -1;
			
		}else{
			return 1;
		}
	}
}

#endif
