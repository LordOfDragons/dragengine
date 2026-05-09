/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DECLASSSCENE_H_
#define _DECLASSSCENE_H_

#include <libdscript/libdscript.h>

class deScene;
class deScriptingDragonScript;



/**
 * \brief Scene script class.
 */
class deClassScene : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassScene(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	~deClassScene() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine) override;
	
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Scene from object or \em NULL if myself is \em NULL. */
	deScene *GetScene(dsRealObject *myself) const;
	
	/** \brief Push scene onto stack. NULL is pushed as NULL scene object. */
	void PushScene(dsRunTime *rt, deScene *scene);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsScene;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsObj;
		dsClass *clsInt;
		dsClass *clsArray;
		
		dsClass *clsRN;
		dsClass *clsMemFile;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfLoadAsynchron);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetFilename);
	
	DEF_NATFUNC(nfGetResourceCount);
	DEF_NATFUNC(nfGetResourceKeys);
	DEF_NATFUNC(nfGetResourceNamed);
	DEF_NATFUNC(nfHasResourceNamed);
	DEF_NATFUNC(nfAddResource);
	DEF_NATFUNC(nfRemoveResource);
	DEF_NATFUNC(nfRemoveAllResources);
	
	DEF_NATFUNC(nfGetFileCount);
	DEF_NATFUNC(nfGetFileKeys);
	DEF_NATFUNC(nfGetFileNamed);
	DEF_NATFUNC(nfHasFileNamed);
	DEF_NATFUNC(nfAddFile);
	DEF_NATFUNC(nfRemoveFile);
	DEF_NATFUNC(nfRemoveAllFiles);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
