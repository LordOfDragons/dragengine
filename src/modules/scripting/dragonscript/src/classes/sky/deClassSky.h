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

#ifndef _DECLASSSKY_H_
#define _DECLASSSKY_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deSky;



/**
 * \brief Sky script class.
 */
class deClassSky : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSky(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Sky or \em NULL if myself is \em NULL. */
	deSky *GetSky(dsRealObject *myself) const;
	
	/** \brief Push sky which can be \em NULL. */
	void PushSky(dsRunTime *rt, deSky *sky);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSky;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsColor;
		dsClass *clsSkyCtrl;
		dsClass *clsSkyLink;
		dsClass *clsSkyLayer;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetBgColor);
	DEF_NATFUNC(nfSetBgColor);
	
	DEF_NATFUNC(nfGetControllerCount);
	DEF_NATFUNC(nfSetControllerCount);
	DEF_NATFUNC(nfGetControllerAt);
	DEF_NATFUNC(nfGetControllerNamed);
	DEF_NATFUNC(nfIndexOfControllerNamed);
	
	DEF_NATFUNC(nfGetLinkCount);
	DEF_NATFUNC(nfSetLinkCount);
	DEF_NATFUNC(nfGetLinkAt);
	
	DEF_NATFUNC(nfGetLayerCount);
	DEF_NATFUNC(nfSetLayerCount);
	DEF_NATFUNC(nfGetLayerAt);
	
	DEF_NATFUNC(nfContentChanged);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
