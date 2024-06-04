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

#ifndef _DECLASSCOLORMATRIX_H_
#define _DECLASSCOLORMATRIX_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deScriptingDragonScript;



/**
 * \brief Color matrix script class.
 */
class deClassColorMatrix : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassColorMatrix( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassColorMatrix();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Color matrix from an object. */
	const decColorMatrix &GetColorMatrix( dsRealObject *myself ) const;
	
	/** \brief Pushe color matrix. */
	void PushColorMatrix( dsRunTime *rt, const decColorMatrix &matrix );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsClrMat, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsClr, *clsFileReader, *clsFileWriter;
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
	DEF_NATFUNC( nfNewTranslation );
	DEF_NATFUNC( nfNewTranslation2 );
	DEF_NATFUNC( nfNewScaling );
	DEF_NATFUNC( nfNewScaling2 );
	DEF_NATFUNC( nfNewST );
	DEF_NATFUNC( nfNewBrightness );
	DEF_NATFUNC( nfNewBrightness2 );
	DEF_NATFUNC( nfNewBrightness3 );
	DEF_NATFUNC( nfNewContrast );
	DEF_NATFUNC( nfNewContrast2 );
	DEF_NATFUNC( nfNewContrast3 );
	DEF_NATFUNC( nfNewSaturation );
	DEF_NATFUNC( nfNewSaturation2 );
	DEF_NATFUNC( nfNewSaturation3 );
	DEF_NATFUNC( nfNewColorInversion );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfGet11 );
	DEF_NATFUNC( nfGet12 );
	DEF_NATFUNC( nfGet13 );
	DEF_NATFUNC( nfGet14 );
	DEF_NATFUNC( nfGet15 );
	DEF_NATFUNC( nfGet21 );
	DEF_NATFUNC( nfGet22 );
	DEF_NATFUNC( nfGet23 );
	DEF_NATFUNC( nfGet24 );
	DEF_NATFUNC( nfGet25 );
	DEF_NATFUNC( nfGet31 );
	DEF_NATFUNC( nfGet32 );
	DEF_NATFUNC( nfGet33 );
	DEF_NATFUNC( nfGet34 );
	DEF_NATFUNC( nfGet35 );
	DEF_NATFUNC( nfGet41 );
	DEF_NATFUNC( nfGet42 );
	DEF_NATFUNC( nfGet43 );
	DEF_NATFUNC( nfGet44 );
	DEF_NATFUNC( nfGet45 );
	DEF_NATFUNC( nfGetRedBase );
	DEF_NATFUNC( nfGetBlueBase );
	DEF_NATFUNC( nfGetGreenBase );
	DEF_NATFUNC( nfGetAlphaBase );
	DEF_NATFUNC( nfGetWhiteBase );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfOpAdd );
	DEF_NATFUNC( nfOpSubtract );
	DEF_NATFUNC( nfOpScale );
	DEF_NATFUNC( nfOpDivide );
	DEF_NATFUNC( nfOpMultiply );
	DEF_NATFUNC( nfOpMultiply2 );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfToString );
	DEF_NATFUNC( nfToStringPrecision );
#undef DEF_NATFUNC
};

#endif
