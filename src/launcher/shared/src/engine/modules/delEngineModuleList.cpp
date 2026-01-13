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

#include "delEngineModuleList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>


// Class delEngineModuleList
///////////////////////////////

// Constructor
////////////////

delEngineModuleList::delEngineModuleList(decTObjectOrderedSet<delEngineModule> &&list) :
decTObjectOrderedSet<delEngineModule>(std::move(list)){
}


// Management
///////////////

delEngineModule *delEngineModuleList::GetNamed(const char *name) const{
	if(!name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	
	delEngineModule *latestModule = nullptr;
	Visit([&](delEngineModule *m){
		if(m->GetName() != name){
			return;
		}
		if(latestModule && deModuleSystem::CompareVersion(m->GetVersion(), latestModule->GetVersion()) <= 0){
			return;
		}
		
		latestModule = m;
	});
	
	return latestModule;
}

delEngineModule *delEngineModuleList::GetNamed(const char *name, const char *version) const{
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(version)
	
	return FindOrDefault([&](const delEngineModule &module){
		return module.GetName() == name && module.GetVersion() == version;
	});
}

delEngineModule *delEngineModuleList::GetNamedAtLeast(const char *name, const char *version) const{
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(version)
	
	delEngineModule *latestModule = nullptr;
	Visit([&](delEngineModule *m){
		if(m->GetName() != name || deModuleSystem::CompareVersion(m->GetVersion(), version) < 0){
			return;
		}
		if(latestModule && deModuleSystem::CompareVersion(m->GetVersion(), latestModule->GetVersion()) <= 0){
			return;
		}
		
		latestModule = m;
	});
	
	return latestModule;
}

delEngineModuleList delEngineModuleList::CollectNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return Collect([&](const delEngineModule &m){
		return m.GetName() == name;
	});
}

decStringSet delEngineModuleList::GetNames() const{
	decStringSet names;
	Visit([&](const delEngineModule &m){
		names.Add(m.GetName());
	});
	return names;
}

bool delEngineModuleList::HasNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	
	return HasMatching([&](const delEngineModule &m){
		return m.GetName() == name;
	});
}

bool delEngineModuleList::HasWith(const char *name, const char *version) const{
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(version)
	
	return HasMatching([&](const delEngineModule &m){
		return m.GetName() == name && m.GetVersion() == version;
	});
}

int delEngineModuleList::IndexOfNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	
	const delEngineModule *latestModule = nullptr;
	int latestIndex = -1;
	
	VisitIndexed([&](int i, delEngineModule *m){
		if(m->GetName() != name){
			return;
		}
		
		if(!latestModule || deModuleSystem::CompareVersion(m->GetVersion(), latestModule->GetVersion()) > 0){
			latestModule = m;
			latestIndex = i;
		}
	});
	
	return latestIndex;
}

int delEngineModuleList::IndexOfWith(const char *name, const char *version) const{
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(version)
	
	return IndexOfMatching([&](const delEngineModule &m){
		return m.GetName() == name && m.GetVersion() == version;
	});
}
