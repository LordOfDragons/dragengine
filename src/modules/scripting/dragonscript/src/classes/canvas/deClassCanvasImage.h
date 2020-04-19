/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DECLASSCANVASIMAGE_H_
#define _DECLASSCANVASIMAGE_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deCanvasImage;
class deScriptingDragonScript;



/**
 * \brief Canvas image script class.
 */
class deClassCanvasImage : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassCanvasImage( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassCanvasImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Retrieve canvas image from a script object.
	 * \details Returns \em NULL if \em myself is \em NULL.
	 */
	deCanvasImage *GetCanvas( dsRealObject *myself ) const;
	
	/**
	 * \brief Push canvas image onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is \em NULL.
	 */
	void PushCanvas( dsRunTime *rt, deCanvasImage *canvas );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCImage, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsImage;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetImage );
	DEF_NATFUNC( nfSetImage );
	DEF_NATFUNC( nfGetRepeatX );
	DEF_NATFUNC( nfSetRepeatX );
	DEF_NATFUNC( nfGetRepeatY );
	DEF_NATFUNC( nfSetRepeatY );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
