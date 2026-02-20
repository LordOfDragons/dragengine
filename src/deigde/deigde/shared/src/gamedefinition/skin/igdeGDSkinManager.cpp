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

#include "igdeGDSkin.h"
#include "igdeGDSkinManager.h"
#include "../igdeGDCategory.h"
#include "../visitor/igdeGDVisitor.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>

#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeGDSkinManager
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkinManager::igdeGDSkinManager() :
pCategories(igdeGDCategory::Ref::New("Skins")){
}

igdeGDSkinManager::~igdeGDSkinManager(){
}



// Management
///////////////

void igdeGDSkinManager::AddSkin(igdeGDSkin *skin){
	if(!skin || pSkins.HasWithPathOrName(skin->GetPath(), skin->GetName())){
		DETHROW(deeInvalidParam);
	}
	pSkins.Add(skin);
}

void igdeGDSkinManager::RemoveSkin(igdeGDSkin *skin){
	pSkins.Remove(skin);
}

void igdeGDSkinManager::RemoveAllSkins(){
	pSkins.RemoveAll();
}



void igdeGDSkinManager::SetDefaultSkinPath(const char *defaultSkinPath){
	pDefaultSkinPath = defaultSkinPath;
}



void igdeGDSkinManager::VisitSkinsMatchingCategory(igdeGDVisitor &visitor, const igdeGDCategory *category) const{
	decPath pathCat;
	if(category){
		category->GetFullPath(pathCat);
	}
	const decString strPathCat(pathCat.GetPathUnix());
	
	pSkins.Visit([&](igdeGDSkin *skin){
		if(skin->GetCategory() == strPathCat){
			visitor.VisitSkin(skin);
		}
	});
}

void igdeGDSkinManager::VisitMatchingFilter(igdeGDVisitor &visitor, const decString &filter) const{
	if(filter.GetLength() < 2){
		return;
	}
	
	const decString realFilter(filter.GetLower());
	pSkins.Visit([&](igdeGDSkin *skin){
		if(skin->GetName().GetLower().FindString(realFilter) != -1
		|| skin->GetPath().GetLower().FindString(realFilter) != -1){
			visitor.VisitSkin(skin);
		}
	});
}



void igdeGDSkinManager::UpdateWith(const igdeGDSkinManager &skinManager){
	skinManager.GetSkins().Visit([&](const igdeGDSkin &otherSkin){
		igdeGDSkin *skinCheck = pSkins.FindWithPath(otherSkin.GetPath());
		if(!skinCheck){
			skinCheck = pSkins.FindNamed(otherSkin.GetName());
		}
		
		const igdeGDSkin::Ref skin(igdeGDSkin::Ref::New(otherSkin));
		if(skinCheck){
			RemoveSkin(skinCheck);
		}
		AddSkin(skin);
	});
	
	pCategories->UpdateWith(skinManager.pCategories);
	pAutoFindPath = skinManager.pAutoFindPath;
	
	if(!skinManager.pDefaultSkinPath.IsEmpty()){
		pDefaultSkinPath = skinManager.pDefaultSkinPath;
	}
}

void igdeGDSkinManager::UpdateWithFound(const igdeGDSkinManager &skinManager){
	skinManager.GetSkins().Visit([&](const igdeGDSkin &foundSkin){
		if(pSkins.FindWithPath(foundSkin.GetPath())){
			return;
		}
		
		const igdeGDSkin::Ref skin(igdeGDSkin::Ref::New(foundSkin));
		
		igdeGDCategory * const autoCategory = pCategories->AutoCategorize(skin->GetPath());
		if(autoCategory){
			skin->SetCategory(autoCategory->GetFullPathString());
		}
		
		AddSkin(skin);
	});
}

class igdeGDSkinManagerFind : public deFileSearchVisitor{
private:
	igdeGDSkinManager &pOwner;
	const char * const pPattern;
	igdeGDSkin::Ref pSkin;
	
public:
	igdeGDSkinManagerFind(igdeGDSkinManager &owner, const char *pattern) :
	pOwner(owner), pPattern(pattern){
	}
	
	virtual bool VisitFile(const deVirtualFileSystem &, const decPath &path){
		if(!path.GetLastComponent().MatchesPattern(pPattern)){
			return true;
		}
		
		const decString fullPath(path.GetPathUnix());
		if(pOwner.GetSkins().HasWithPath(fullPath)){
			return true;
		}
		
		decString genName(fullPath.GetReplaced(decPath::PathSeparator(), ' '));
		const int index = genName.FindReverse('.');
		if(index != -1){
			genName = genName.GetLeft(index);
		}
		
		try{
			pSkin = igdeGDSkin::Ref::New(fullPath, genName);
			pSkin->SetDescription("Auto-Imported");
			pOwner.AddSkin(pSkin);
			
		}catch(const deException &){
		}
		
		return true;
	}
	
	virtual bool VisitDirectory(const deVirtualFileSystem &vfs, const decPath &path){
		vfs.SearchFiles(path, *this);
		return true;
	}
};

void igdeGDSkinManager::FindAndAddSkins(deVirtualFileSystem &vfs, const decPath &directory,
const char *pattern, bool recursive){
	igdeGDSkinManagerFind process(*this, pattern);
	vfs.SearchFiles(directory, process);
}
