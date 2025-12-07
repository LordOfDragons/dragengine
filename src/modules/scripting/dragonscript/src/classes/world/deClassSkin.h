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

#ifndef _DECLASSSKIN_H_
#define _DECLASSSKIN_H_

#include <libdscript/libdscript.h>

class deEngine;
class deSkin;
class deScriptingDragonScript;



/**
 * \brief Skin script class.
 */
class deClassSkin : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSkinPropertyType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSkin(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Skin from object. */
	deSkin *GetSkin(dsRealObject *myself) const;
	
	/** \brief Pushe skin. */
	void PushSkin(dsRunTime *rt, deSkin *skin);
	
	inline dsClass *GetClassSkinPropertyType() const{return pClsSkinPropertyType;}
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkin;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsObject;
		
		dsClass *clsResourceListener;
		dsClass *clsColor;
		dsClass *clsImage;
		dsClass *clsVideo;
		dsClass *clsSkinPropertyType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfLoad);
	DEF_NATFUNC(nfLoadAsynchron);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetTextureCount);
	DEF_NATFUNC(nfIndexOfTextureNamed);
	
	DEF_NATFUNC(nfTextureGetName);
	DEF_NATFUNC(nfTextureGetPropertyCount);
	DEF_NATFUNC(nfTextureIndexOfPropertyNamed);
	
	DEF_NATFUNC(nfPropertyGetName);
	DEF_NATFUNC(nfPropertyGetRenderable);
	DEF_NATFUNC(nfPropertyGetType);
	DEF_NATFUNC(nfPropertyGetValue);
	DEF_NATFUNC(nfPropertyGetColor);
	DEF_NATFUNC(nfPropertyGetImage);
	DEF_NATFUNC(nfPropertyGetVideo);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
