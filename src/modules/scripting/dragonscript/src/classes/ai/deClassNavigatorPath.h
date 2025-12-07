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

#ifndef _DEDSCLASSNAVIGATORPATH_H_
#define _DEDSCLASSNAVIGATORPATH_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deNavigatorPath;



/**
 * \brief Navigator path script class.
 */
class deClassNavigatorPath : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassNavigatorPath(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassNavigatorPath();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Navigator path or throws exception if myself is \em NULL. */
	deNavigatorPath &GetNavigatorPath(dsRealObject *myself) const;
	
	/** \brief Push navigator path. */
	void PushNavigatorPath(dsRunTime *rt, const deNavigatorPath &path);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsNavPath;
		
		dsClass *clsBool;
		dsClass *clsFloat;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsString;
		dsClass *clsVoid;
		
		dsClass *clsDVector;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
		dsClass *clsDebugDrawer;
		dsClass *clsDMatrix;
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
	
	DEF_NATFUNC(nfGetCount);
	DEF_NATFUNC(nfGetAt);
	DEF_NATFUNC(nfSetAt);
	DEF_NATFUNC(nfAdd);
	DEF_NATFUNC(nfAddPath);
	DEF_NATFUNC(nfRemoveFrom);
	DEF_NATFUNC(nfRemoveAll);
	DEF_NATFUNC(nfTransform);
	DEF_NATFUNC(nfTransformed);
	DEF_NATFUNC(nfGetLength);
	DEF_NATFUNC(nfGetLengthTo);
	DEF_NATFUNC(nfGetLengthFrom);
	DEF_NATFUNC(nfGetLengthBetween);
	
	DEF_NATFUNC(nfReadFromFile);
	DEF_NATFUNC(nfWriteToFile);
	
	DEF_NATFUNC(nfUpdateDebugDrawer);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

#endif
