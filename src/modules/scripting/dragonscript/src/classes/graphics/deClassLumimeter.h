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
#ifndef _DECLASSLUMIMETER_H_
#define _DECLASSLUMIMETER_H_

// includes
#include <libdscript/libdscript.h>



// predefinitions
class deEngine;
class deLumimeter;
class deClassVector;
class deClassDVector;
class deClassColor;
class deScriptingDragonScript;



// lumimeter script class
class deClassLumimeter : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassVector *pClsVec;
	deClassDVector *pClsDVec;
	deClassColor *pClsClr;
	
public:
	// constructor
	deClassLumimeter( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	virtual ~deClassLumimeter();
	
	// internal functions
	virtual void CreateClassMembers( dsEngine *engine );
	
	deLumimeter *GetLumimeter( dsRealObject *myself ) const;
	void PushLumimeter( dsRunTime *rt, deLumimeter *lumimeter );
	
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassColor *GetClassColor() const{ return pClsClr; }
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }

private:
	struct sInitData{
		dsClass *clsLM, *clsVoid, *clsInt, *clsFlt, *clsBool, *clsObj;
		dsClass *clsVec, *clsDVec, *clsClr;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetDirection );
	DEF_NATFUNC( nfSetDirection );
	DEF_NATFUNC( nfGetConeInnerAngle );
	DEF_NATFUNC( nfSetConeInnerAngle );
	DEF_NATFUNC( nfGetConeOuterAngle );
	DEF_NATFUNC( nfSetConeOuterAngle );
	DEF_NATFUNC( nfGetConeExponent );
	DEF_NATFUNC( nfSetConeExponent );
	
	DEF_NATFUNC( nfMeasureLuminance );
	DEF_NATFUNC( nfMeasureColor );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
