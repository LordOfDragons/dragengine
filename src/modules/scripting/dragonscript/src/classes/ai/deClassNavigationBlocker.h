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

#ifndef _DEDSCLASSNAVIGATIONBLOCKER_H_
#define _DEDSCLASSNAVIGATIONBLOCKER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deNavigationBlocker;



/**
 * \brief Navigation Mesh Script Class.
 */
class deClassNavigationBlocker : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsNavigationSpaceType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassNavigationBlocker( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassNavigationBlocker();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the navigation blocker or NULL if myself is NULL. */
	deNavigationBlocker *GetNavigationBlocker( dsRealObject *myself ) const;
	/** \brief Pushes the navigation blocker which can be NULL. */
	void PushNavigationBlocker( dsRunTime *rt, deNavigationBlocker *blocker );
	
	inline dsClass *GetClassNavigationSpaceType() const{ return pClsNavigationSpaceType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsNavBlocker, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsDVec, *clsVec, *clsQuat, *clsShaList;
		dsClass *clsNavigationSpaceType;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetLayer );
	DEF_NATFUNC( nfSetLayer );
	DEF_NATFUNC( nfGetSpaceType );
	DEF_NATFUNC( nfSetSpaceType );
	DEF_NATFUNC( nfGetBlockingPriority );
	DEF_NATFUNC( nfSetBlockingPriority );
	DEF_NATFUNC( nfGetEnabled );
	DEF_NATFUNC( nfSetEnabled );
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC( nfGetShapeList );
	DEF_NATFUNC( nfSetShapeList );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
