/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALGUI_H_
#define _DEALGUI_H_

#include "dealBasics.h"
#include "config/dealConfiguration.h"
#include "filesystem/deVirtualFileSystem.h"
#include "common/string/decString.h"

class dealLauncher;
class dealFont;
class dealWidget;
class dealWidgetLayout;
class dealShader;
class dealDialog;
class dealGuiTheme;


/**
 * \brief Android display.
 */
class dealDisplay{
public:
	/** \brief Shader parameters for shape shaders. */
	enum eShaderParametersShape{
		espsTransform,
		espsTCTransform,
		espsColorTransform,
		espsColorTransform2,
		espsGamma,
		espsClipRect,
		espsTCClamp
	};
	
private:
	dealLauncher &pLauncher;
	
	EGLDisplay pDisplay;
	EGLSurface pSurface;
	EGLContext pContext;
	EGLConfig pConfig;
	int pWidth;
	int pHeight;
	
	GLuint pVBOShapes;
	
	dealGuiTheme *pDefaultGuiTheme;
	dealFont *pFontDefault;
	int pFontSizeDefault;
	dealShader *pShaderShape;
	dealShader *pShaderShapeTex;
	
	dealWidgetLayout *pContent;
	dealDialog *pDialog;
	dealWidget *pCaptureWidget;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create display. */
	dealDisplay( dealLauncher &launcher );
	
	/** \brief Clean up display. */
	~dealDisplay();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline dealLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Screen width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Screen height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Display is ready. */
	bool Ready() const;
	
	
	
	/** \brief Initialize display. */
	void Init();
	
	/** \brief Close display. */
	void Close();
	
	/** \brief Application configuration changed like screen rotated. */
	void ConfigChanged();
	
	
	
	/** \brief Content widget. */
	inline dealWidgetLayout *GetContent() const{ return pContent; }
	
	/** \brief Dialog or \em NULL if there are none. */
	inline dealDialog *GetDialog() const{ return pDialog; }
	
	/** \brief Set dialog or \em NULL to show none. */
	void SetDialog( dealDialog *dialog );
	
	/** \brief Capture widget or \em NULL if not set. */
	inline dealWidget *GetCaptureWidget() const{ return pCaptureWidget; }
	
	/** \brief Set capture widget or \em NULL if not set. */
	void SetCaptureWidget( dealWidget *widget );
	
	
	
	/** \brief Paint display if dirty. */
	void Paint();
	
	/** \brief Activate VBO shapes. */
	void ActivateVBOShapes();
	
	/** \brief Screen orientation changed. */
	void ScreenOrientationChanged();
	
	
	
	/** \brief Default gui theme. */
	inline dealGuiTheme *GetDefaultGuiTheme() const{ return pDefaultGuiTheme; }
	
	/** \brief Default font. */
	inline dealFont *GetDefaultFont() const{ return pFontDefault; }
	
	/** \brief Default font size. */
	inline int GetDefaultFontSize() const{ return pFontSizeDefault; }
	
	/** \brief Shape shader. */
	inline dealShader *GetShapeShader() const{ return pShaderShape; }
	
	/** \brief Textured shape shader. */
	inline dealShader *GetTexturedShapeShader() const{ return pShaderShapeTex; }
	/*@}*/
	
private:
	void pCleanUp();
	void pLoadDefaultAssets();
	void pCreateShapesVBO();
	void pCreateShapeShader();
	void pCreateShapeTexShader();
};

#endif
 
