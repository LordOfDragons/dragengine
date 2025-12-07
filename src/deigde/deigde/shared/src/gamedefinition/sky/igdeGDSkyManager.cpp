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

#include "igdeGDSky.h"
#include "igdeGDSkyManager.h"
#include "../igdeGDCategory.h"
#include "../igdeGameDefinition.h"
#include "../visitor/igdeGDVisitor.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeGDSkyManager
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkyManager::igdeGDSkyManager(){
	pCategories.TakeOver(new igdeGDCategory("Skies"));
}

igdeGDSkyManager::~igdeGDSkyManager(){
}



// Management
///////////////

void igdeGDSkyManager::AddSky(igdeGDSky *sky){
	if(! sky){
		DETHROW(deeInvalidParam);
	}
	pSkyList.Add(sky);
}

void igdeGDSkyManager::RemoveSky(igdeGDSky *sky){
	pSkyList.Remove(sky);
}

void igdeGDSkyManager::RemoveAllSkies(){
	pSkyList.RemoveAll();
}



void igdeGDSkyManager::SetDefaultPath(const char *path){
	pDefaultPath = path;
}



void igdeGDSkyManager::VisitSkiesMatchingCategory(igdeGDVisitor &visitor, const igdeGDCategory *category) const{
	const int count = pSkyList.GetCount();
	decPath pathCat;
	int i;
	
	if(category){
		category->GetFullPath(pathCat);
	}
	const decString strPathCat(pathCat.GetPathUnix());
	
	for(i=0; i<count; i++){
		igdeGDSky * const sky = (igdeGDSky*)pSkyList.GetAt(i);
		if(sky->GetCategory() == strPathCat){
			visitor.VisitSky(sky);
		}
	}
}

void igdeGDSkyManager::VisitMatchingFilter(igdeGDVisitor &visitor, const decString &filter) const{
	if(filter.GetLength() < 2){
		return;
	}
	
	const decString realFilter(filter.GetLower());
	const int count = pSkyList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeGDSky * const sky = (igdeGDSky*)pSkyList.GetAt(i);
		if(sky->GetName().GetLower().FindString(realFilter) != -1
		|| sky->GetPath().GetLower().FindString(realFilter) != -1){
			visitor.VisitSky(sky);
		}
	}
}



void igdeGDSkyManager::UpdateWith(const igdeGDSkyManager &manager){
	const int count = manager.GetSkyList().GetCount();
	igdeGDSky *sky = NULL;
	igdeGDSky *skyCheck;
	int i;
	
	try{
		for(i=0; i<count; i++){
			sky = new igdeGDSky(*manager.GetSkyList().GetAt(i));
			skyCheck = pSkyList.GetWithPath(sky->GetPath());
			if(skyCheck){
				RemoveSky(skyCheck);
			}
			AddSky(sky);
			sky->FreeReference();
			sky = NULL;
		}
		
	}catch(const deException &){
		if(sky){
			sky->FreeReference();
		}
		throw;
	}
	
	pCategories->UpdateWith(manager.pCategories);
	pAutoFindPath = manager.pAutoFindPath;
	
	if(! manager.pDefaultPath.IsEmpty()){
		pDefaultPath = manager.pDefaultPath;
	}
}

void igdeGDSkyManager::UpdateWithFound(const igdeGDSkyManager &skyManager){
	const int count = skyManager.GetSkyList().GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const igdeGDSky &foundSky = *skyManager.GetSkyList().GetAt(i);
		
		igdeGDSky * const skyCheck = pSkyList.GetWithPath(foundSky.GetPath());
		if(skyCheck){
			continue;
		}
		
		const igdeGDSky::Ref sky(igdeGDSky::Ref::NewWith(foundSky));
		
		igdeGDCategory * const autoCategory = pCategories->AutoCategorize(sky->GetPath());
		if(autoCategory){
			sky->SetCategory(autoCategory->GetFullPathString());
		}
		
		AddSky(sky);
	}
}

class igdeGDSkyManagerFind : public deFileSearchVisitor{
private:
	igdeGDSkyManager &pOwner;
	const char * const pPattern;
	igdeGDSky::Ref pSky;
	
public:
	igdeGDSkyManagerFind(igdeGDSkyManager &owner, const char *pattern) :
	pOwner(owner), pPattern(pattern){
	}
	
	virtual bool VisitFile(const deVirtualFileSystem &vfs, const decPath &path){
		if(! path.GetLastComponent().MatchesPattern(pPattern)){
			return true;
		}
		
		const decString fullPath(path.GetPathUnix());
		if(pOwner.GetSkyList().HasWithPath(fullPath)){
			return true;
		}
		
		decString genName(fullPath.GetReplaced(decPath::PathSeparator(), ' '));
		const int index = genName.FindReverse('.');
		if(index != -1){
			genName = genName.GetLeft(index);
		}
		
		try{
			pSky.TakeOverWith(fullPath, genName);
			pSky->SetDescription("Auto-Imported");
			pOwner.AddSky(pSky);
			
		}catch(const deException &){
		}
		
		return true;
	}
	
	virtual bool VisitDirectory(const deVirtualFileSystem &vfs, const decPath &path){
		vfs.SearchFiles(path, *this);
		return true;
	}
};

void igdeGDSkyManager::FindAndAddSkies(deVirtualFileSystem &vfs,
const decPath &directory, const char *pattern, bool recursive){
	igdeGDSkyManagerFind process(*this, pattern);
	vfs.SearchFiles(directory, process);
}
