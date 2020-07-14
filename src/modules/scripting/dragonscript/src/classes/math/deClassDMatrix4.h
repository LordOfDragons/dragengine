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

#ifndef _DECLASSDMATRIX4_H_
#define _DECLASSDMATRIX4_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deEngine;
class deScriptingDragonScript;



/**
 * @brief 4x4 Double Precision Matrix Script Class.
 */
class deClassDMatrix4 : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** Creates a new class. */
	deClassDMatrix4( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassDMatrix4();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the matrix from an object. */
	const decDMatrix &GetDMatrix4( dsRealObject *myself ) const;
	
	/** Pushes a matrix. */
	void PushDMatrix4( dsRunTime *rt, const decDMatrix &matrix );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsDMatrix4;
		
		dsClass *clsBool;
		dsClass *clsFlt;
		dsClass *clsInt;
		dsClass *clsObj;
		dsClass *clsStr;
		dsClass *clsVoid;
		
		dsClass *clsDMatrix;
		dsClass *clsDVec;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
		dsClass *clsMatrix;
		dsClass *clsMatrix4;
		dsClass *clsQuat;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewValues );
	DEF_NATFUNC( nfNewTranslation );
	DEF_NATFUNC( nfNewTranslation2 );
	DEF_NATFUNC( nfNewScaling );
	DEF_NATFUNC( nfNewScaling2 );
	DEF_NATFUNC( nfNewRotationX );
	DEF_NATFUNC( nfNewRotationY );
	DEF_NATFUNC( nfNewRotationZ );
	DEF_NATFUNC( nfNewRotation );
	DEF_NATFUNC( nfNewRotation2 );
	DEF_NATFUNC( nfNewRotationAxis );
	DEF_NATFUNC( nfNewSRT );
	DEF_NATFUNC( nfNewRT );
	DEF_NATFUNC( nfNewSVUT );
	DEF_NATFUNC( nfNewVU );
	DEF_NATFUNC( nfNewCamera );
	DEF_NATFUNC( nfNewWorld );
	DEF_NATFUNC( nfNewWorld2 );
	DEF_NATFUNC( nfNewFromQuaternion );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfGet11 );
	DEF_NATFUNC( nfGet12 );
	DEF_NATFUNC( nfGet13 );
	DEF_NATFUNC( nfGet14 );
	DEF_NATFUNC( nfGet21 );
	DEF_NATFUNC( nfGet22 );
	DEF_NATFUNC( nfGet23 );
	DEF_NATFUNC( nfGet24 );
	DEF_NATFUNC( nfGet31 );
	DEF_NATFUNC( nfGet32 );
	DEF_NATFUNC( nfGet33 );
	DEF_NATFUNC( nfGet34 );
	DEF_NATFUNC( nfGetViewVector );
	DEF_NATFUNC( nfGetUpVector );
	DEF_NATFUNC( nfGetRightVector );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfTransformNormal );
	DEF_NATFUNC( nfGetEulerAngles );
	DEF_NATFUNC( nfGetInverse );
	DEF_NATFUNC( nfNormalize );
	DEF_NATFUNC( nfToQuaternion );
	DEF_NATFUNC( nfToDMatrix );
	DEF_NATFUNC( nfToMatrix4 );
	DEF_NATFUNC( nfToMatrix );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfOpAdd );
	DEF_NATFUNC( nfOpSubtract );
	DEF_NATFUNC( nfOpScale );
	DEF_NATFUNC( nfOpDivide );
	DEF_NATFUNC( nfOpMultiply );
	DEF_NATFUNC( nfOpMultiply2 );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

#endif
