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

#ifndef _DECLASSTEXMATRIX_H_
#define _DECLASSTEXMATRIX_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deScriptingDragonScript;



/**
 * @brief Texture Matrix Script Class.
 */
class deClassTexMatrix : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassTexMatrix( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassTexMatrix();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the texture matrix from an object. */
	const decTexMatrix &GetTexMatrix( dsRealObject *myself ) const;
	/** Pushes a texture matrix. */
	void PushTexMatrix( dsRunTime *rt, const decTexMatrix &matrix );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsTexMat, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsVec2, *clsFileReader, *clsFileWriter;
		dsClass *clsTexMatrix2;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNew2 );
	DEF_NATFUNC( nfNewTranslation );
	DEF_NATFUNC( nfNewTranslation2 );
	DEF_NATFUNC( nfNewScaling );
	DEF_NATFUNC( nfNewScaling2 );
	DEF_NATFUNC( nfNewRotation );
	DEF_NATFUNC( nfNewShear );
	DEF_NATFUNC( nfNewShear2 );
	DEF_NATFUNC( nfNewST );
	DEF_NATFUNC( nfNewST2 );
	DEF_NATFUNC( nfNewSRT );
	DEF_NATFUNC( nfNewSRT2 );
	DEF_NATFUNC(nfNewRT);
	DEF_NATFUNC(nfNewRT2);
	DEF_NATFUNC( nfNewCenterSRT );
	DEF_NATFUNC( nfNewCenterSRT2 );
	DEF_NATFUNC( nfNewCenterRotation );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfGet11 );
	DEF_NATFUNC( nfGet12 );
	DEF_NATFUNC( nfGet13 );
	DEF_NATFUNC( nfGet21 );
	DEF_NATFUNC( nfGet22 );
	DEF_NATFUNC( nfGet23 );
	DEF_NATFUNC( nfGet31 );
	DEF_NATFUNC( nfGet32 );
	DEF_NATFUNC( nfGet33 );
	DEF_NATFUNC(nfTransformNormal);
	DEF_NATFUNC(nfGetRightVector);
	DEF_NATFUNC(nfGetUpVector);
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfGetRotationAngle);
	DEF_NATFUNC(nfGetScaling);
	DEF_NATFUNC(nfGetInverse);
	DEF_NATFUNC(nfGetRotation);
	DEF_NATFUNC(nfNormalize);
	DEF_NATFUNC(nfToTexMatrix2);
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfOpAdd );
	DEF_NATFUNC( nfOpSubtract );
	DEF_NATFUNC( nfOpScale );
	DEF_NATFUNC( nfOpDivide );
	DEF_NATFUNC( nfOpMultiply );
	DEF_NATFUNC( nfOpMultiply2 );
	DEF_NATFUNC( nfOpMultiply3 );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfToString );
	DEF_NATFUNC( nfToStringPrecision );
#undef DEF_NATFUNC
};

#endif
