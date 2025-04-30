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

#ifndef _DECLASSMATRIX_H_
#define _DECLASSMATRIX_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deEngine;
class deScriptingDragonScript;



/**
 * @brief 4x3 Matrix Script Class.
 */
class deClassMatrix : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** Creates a new class. */
	deClassMatrix( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassMatrix();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the matrix from an object. */
	const decMatrix &GetMatrix( dsRealObject *myself ) const;
	/** Pushes a matrix. */
	void PushMatrix( dsRunTime *rt, const decMatrix &matrix );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsMatrix;
		
		dsClass *clsBool;
		dsClass *clsFlt;
		dsClass *clsInt;
		dsClass *clsObj;
		dsClass *clsStr;
		dsClass *clsVoid;
		
		dsClass *clsDMatrix;
		dsClass *clsDMatrix4;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
		dsClass *clsMatrix4;
		dsClass *clsQuat;
		dsClass *clsVec;
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
	DEF_NATFUNC(nfGetScaling);
	DEF_NATFUNC( nfGetInverse );
	DEF_NATFUNC( nfGetRotation );
	DEF_NATFUNC( nfNormalize );
	DEF_NATFUNC( nfToQuaternion );
	DEF_NATFUNC( nfToDMatrix4 );
	DEF_NATFUNC( nfToDMatrix );
	DEF_NATFUNC( nfToMatrix4 );
	
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
	DEF_NATFUNC( nfToStringPrecision );
#undef DEF_NATFUNC
};

#endif
