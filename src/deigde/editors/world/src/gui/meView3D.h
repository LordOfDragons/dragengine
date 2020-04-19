/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEVIEW3D_H_
#define _MEVIEW3D_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseKeyListenerReference.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasTextReference.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>
#include <dragengine/resources/font/deFontReference.h>

class meView3DListener;
class meViewEditor;

class meWorld;
class meWindowMain;


/**
 * \brief 3D view of the world.
 */
class meView3D : public igdeViewRenderWindow{
private:
	meWindowMain &pWindowMain;
	meView3DListener *pListener;
	
	meWorld *pWorld;
	
	igdeMouseKeyListenerReference pListenerEditor;
	meViewEditor *pEditor;
	
	int pFPSHistory[ 30 ];
	float pFPSRedrawCanvasDelay;
	
	deFontReference pFontStats;
	deCanvasViewReference pCanvasFPS;
	deCanvasTextReference pCanvasFPSText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	meView3D( meWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~meView3D();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world render. */
	void SetWorld( meWorld *world );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	/** \brief Resize canvas to fit window size. */
	virtual void OnResize();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Mode changed. */
	void ModeChanged();
	
	/** \brief Active camera changed. */
	void ActiveCameraChanged();
	
	/** \brief Editor or NULL. */
	inline meViewEditor *GetEditor() const{ return pEditor; }
	/*@}*/
};

#endif
