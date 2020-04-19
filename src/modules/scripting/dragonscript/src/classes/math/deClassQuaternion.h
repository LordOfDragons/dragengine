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
#ifndef _DECLASSQUATERNION_H_
#define _DECLASSQUATERNION_H_

// includes
#include <libdscript/libdscript.h>
#include "dragengine/common/math/decMath.h"

// predefinitions
class deEngine;
class deScriptingDragonScript;

// quaternion script class
class deClassQuaternion : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
public:
	// constructor
	deClassQuaternion( deEngine *gameEngine, deScriptingDragonScript *scriptManager );
	~deClassQuaternion();
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	const decQuaternion &GetQuaternion( dsRealObject *This ) const;
	void PushQuaternion( dsRunTime *RT, const decQuaternion &vector );

private:
	struct sInitData{
		dsClass *clsQuat;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsObj;
		dsClass *clsInt;
		dsClass *clsFlt;
		
		dsClass *clsVec;
		dsClass *clsDVec;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNew2 );
	DEF_NATFUNC( nfNew3 );
	DEF_NATFUNC( nfNewFromEuler );
	DEF_NATFUNC( nfNewFromEuler2 );
	DEF_NATFUNC( nfNewFromEulerX );
	DEF_NATFUNC( nfNewFromEulerY );
	DEF_NATFUNC( nfNewFromEulerZ );
	DEF_NATFUNC( nfNewFromAxis );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetX );
	DEF_NATFUNC( nfGetY );
	DEF_NATFUNC( nfGetZ );
	DEF_NATFUNC( nfGetW );
	DEF_NATFUNC( nfGet );
	DEF_NATFUNC( nfGetLength );
	DEF_NATFUNC( nfNormalize );
	DEF_NATFUNC( nfConjugate );
	DEF_NATFUNC( nfAbsolute );
	DEF_NATFUNC( nfGetEulerAngles );
	DEF_NATFUNC( nfSlerp );
	
	DEF_NATFUNC( nfIsEqualTo );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfOpMinus );
	DEF_NATFUNC( nfOpAdd );
	DEF_NATFUNC( nfOpSubtract );
	DEF_NATFUNC( nfOpScale );
	DEF_NATFUNC( nfOpDivide );
	DEF_NATFUNC( nfOpMultiply );
	DEF_NATFUNC( nfOpMultiplyVector );
	DEF_NATFUNC( nfOpMultiplyDVector );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

// end of include only once
#endif
