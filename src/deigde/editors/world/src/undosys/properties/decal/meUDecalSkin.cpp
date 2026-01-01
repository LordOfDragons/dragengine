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

#include "meUDecalSkin.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalSkin
///////////////////////

// Constructor, destructor
////////////////////////////

meUDecalSkin::meUDecalSkin(meDecal *decal, const char *newskin){
	DEASSERT_NOTNULL(decal)
	DEASSERT_NOTNULL(decal->GetWorld())
	DEASSERT_NOTNULL(newskin)
	
	SetShortInfo("Set Decal Skin");
	
	const cDecal::Ref udata(cDecal::Ref::New());
	udata->decal = decal;
	udata->oldskin = decal->GetSkinPath();
	udata->newskin = newskin;
	pDecals.Add(udata);
}

meUDecalSkin::meUDecalSkin(meDecal::List &decals, const char *newskin){
	DEASSERT_NOTNULL(newskin)
	DEASSERT_TRUE(decals.IsNotEmpty())
	
	SetShortInfo("Set Decal Skins");
	
	decals.Visit([&](meDecal *decal){
		const cDecal::Ref udata(cDecal::Ref::New());
		udata->decal = decal;
		udata->oldskin = decal->GetSkinPath();
		udata->newskin = newskin;
		pDecals.Add(udata);
	});
}

meUDecalSkin::~meUDecalSkin(){
}



// Management
///////////////

void meUDecalSkin::Undo(){
	pDecals.Visit([](const cDecal &data){
		data.decal->SetSkinPath(data.oldskin);
	});
	
	pDecals.First()->decal->GetWorld()->GetGuiParameters().SetElementMode(meWorldGuiParameters::eemDecal);
}

void meUDecalSkin::Redo(){
	pDecals.Visit([&](const cDecal &data){
		data.decal->SetSkinPath(data.newskin);
	});
	
	pDecals.First()->decal->GetWorld()->GetGuiParameters().SetElementMode(meWorldGuiParameters::eemDecal);
}
