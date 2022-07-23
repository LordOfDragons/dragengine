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
#ifndef _DECLASSGRAPHICSYSTEM_H_
#define _DECLASSGRAPHICSYSTEM_H_

// includes
#include <libdscript/libdscript.h>
#include "dragengine/common/math/decMath.h"

// predefinitions
class deEngine;
class deScriptingDragonScript;


// graphics script class
class deClassGraphicSystem : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	// constructor
	deClassGraphicSystem(deScriptingDragonScript &ds);
	~deClassGraphicSystem();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	inline deScriptingDragonScript &GetDS() const{ return pDS; }

private:
	struct sInitData{
		dsClass *clsGraSys, *clsVoid, *clsInt, *clsFlt;
		dsClass *clsBool, *clsStr, *clsModPar, *clsCView, *clsPoint;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfGetWindowWidth );
	DEF_NATFUNC( nfGetWindowHeight );
	DEF_NATFUNC( nfGetWindowSize );
	DEF_NATFUNC( nfSetWindowGeometry );
	DEF_NATFUNC( nfSetWindowTitle );
	DEF_NATFUNC( nfGetPrimaryCanvas );
	DEF_NATFUNC(nfGetParameterCount);
	DEF_NATFUNC(nfGetParameterInfo);
	DEF_NATFUNC(nfGetParameterInfo2);
	DEF_NATFUNC(nfGetParameterValue);
	DEF_NATFUNC(nfSetParameterValue);
	DEF_NATFUNC( nfSendCommand );
	DEF_NATFUNC( nfGetFPSRate );
#undef DEF_NATFUNC
};

// end of include only once
#endif
