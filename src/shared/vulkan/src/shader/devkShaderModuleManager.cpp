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

#include "devkShaderModuleManager.h"
#include "devkShaderModule.h"
#include "../devkDevice.h"
#include "../deSharedVulkan.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class devkShaderModuleManager
//////////////////////////////////

// Constructor, destructor
////////////////////////////

devkShaderModuleManager::devkShaderModuleManager(devkDevice &device) :
pDevice(device){
}

devkShaderModuleManager::~devkShaderModuleManager(){
}



// Management
///////////////

int devkShaderModuleManager::GetCount() const{
	return pSources.GetCount();
}

devkShaderModule *devkShaderModuleManager::GetAt(int index) const{
	return (devkShaderModule*)pSources.GetAt(index);
}

devkShaderModule *devkShaderModuleManager::GetWithPath(const char *path) const{
	const int count = pSources.GetCount();
	int i;
	for(i=0; i<count; i++){
		devkShaderModule * const module = (devkShaderModule*)pSources.GetAt(i);
		if(module->GetPath() == path){
			return module;
		}
	}
	
	return nullptr;
}

bool devkShaderModuleManager::HasWithPath(const char *path) const{
	const int count = pSources.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(((devkShaderModule*)pSources.GetAt(i))->GetPath() == path){
			return true;
		}
	}
	
	return false;
}

void devkShaderModuleManager::Add(devkShaderModule *module){
	if(!module){
		DETHROW_INFO(deeNullPointer, "module");
	}
	if(HasWithPath(module->GetPath())){
		DETHROW_INFO(deeInvalidParam, "module with path is present");
	}
	
	pSources.Add(module);
}

void devkShaderModuleManager::Clear(){
	pSources.RemoveAll();
}

void devkShaderModuleManager::Load(const char *directory){
	deBaseModule &baseModule = pDevice.GetInstance().GetVulkan().GetModule();
	deVirtualFileSystem &vfs = baseModule.GetVFS();
	const int oldSourceCount = pSources.GetCount();
	decBaseFileReader::Ref reader;
	devkShaderModule::Ref module;
	
	const decPath basePath(decPath::CreatePathUnix(directory));
	const int basePathLen = (int)strlen(basePath.GetPathUnix()) + 1;
	
	deCollectFileSearchVisitor collect("*.spv", true);
	vfs.SearchFiles(basePath, collect);
	
	const dePathList &pathList = collect.GetFiles();
	const int count = pathList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const decPath &path = pathList.GetAt(i);
		const decString filename(path.GetPathUnix().GetMiddle(basePathLen));
		
		try{
			reader.TakeOver(vfs.OpenFileForReading(path));
			module.TakeOverWith(pDevice, filename, reader);
			Add(module);
			
		}catch(const deException &){
			baseModule.LogInfoFormat("Vulkan: Loading shader module failed: %s", filename.GetString());
			throw;
		}
	}
	
	baseModule.LogInfoFormat("Vulkan: Loaded %d shader modules.", pSources.GetCount() - oldSourceCount);
}
