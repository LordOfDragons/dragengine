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
#include <string.h>
#include <stdlib.h>

#include "gdeUCategoryAdd.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/category/gdeCategory.h"

#include <dragengine/common/exceptions.h>



// Class gdeUCategoryAdd
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeUCategoryAdd::gdeUCategoryAdd(gdeGameDefinition *gameDefinition, gdeCategory *parent,
gdeCategory *category, eCategoryType type) :
gdeUCategoryBase(gameDefinition, type),
pParent(NULL),
pCategory(NULL)
{
	if(!category){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Category add");
	
	pParent = parent;
	
	pCategory = category;
	category->AddReference();
}

gdeUCategoryAdd::~gdeUCategoryAdd(){
}



// Management
///////////////

void gdeUCategoryAdd::Undo(){
	gdeGameDefinition &gameDefinition = *GetGameDefinition();
	
	gdeCategory *activeCheck = gameDefinition.GetActiveCategory();
	while(activeCheck){
		if(pCategory == activeCheck){
			switch(gameDefinition.GetSelectedObjectType()){
			case gdeGameDefinition::eotCategoryObjectClass:
			case gdeGameDefinition::eotCategoryParticleEmitter:
			case gdeGameDefinition::eotCategorySkin:
			case gdeGameDefinition::eotCategorySky:
				if(!activeCheck->GetParent()){
					gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotNoSelection);
				}
				break;
				
			default:
				break;
			}
			
			gameDefinition.SetActiveCategory(activeCheck->GetParent());
			break;
		}
		
		activeCheck = activeCheck->GetParent();
	}
	
	if(pParent){
		pParent->RemoveCategory(pCategory);
		
	}else{
		switch(GetType()){
		case ectObjectClass:
			gameDefinition.GetCategoriesObjectClass().Remove(pCategory);
			break;
			
		case ectParticleEmitter:
			gameDefinition.GetCategoriesParticleEmitter().Remove(pCategory);
			break;
			
		case ectSkin:
			gameDefinition.GetCategoriesSkin().Remove(pCategory);
			break;
			
		case ectSky:
			gameDefinition.GetCategoriesSky().Remove(pCategory);
			break;
			
			
		default:
			DETHROW(deeInvalidParam);
		}
	}
	
	Notify();
}

void gdeUCategoryAdd::Redo(){
	gdeGameDefinition &gameDefinition = *GetGameDefinition();
	
	if(pParent){
		pParent->AddCategory(pCategory);
		
	}else{
		switch(GetType()){
		case ectObjectClass:
			gameDefinition.GetCategoriesObjectClass().Add(pCategory);
			break;
			
		case ectParticleEmitter:
			gameDefinition.GetCategoriesParticleEmitter().Add(pCategory);
			break;
			
		case ectSkin:
			gameDefinition.GetCategoriesSkin().Add(pCategory);
			break;
			
		case ectSky:
			gameDefinition.GetCategoriesSky().Add(pCategory);
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
	}
	
	Notify();
	
	gameDefinition.SetActiveCategory(pCategory);
	
	switch(GetType()){
	case ectObjectClass:
		gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategoryObjectClass);
		break;
		
	case ectParticleEmitter:
		gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategoryParticleEmitter);
		break;
		
	case ectSkin:
		gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategorySkin);
		break;
		
	case ectSky:
		gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategorySky);
		break;
		
	default:
		break;
	}
}
