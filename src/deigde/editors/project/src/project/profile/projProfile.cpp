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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "projProfile.h"
#include "../projProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decUuid.h>



// Class projProfile
//////////////////////

// Constructor, destructor
////////////////////////////

projProfile::projProfile() :
pProject(nullptr),
pName("Profile"),

pScriptDirectory("/scripts"),
pGameObject("MyGameApp"),
pPathConfig("/config"),
pPathCapture("/capture"),

pAliasIdentifier("my_game_app"),

pDelgaPath("projribute/MyGameApp.delga"){
}

projProfile::projProfile(const projProfile &profile) :
pProject(nullptr),
pName(profile.pName),
pDescription(profile.pDescription),

pScriptDirectory(profile.pScriptDirectory),
pGameObject(profile.pGameObject),
pPathConfig(profile.pPathConfig),
pPathCapture(profile.pPathCapture),

pIdentifier(profile.pIdentifier),
pAliasIdentifier(profile.pAliasIdentifier),
pTitle(profile.pTitle),
pGameDescription(profile.pGameDescription),
pCreator(profile.pCreator),
pWebsite(profile.pWebsite),
pWindowSize(profile.pWindowSize),
pIcons(profile.pIcons),

pExcludePatterns(profile.pExcludePatterns),
pRequiredExtensions(profile.pRequiredExtensions),

pDelgaPath(profile.pDelgaPath),

pRunArguments(profile.pRunArguments){
}

projProfile::~projProfile(){
	pCleanUp();
}



// Management
///////////////

void projProfile::SetProject(projProject *project){
	pProject = project;
}



void projProfile::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	NotifyNameChanged();
}

void projProfile::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	NotifyChanged();
}



void projProfile::SetScriptDirectory(const char *directory){
	if(pScriptDirectory == directory){
		return;
	}
	
	pScriptDirectory = directory;
	NotifyChanged();
}

void projProfile::SetGameObject(const char *gameObject){
	if(pGameObject == gameObject){
		return;
	}
	
	pGameObject = gameObject;
	NotifyChanged();
}

void projProfile::SetPathConfig(const char *path){
	if(pPathConfig == path){
		return;
	}
	
	pPathConfig = path;
	NotifyChanged();
}

void projProfile::SetPathCapture(const char *path){
	if(pPathCapture == path){
		return;
	}
	
	pPathCapture = path;
	NotifyChanged();
}



void projProfile::SetIdentifier(const decUuid &identifier){
	if(pIdentifier == identifier){
		return;
	}
	
	pIdentifier = identifier;
	NotifyChanged();
}

void projProfile::SetAliasIdentifier(const char *identifier){
	if(pAliasIdentifier == identifier){
		return;
	}
	
	pAliasIdentifier = identifier;
	NotifyChanged();
}

void projProfile::SetTitle(const char *title){
	if(pTitle == title){
		return;
	}
	
	pTitle = title;
	NotifyChanged();
}

void projProfile::SetGameDescription(const char *description){
	if(pGameDescription == description){
		return;
	}
	
	pGameDescription = description;
	NotifyChanged();
}

void projProfile::SetCreator(const char *creator){
	if(pCreator == creator){
		return;
	}
	
	pCreator = creator;
	NotifyChanged();
}

void projProfile::SetWebsite(const char *website){
	if(pWebsite == website){
		return;
	}
	
	pWebsite = website;
	NotifyChanged();
}

void projProfile::SetWindowSize(const decPoint &size){
	if(size == pWindowSize){
		return;
	}
	
	pWindowSize = size;
	NotifyChanged();
}

void projProfile::SetIcons(const decStringSet &icons){
	if(icons == pIcons){
		return;
	}
	
	pIcons = icons;
	NotifyChanged();
}



void projProfile::SetExcludePatterns(const decStringSet &patterns){
	if(patterns == pExcludePatterns){
		return;
	}
	
	pExcludePatterns = patterns;
	NotifyChanged();
}

void projProfile::SetRequiredExtensions(const decStringSet &extensions){
	if(extensions == pRequiredExtensions){
		return;
	}
	
	pRequiredExtensions = extensions;
	NotifyChanged();
}



void projProfile::SetDelgaPath(const char *path){
	if(pDelgaPath == path){
		return;
	}
	
	pDelgaPath = path;
	NotifyChanged();
}



void projProfile::SetRunArguments(const char *arguments){
	if(pRunArguments == arguments){
		return;
	}
	
	pRunArguments = arguments;
	NotifyChanged();
}



void projProfile::NotifyChanged(){
	if(pProject){
		pProject->NotifyProfileChanged(this);
	}
}

void projProfile::NotifyNameChanged(){
	if(pProject){
		pProject->NotifyProfileNameChanged(this);
	}
}



// Private Functions
//////////////////////

void projProfile::pCleanUp(){
}
