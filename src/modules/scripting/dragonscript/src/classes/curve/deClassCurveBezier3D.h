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

#ifndef _DEDSCLASSCURVEBEZIER3D_H_
#define _DEDSCLASSCURVEBEZIER3D_H_

#include <libdscript/libdscript.h>

class decCurveBezier3DEvaluator;
class decCurveBezier3D;
class deScriptingDragonScript;



/**
 * \brief 3D bezier curve script class.
 */
class deClassCurveBezier3D : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassCurveBezier3D( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassCurveBezier3D();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dragonscript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Curve. */
	decCurveBezier3D &GetCurve( dsRealObject *myself ) const;
	
	/** \brief Curve evaluator. */
	decCurveBezier3DEvaluator &GetEvaluator( dsRealObject *myself ) const;
	
	/** \brief Push curve. */
	void PushCurve( dsRunTime *rt, const decCurveBezier3D &curve );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsCBezier3D;
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsVector;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPointCount );
	DEF_NATFUNC( nfGetPointAt );
	DEF_NATFUNC( nfGetHandle1At );
	DEF_NATFUNC( nfGetHandle2At );
	DEF_NATFUNC( nfSetPointAt );
	DEF_NATFUNC( nfSetPointAt2 );
	DEF_NATFUNC( nfAddPoint );
	DEF_NATFUNC( nfAddPoint2 );
	DEF_NATFUNC( nfInsertPoint );
	DEF_NATFUNC( nfInsertPoint2 );
	DEF_NATFUNC( nfRemovePointFrom );
	DEF_NATFUNC( nfRemoveAllPoints );
	
	DEF_NATFUNC( nfEvaluateAt );
	DEF_NATFUNC( nfEvaluateAt2 );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
