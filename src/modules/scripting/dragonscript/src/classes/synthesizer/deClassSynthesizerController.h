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

#ifndef _DEDSCLASSSYNTHESIZERCONTROLLER_H_
#define _DEDSCLASSSYNTHESIZERCONTROLLER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deSynthesizer;
class deSynthesizerInstance;



/**
 * \brief Synthesizer controller script class.
 */
class deClassSynthesizerController : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSynthesizerController( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassSynthesizerController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get parameters from object. */
	void GetController( dsRealObject *myself, deSynthesizer *&synthesizer,
		deSynthesizerInstance *&instance, int &index ) const;
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deSynthesizer *synthesizer, int index );
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deSynthesizerInstance *instance, int index );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSynthesizerCtrl;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsCurveBezier;
		dsClass *clsSynthesizer;
		dsClass *clsSynthesizerInst;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetSynthesizer );
	DEF_NATFUNC( nfGetSynthesizerInstance );
	DEF_NATFUNC( nfGetControllerIndex );
	
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfSetName );
	DEF_NATFUNC( nfGetLower );
	DEF_NATFUNC( nfGetUpper );
	DEF_NATFUNC( nfSetRange );
	DEF_NATFUNC( nfGetCurve );
	DEF_NATFUNC( nfSetCurve );
	DEF_NATFUNC( nfSetCurveConstant );
	DEF_NATFUNC( nfSetCurveTime );
	DEF_NATFUNC( nfGetClamp );
	DEF_NATFUNC( nfSetClamp );
	DEF_NATFUNC( nfSetCurveFrom );
	DEF_NATFUNC( nfSetAllFrom );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
