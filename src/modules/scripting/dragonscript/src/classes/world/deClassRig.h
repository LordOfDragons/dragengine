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

#ifndef _DECLASSRIG_H_
#define _DECLASSRIG_H_

#include <libdscript/libdscript.h>

class deEngine;
class deRig;
class deScriptingDragonScript;



/**
 * \brief Rig script class.
 */
class deClassRig : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassRig( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassRig();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the rig from an object. Returns NULL if myself is NULL. */
	deRig *GetRig( dsRealObject *myself ) const;
	/** \brief Pushes a rig. Null is pushed as NULL rig object. */
	void PushRig( dsRunTime *rt, deRig *rig );
	/*@}*/

private:
	struct sInitData{
		dsClass *clsRig, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsFlt;
		dsClass *clsRN, *clsVec, *clsQuat, *clsCCon, *clsShapeList, *clsMat;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	
	DEF_NATFUNC( nfGetBoneCount );
	DEF_NATFUNC( nfIndexOfBoneNamed );
	DEF_NATFUNC( nfBoneGetName );
	DEF_NATFUNC( nfBoneGetPosition );
	DEF_NATFUNC( nfBoneGetOrientation );
	DEF_NATFUNC( nfBoneGetMatrix );
	DEF_NATFUNC( nfBoneGetInverseMatrix );
	DEF_NATFUNC( nfBoneGetCentralMassPoint );
	DEF_NATFUNC( nfBoneGetMass );
	DEF_NATFUNC( nfBoneGetDynamic );
	DEF_NATFUNC( nfBoneGetIKLimitsLower );
	DEF_NATFUNC( nfBoneGetIKLimitsUpper );
	DEF_NATFUNC( nfBoneGetIKResistance );
	DEF_NATFUNC( nfBoneGetIKLocked );
	DEF_NATFUNC( nfBoneGetConstraintCount );
	DEF_NATFUNC( nfBoneGetConstraintAt );
	DEF_NATFUNC( nfBoneGetShapes );
	DEF_NATFUNC( nfBoneShapeGetProperty );
	
	DEF_NATFUNC( nfGetShapes );
	DEF_NATFUNC( nfShapeGetProperty );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
