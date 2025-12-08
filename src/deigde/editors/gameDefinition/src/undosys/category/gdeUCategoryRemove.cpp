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

#include "gdeUCategoryRemove.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/category/gdeCategory.h"

#include <dragengine/common/exceptions.h>



// Class gdeUCategoryRemove
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUCategoryRemove::gdeUCategoryRemove(gdeGameDefinition *gameDefinition,
gdeCategory *category, eCategoryType type) :
gdeUCategoryBase(gameDefinition, type),
pParent(NULL),
pCategory(NULL)
{
	if(!category){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Category remove");
	
	gdeCategory * const parent = category->GetParent();
	if(parent){
		if(!parent->GetCategories().Has(category)){
		   DETHROW(deeInvalidParam);
		}
		
		pParent = parent;
		parent->AddReference();
		
	}else{
		switch(type){
		case ectObjectClass:
			if(!gameDefinition->GetCategoriesObjectClass().Has(category)){
				DETHROW(deeInvalidParam);
			}
			break;
			
		case ectParticleEmitter:
			if(!gameDefinition->GetCategoriesParticleEmitter().Has(category)){
				DETHROW(deeInvalidParam);
			}
			break;
			
		case ectSkin:
			if(!gameDefinition->GetCategoriesSkin().Has(category)){
				DETHROW(deeInvalidParam);
			}
			break;
			
		case ectSky:
			if(!gameDefinition->GetCategoriesSky().Has(category)){
				DETHROW(deeInvalidParam);
			}
			break;
			
			
		default:
			DETHROW(deeInvalidParam);
		}
	}
	
	pCategory = category;
	category->AddReference();
}

gdeUCategoryRemove::~gdeUCategoryRemove(){
}



// Management
///////////////

void gdeUCategoryRemove::Undo(){
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
}

void gdeUCategoryRemove::Redo(){
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
