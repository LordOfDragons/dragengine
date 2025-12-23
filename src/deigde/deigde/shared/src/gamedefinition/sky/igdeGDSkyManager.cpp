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
	pCategories = igdeGDCategory::Ref::New("Skies");
}

igdeGDSkyManager::~igdeGDSkyManager(){
}



// Management
///////////////

void igdeGDSkyManager::AddSky(igdeGDSky *sky){
	if(!sky){
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
	decPath pathCat;
	
	if(category){
		category->GetFullPath(pathCat);
	}
	const decString strPathCat(pathCat.GetPathUnix());
	
	pSkyList.Visit([&](igdeGDSky *sky){
		if(sky->GetCategory() == strPathCat){
			visitor.VisitSky(sky);
		}
	});
}

void igdeGDSkyManager::VisitMatchingFilter(igdeGDVisitor &visitor, const decString &filter) const{
	if(filter.GetLength() < 2){
		return;
	}
	
	const decString realFilter(filter.GetLower());
	
	pSkyList.Visit([&](igdeGDSky *sky){
		if(sky->GetName().GetLower().FindString(realFilter) != -1
		|| sky->GetPath().GetLower().FindString(realFilter) != -1){
			visitor.VisitSky(sky);
		}
	});
}



void igdeGDSkyManager::UpdateWith(const igdeGDSkyManager &manager){
	manager.GetSkyList().Visit([&](const igdeGDSky &sky){
		const igdeGDSky::Ref skyCopy(igdeGDSky::Ref::New(sky));
		igdeGDSky * const check = pSkyList.FindOrDefault([&](const igdeGDSky &s){
			return s.GetPath() == skyCopy->GetPath();
		});
		if(check){
			RemoveSky(check);
		}
		AddSky(skyCopy);
	});
	
	pCategories->UpdateWith(manager.pCategories);
	pAutoFindPath = manager.pAutoFindPath;
	
	if(!manager.pDefaultPath.IsEmpty()){
		pDefaultPath = manager.pDefaultPath;
	}
}

void igdeGDSkyManager::UpdateWithFound(const igdeGDSkyManager &skyManager){
	skyManager.GetSkyList().Visit([&](const igdeGDSky &foundSky){
		const igdeGDSky::Ref skyCheck(pSkyList.FindOrDefault([&](const igdeGDSky &s){
			return s.GetPath() == foundSky.GetPath();
		}));
		if(skyCheck){
			return;
		}
		
		const igdeGDSky::Ref sky(igdeGDSky::Ref::New(foundSky));
		
		igdeGDCategory * const autoCategory = pCategories->AutoCategorize(sky->GetPath());
		if(autoCategory){
			sky->SetCategory(autoCategory->GetFullPathString());
		}
		
		AddSky(sky);
	});
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
		if(!path.GetLastComponent().MatchesPattern(pPattern)){
			return true;
		}
		
		const decString fullPath(path.GetPathUnix());
		const bool hasPath = pOwner.GetSkyList().HasMatching([&](const igdeGDSky &s){
			return s.GetPath() == fullPath;
		});
		if(hasPath){
			return true;
		}
		
		decString genName(fullPath.GetReplaced(decPath::PathSeparator(), ' '));
		const int index = genName.FindReverse('.');
		if(index != -1){
			genName = genName.GetLeft(index);
		}
		
		try{
			pSky = igdeGDSky::Ref::New(fullPath, genName);
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
