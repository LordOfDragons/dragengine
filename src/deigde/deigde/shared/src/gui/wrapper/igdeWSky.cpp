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

#include "igdeWSky.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/sky/igdeGDSky.h"
#include "../../gamedefinition/sky/igdeGDSkyController.h"
#include "../../gamedefinition/sky/igdeGDSkyManager.h"
#include "../../gameproject/igdeGameProject.h"
#include "../../loadsave/igdeLoadSky.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyInstanceManager.h>
#include <dragengine/resources/world/deWorld.h>


// Meta Context
/////////////////

// Class igdeWSky::MetaContext
////////////////////////////////

igdeWSky::MetaContext::MetaContext(igdeWSky *wrapper) :
igdeMetaContext("igde.wsky"),
pWrapper(wrapper){
	SetProperties(MetaProperties::global.properties);
}

igdeWSky::MetaContext::~MetaContext() = default;

igdeWSky &igdeWSky::MetaContext::GetWrapperRef() const{
	DEASSERT_NOTNULL(pWrapper)
	return *pWrapper;
}

igdeWSky::MetaContext::Ref igdeWSky::MetaContext::Capture() const{
	auto context = Ref::New(pWrapper);
	context->pGuard = pWrapper;
	return context;
}

igdeEnvironment &igdeWSky::MetaContext::GetEnvironment() const{
	return GetWrapperRef().GetEnvironment();
}

igdeUndoSystem *igdeWSky::MetaContext::GetUndoSystem() const{
	return GetWrapperRef().GetUndoSystem();
}

igdeClipboard *igdeWSky::MetaContext::GetClipboard() const{
	return &GetEnvironment().GetClipboard();
}


// Meta Properties
////////////////////

// igdeWSky::MetaProperties::Path
igdeWSky::MetaProperties::Path::Path() :
TBase("igde.wsky.path", "Igde.WPSky.SkyPath", igdeEnvironment::eFilePatternListTypes::efpltSky){
}

igdeWSky::MetaProperties::Path::~Path() = default;

igdeMetaPropertyPathStorage::Storage &igdeWSky::MetaProperties::Path::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Wrapper(context).GetMPPath();
}


// Properties

igdeWSky::MetaProperties igdeWSky::MetaProperties::global;

igdeWSky::MetaProperties::MetaProperties() :
path(deTObjectReference<Path>::New()),
properties(igdeMetaContext::PropertyList::Ref::New(decTObjectOrderedSet<igdeMetaProperty>(devctag,
	path))){
}


// Asynchronous load finished listener
////////////////////////////////////////

igdeWSky::cAsyncLoadFinished::cAsyncLoadFinished() = default;
igdeWSky::cAsyncLoadFinished::~cAsyncLoadFinished() = default;


// Class igdeWSky
///////////////////

// Constructor, destructor
////////////////////////////

igdeWSky::igdeWSky(igdeEnvironment &environment) :
pEnvironment(environment),
pUndoSystem(nullptr),
pMetaContext(MetaContext::Ref::New(this)),
pAsyncLoadFinished(nullptr),
pAsyncLoadCounter(0),
pMPPath(MetaProperties::global.path, pMetaContext)
{
	pEngSkyInstance = environment.GetEngineController()->GetEngine()->
		GetSkyInstanceManager()->CreateSkyInstance();
	
	pMPPath.onValueChanged = [this](){
		if(pMPPath->IsEmpty()){
			pSetGDSky(nullptr);
			pSetSky(nullptr);
			return;
		}
		
		auto &skies = pEnvironment.GetGameProject()->GetGameDefinition()->GetSkyManager()->GetSkies();
		auto gdsky = skies.FindWithPath(pMPPath);
		if(!gdsky){
			gdsky = skies.FindNamed(pMPPath);
		}
		
		if(gdsky){
			pSetGDSky(gdsky);
			
		}else{
			pLoadSky(pMPPath);
		}
		
		onChanged();
	};
}

igdeWSky::~igdeWSky(){
	SetWorld(nullptr);
	
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}



// Management
///////////////

void igdeWSky::SetUndoSystem(igdeUndoSystem *undoSystem){
	pUndoSystem = undoSystem;
}

void igdeWSky::SetWorld(deWorld *world){
	if(world == pEngWorld){
		return;
	}
	
	if(pEngWorld && pEngSkyInstance){
		pEngWorld->RemoveSky(pEngSkyInstance);
	}
	
	pEngWorld = world;
	
	if(world){
		world->AddSky(pEngSkyInstance);
	}
}

