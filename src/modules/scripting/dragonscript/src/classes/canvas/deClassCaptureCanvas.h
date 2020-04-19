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

#ifndef _DECLASSCAPTURECANVAS_H_
#define _DECLASSCAPTURECANVAS_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deCaptureCanvas;
class deScriptingDragonScript;



/**
 * \brief Capture canvas script class.
 */
class deClassCaptureCanvas : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassCaptureCanvas( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassCaptureCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Retrieve capture canvas from a script object.
	 * \details Returns \em NULL if \em myself is \em NULL.
	 */
	deCaptureCanvas *GetCaptureCanvas( dsRealObject *myself ) const;
	
	/**
	 * \brief Push capture canvas onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is \em NULL.
	 */
	void PushCaptureCanvas( dsRunTime *rt, deCaptureCanvas *captureCanvas );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCapCan, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsCView, *clsImage;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetCanvasView );
	DEF_NATFUNC( nfSetCanvasView );
	DEF_NATFUNC( nfGetImage );
	DEF_NATFUNC( nfSetImage );
	DEF_NATFUNC( nfDropImage );
	DEF_NATFUNC( nfGetCapture );
	DEF_NATFUNC( nfSetCapture );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
