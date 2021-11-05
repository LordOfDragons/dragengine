/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfDestructor );
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfGetDescription );
	DEF_NATFUNC( nfGetType );
	DEF_NATFUNC( nfGetMinimumValue );
	DEF_NATFUNC( nfGetMaximumValue );
	DEF_NATFUNC( nfGetValueStepSize );
	DEF_NATFUNC( nfGetSelectionEntryCount );
	DEF_NATFUNC( nfGetSelectionEntryValueAt );
	DEF_NATFUNC( nfGetSelectionEntryDisplayNameAt );
	DEF_NATFUNC( nfGetSelectionEntryDescriptionAt );
	DEF_NATFUNC( nfGetCategory );
	DEF_NATFUNC( nfGetDisplayName );
	DEF_NATFUNC( nfGetDefaultValue );
#undef DEF_NATFUNC
};

// end of include only once
#endif
