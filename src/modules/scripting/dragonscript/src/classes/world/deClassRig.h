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
	DEF_NATFUNC( nfBoneGetRotation );
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
	DEF_NATFUNC( nfBoneConstraintGetReferencePosition );
	DEF_NATFUNC( nfBoneConstraintGetReferenceOrientation );
	DEF_NATFUNC( nfBoneConstraintGetBoneOffset );
	DEF_NATFUNC( nfBoneGetShapes );
	DEF_NATFUNC( nfBoneShapeGetProperty );
	DEF_NATFUNC( nfBoneGetParent );
	
	DEF_NATFUNC( nfGetShapes );
	DEF_NATFUNC( nfShapeGetProperty );
	DEF_NATFUNC( nfGetCentralMassPoint );
	DEF_NATFUNC( nfGetRootBone );
	
	DEF_NATFUNC( nfSave );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
