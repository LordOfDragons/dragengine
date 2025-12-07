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

// include only once
#ifndef _DECLASSMODULEPARAMETER_H_
#define _DECLASSMODULEPARAMETER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deBaseModule;
class deScriptingDragonScript;

// module parameter script class
class deClassModuleParameter : public dsClass{
private:
	deEngine *p_gameEngine;
	deScriptingDragonScript *p_scrMgr;
	dsClass *pClsModuleParameterType;
	dsClass *pClsModuleParameterCategory;
	
public:
	// constructor
	deClassModuleParameter(deEngine *GameEngine, deScriptingDragonScript *ScrMgr);
	~deClassModuleParameter();
	// internal functions
	inline deScriptingDragonScript *GetScriptModule() const{ return p_scrMgr; }
	void CreateClassMembers(dsEngine *engine);
	void PushParameter(dsRunTime *RT, deBaseModule *module, int index);
	inline deEngine *GetGameEngine() const{ return p_gameEngine; }
	inline dsClass *GetClassModuleParameterType() const{ return pClsModuleParameterType; }
	inline dsClass *GetClassModuleParameterCategory() const{ return pClsModuleParameterCategory; }

private:
	struct sInitData{
		dsClass *clsMP, *clsPar, *clsVoid;
		dsClass *clsInt, *clsFlt, *clsStr;
		dsClass *clsModuleParameterType;
		dsClass *clsModuleParameterCategory;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfDestructor);
	DEF_NATFUNC(nfGetName);
	DEF_NATFUNC(nfGetDescription);
	DEF_NATFUNC(nfGetType);
	DEF_NATFUNC(nfGetMinimumValue);
	DEF_NATFUNC(nfGetMaximumValue);
	DEF_NATFUNC(nfGetValueStepSize);
	DEF_NATFUNC(nfGetSelectionEntryCount);
	DEF_NATFUNC(nfGetSelectionEntryValueAt);
	DEF_NATFUNC(nfGetSelectionEntryDisplayNameAt);
	DEF_NATFUNC(nfGetSelectionEntryDescriptionAt);
	DEF_NATFUNC(nfGetCategory);
	DEF_NATFUNC(nfGetDisplayName);
	DEF_NATFUNC(nfGetDefaultValue);
#undef DEF_NATFUNC
};

// end of include only once
#endif
