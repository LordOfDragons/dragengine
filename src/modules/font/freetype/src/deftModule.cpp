/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deftModule.h"
#include "deftFont.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>


// Export definition
//////////////////////

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEFreeTypeCreateModule(deLoadableModule *loadableModule);
#ifdef __cplusplus
}
#endif
#endif


// Entry function
///////////////////

deBaseModule *DEFreeTypeCreateModule(deLoadableModule *loadableModule){
	try{
		return new deftModule(*loadableModule);
		
	}catch(const deException &){
		return nullptr;
	}
}


// Class deftModule
/////////////////////

// Constructor, destructor
////////////////////////////

deftModule::deftModule(deLoadableModule &loadableModule) :
deBaseFontModule(loadableModule),
pFTLib(nullptr)
{
	AssertFT(FT_Init_FreeType(&pFTLib), "FT_Init_FreeType");
}

deftModule::~deftModule(){
	if(pFTLib){
		FT_Done_FreeType(pFTLib);
		pFTLib = nullptr;
	}
}


// Loading and saving
///////////////////////

void deftModule::AssertFT(FT_Error error, const char *function){
	if(error == FT_Err_Ok){
		return;
	}
	
	decString message;
	message.Format("%s failed (0x%x)", function, (int)error);
	DETHROW_INFO(deeInvalidAction, message);
}

void deftModule::LoadFont(decBaseFileReader &file, deFont &font){
	deftFont(*this, file).LoadFont(font);
}

deFontSize::Ref deftModule::LoadFontSize(const decBaseFileReader::Ref &reader,
const deFont::Ref &font, int lineHeight){
	// when using a parallel task the task has to hold reader, font, size and deftFont instance
	return deftFont(*this, reader).LoadFontSize(font, lineHeight);
}

void deftModule::SaveFont(decBaseFileWriter &file, const deFont &font){
	DETHROW_INFO(deeInvalidAction, "Not supported");
}


// Private functions
//////////////////////


#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deftModuleInternal : public deInternalModule{
public:
	typedef deTObjectReference<deftModuleInternal> Ref;
	
	deftModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("FreeType");
		SetDescription("Handles FreeType fonts.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtFont);
		SetDirectoryName("freetype");
		GetPatternList().Add(".ttf");
		GetPatternList().Add(".otf");
		SetDefaultExtension(".ttf");
		SetNoSaving(true);
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(DEFreeTypeCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> deftRegisterInternalModule(deModuleSystem *system){
	return deftModuleInternal::Ref::NewWith(system);
}
#endif
