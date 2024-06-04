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

#ifndef _DEDSCLASSANIMATORCONTROLLER_H_
#define _DEDSCLASSANIMATORCONTROLLER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deAnimator;
class deAnimatorInstance;



/**
 * \brief Animator controller script class.
 */
class deClassAnimatorController : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassAnimatorController( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassAnimatorController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get parameters from object. */
	void GetController( dsRealObject *myself, deAnimator *&animator,
		deAnimatorInstance *&instance, int &index ) const;
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deAnimator *animator, int index );
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deAnimatorInstance *instance, int index );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsAnimatorCtrl;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsVector;
		dsClass *clsAnimator;
		dsClass *clsAnimatorInst;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetAnimator );
	DEF_NATFUNC( nfGetAnimatorInstance );
	DEF_NATFUNC( nfGetControllerIndex );
	
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfSetName );
	DEF_NATFUNC( nfGetLower );
	DEF_NATFUNC( nfGetUpper );
	DEF_NATFUNC( nfSetRange );
	DEF_NATFUNC( nfGetValue );
	DEF_NATFUNC( nfSetValue );
	DEF_NATFUNC( nfGetRelativeValue );
	DEF_NATFUNC( nfSetRelativeValue );
	DEF_NATFUNC( nfGetVector );
	DEF_NATFUNC( nfSetVector );
	DEF_NATFUNC( nfGetRotation );
	DEF_NATFUNC( nfSetRotation );
	DEF_NATFUNC( nfGetFrozen );
	DEF_NATFUNC( nfSetFrozen );
	DEF_NATFUNC( nfGetClamp );
	DEF_NATFUNC( nfSetClamp );
	
	DEF_NATFUNC( nfIncrement );
	DEF_NATFUNC( nfSetToLower );
	DEF_NATFUNC( nfSetToCenter );
	DEF_NATFUNC( nfSetToUpper );
	DEF_NATFUNC( nfReverse );
	DEF_NATFUNC( nfSetValueFrom );
	DEF_NATFUNC( nfSetAllFrom );
	DEF_NATFUNC( nfIsAtLower );
	DEF_NATFUNC( nfIsAtUpper );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
