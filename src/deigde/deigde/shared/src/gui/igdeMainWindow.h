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

#ifndef _IGDEMAINWINDOW_H_
#define _IGDEMAINWINDOW_H_

#include "igdeWindow.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/math/decMath.h>


class igdeEngineController;
class igdeApplication;



/**
 * \brief Main window.
 */
class DE_DLL_EXPORT igdeMainWindow : public igdeWindow{
public:
	/** \brief Window state. */
	enum eWindowStates{
		ewsNormal,
		ewsMinimized,
		ewsMaximized
	};
	
	
	
private:
	igdeEngineController *pEngineController;
	eWindowStates pWindowState;
	decPoint pNormalPosition, pNormalSize;
	bool pNormalPositionSet, pNormalSizeSet;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create main window prototype. */
	igdeMainWindow(igdeEnvironment &environment, const char *title);
	
	
	
protected:
	/**
	 * \brief Clean up main window prototype.
	 * \warning Subclass is responsible to calls StopEngine to shut down the engine
	 *          before starting cleaning up widgets.
	 */
	virtual ~igdeMainWindow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Window state. */
	inline eWindowStates GetWindowState() const{return pWindowState;}
	
	/** \brief Set window state. */
	void SetWindowState(eWindowStates windowState);
	
	/** \brief Position in normal state. */
	inline const decPoint &GetNormalPosition() const{return pNormalPosition;}
	
	/** \brief Set position in normal state. */
	void SetNormalPosition(const decPoint &position);
	
	/** \brief Size in normal state. */
	inline const decPoint &GetNormalSize() const{return pNormalSize;}
	
	/** \brief Set size in normal state. */
	void SetNormalSize(const decPoint &size);
	
	/** \brief Normal position has been set at least once. */
	inline bool GetNormalPositionSet() const{return pNormalPositionSet;}
	
	/** \brief Normal size has been set at least once. */
	inline bool GetNormalSizeSet() const{return pNormalSizeSet;}
	
	
	/** \brief Get engine controller or NULL if not present. */
	inline igdeEngineController *GetMainWindowEngineController() const{return pEngineController;}
	
	/** \brief Start engine. */
	void StartEngine();
	
	/** \brief Stop engine. */
	void StopEngine();
	
	/** \brief Hook method called before the engine starts. */
	virtual void OnBeforeEngineStart();
	
	/** \brief Hook method called after the engine has been started. */
	virtual void OnAfterEngineStart();
	
	/** \brief Hook method called before the engine is shut down. */
	virtual void OnBeforeEngineStop();
	
	/** \brief Hook method called before the engine is shut down. */
	virtual void OnAfterEngineStop();
	
	/** \brief Processes frame update messages. */
	virtual void OnFrameUpdate();
	
	/** \brief Window state changed. */
	virtual void OnWindowState();
	
	
	
	/** \brief Close window bypassing CloseWindow(). */
	virtual void Close();
	
	
	
protected:
	void CreateEngineController();
	void AddNullModules();
	void ActivateNullModules();
	
	
	
public:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Drop native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DropNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Window title changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnTitleChanged();
	
	/**
	 * \brief Window icon changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnIconChanged();
	
	/**
	 * \brief Window size changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnSizeChanged();
	
	/**
	 * \brief Window position changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnPositionChanged();
	
	/** \brief Visible changed. */
	virtual void OnVisibleChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Window state changed. */
	virtual void OnWindowStateChanged();
	/*@}*/
};

#endif
