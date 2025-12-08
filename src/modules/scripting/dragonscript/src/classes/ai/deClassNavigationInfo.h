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

#ifndef _DEDSCLASSNAVIGATION_H_
#define _DEDSCLASSNAVIGATION_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class dedsNavigationInfo;



/**
 * \brief Navigator information script class.
 */
class deClassNavigationInfo : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassNavigationInfo(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassNavigationInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	
	
	/** \brief Navigation information or \em NULL if myself is \em NULL. */
	dedsNavigationInfo *GetNavigationInfo(dsRealObject *myself) const;
	
	/** \brief Pushe navigation info or \em NULL. */
	void PushNavigationInfo(dsRunTime *rt, dedsNavigationInfo *info);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsNavInfo;
		
		dsClass *clsBool;
		dsClass *clsDVector;
		dsClass *clsFloat;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsString;
		dsClass *clsVoid;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewCopy);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetCostType);
	DEF_NATFUNC(nfSetCostType);
	
	DEF_NATFUNC(nfGetPathIndex);
	DEF_NATFUNC(nfSetPathIndex);
	DEF_NATFUNC(nfGetPathFactor);
	DEF_NATFUNC(nfSetPathFactor);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

#endif