int igdeWSky::GetControllerCount() const{
	return pEngSkyInstance->GetControllers().GetCount();
}

const deSkyController &igdeWSky::GetControllerAt(int index) const{
	return pEngSkyInstance->GetControllers().GetAt(index);
}

void igdeWSky::SetControllerValue(int index, float value){
	pEngSkyInstance->GetControllers().GetAt(index)->SetCurrentValue(value);
	pEngSkyInstance->NotifyControllerChangedAt(index);
}

const deSky *igdeWSky::GetSky() const{
	return pEngSkyInstance->GetSky();
}

void igdeWSky::SetSky(deSky *sky){
	pMPPath = "";
	pSetGDSky(nullptr);
	pSetSky(sky);
}

void igdeWSky::SetGDDefaultSky(){
	SetPath(pEnvironment.GetGameProject()->GetGameDefinition()->GetSkyManager()->GetDefaultPath());
}

void igdeWSky::SetGDSky(igdeGDSky *gdsky){
	if(pGDSky == gdsky){
		return;
	}
	
	if(gdsky){
		pMPPath = gdsky->GetPath();
		pSetGDSky(gdsky);
		
	}else{
		SetSky(nullptr);
	}
}

void igdeWSky::SetPath(const char *value){
	pMPPath = value;
}

void igdeWSky::SetAsyncLoadFinished(cAsyncLoadFinished *listener){
	pAsyncLoadFinished = listener;
}



void igdeWSky::OnGameDefinitionChanged(){
	if(!pGDSky){
		return; // custom sky or loaded from file
	}
	
	decStringList names;
	if(pEngSkyInstance){
		pEngSkyInstance->GetControllers().Visit([&](const deSkyController &controller){
			names.Add(controller.GetName());
		});
	}
	
	decTList<float> values;
	if(pEngSkyInstance){
		values.EnlargeCapacity(pEngSkyInstance->GetControllers().GetCount());
		pEngSkyInstance->GetControllers().Visit([&](const deSkyController &controller){
			values.Add(controller.GetCurrentValue());
		});
	}
	
	SetPath(pGDSky->GetPath());
	
	if(pEngSkyInstance){
		names.VisitIndexed([&](int i, const decString &name){
			const int index = names.IndexOf(name);
			if(index != -1){
				pEngSkyInstance->GetControllers().GetAt(index)->SetCurrentValue(values[i]);
				pEngSkyInstance->NotifyControllerChangedAt(index);
			}
		});
	}
}



// Private Functions
/////////////////////

void igdeWSky::pSetSky(deSky *sky){
	if(pEngSkyInstance->GetSky() == sky){
		return;
	}
	
	pEngSkyInstance->SetSky(sky);
	
	pMaxLightIntensity = 0.0f;
	if(sky){
		sky->GetLayers().Visit([&](const deSkyLayer &layer){
			pMaxLightIntensity += layer.GetLightIntensity() + layer.GetAmbientIntensity();
		});
	}
}

void igdeWSky::pSetGDSky(igdeGDSky *gdsky){
	if(pGDSky == gdsky){
		return;
	}
	pGDSky = gdsky;
	
	if(gdsky){
		pLoadSky(gdsky->GetPath());
		
	}else{
		pSetSky(nullptr);
	}
}

void igdeWSky::pLoadSky(const char *value){
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	
	pAsyncLoadCounter = 0;
	
	const auto vfsPath = decPath::CreatePathUnix(value);
	if(!engine.GetVirtualFileSystem()->ExistsFile(vfsPath)){
		pCheckAsyncLoadFinished();
		return;
	}
	
	try{
		auto sky = engine.GetSkyManager()->CreateSky();
		igdeLoadSky(pEnvironment, pEnvironment.GetLogger(), "igdeWSky").Load(
			value, sky, engine.GetVirtualFileSystem()->OpenFileForReading(vfsPath));
		pSetSky(sky);
		
	}catch(const deException &){
		pCheckAsyncLoadFinished();
		throw;
	}
	
	pCheckAsyncLoadFinished();
}

void igdeWSky::pCheckAsyncLoadFinished(){
	if(pAsyncLoadCounter != 0){
		return;
	}
	
	if(pAsyncLoadFinished){
		pAsyncLoadFinished->LoadFinished(*this, true);
	}
}
