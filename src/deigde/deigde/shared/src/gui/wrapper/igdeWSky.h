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

#ifndef _IGDEWSKY_H_
#define _IGDEWSKY_H_

#include "../../gamedefinition/sky/igdeGDSky.h"
#include "../../meta/igdeMetaContext.h"
#include "../../meta/property/igdeMetaPropertyPath.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/world/deWorld.h>

class igdeEnvironment;

class deSky;
class deSkyController;


/**
 * \brief Sky wrapper.
 * 
 * Provides a simple way to display a sky from game definition or a custom sky.
 * The sky can be modified by changing controller values.
 */
class DE_DLL_EXPORT igdeWSky : public deObject{
public:
	/** \brief Reference */
	using Ref = deTObjectReference<igdeWSky>;
	
	
	/** \brief Asynchronous loading finished. */
	class DE_DLL_EXPORT cAsyncLoadFinished{
	public:
		/** \brief Create listener. */
		cAsyncLoadFinished();
		
		/** \brief Clean up listener. */
		virtual ~cAsyncLoadFinished();
		
		/** \brief Loading finished. */
		virtual void LoadFinished(igdeWSky &wrapper, bool succeeded) = 0;
	};
	
	/** \brief Meta context. */
	class DE_DLL_EXPORT MetaContext : public igdeMetaContext{
	private:
		igdeWSky *pWrapper;
		igdeWSky::Ref pGuard;
		
	public:
		using Ref = deTObjectReference<MetaContext>;
		MetaContext(igdeWSky *wrapper);
		
		inline igdeWSky *GetWrapper() const{ return pWrapper; }
		igdeWSky &GetWrapperRef() const;
		Ref Capture() const;
		igdeEnvironment &GetEnvironment() const override;
		igdeUndoSystem *GetUndoSystem() const override;
		igdeClipboard *GetClipboard() const override;
		
	protected:
		virtual ~MetaContext() override;
	};
	
	/** \brief Meta properties. */
	class DE_DLL_EXPORT MetaProperties{
	public:
		template <typename T> class TBase : public T{
		public:
			template <typename... A> TBase(A&&... args) : T(std::forward<A>(args)...) {}
			
			igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
				return context.DynamicCast<MetaContext>()->Capture();
			}
			
			bool IsValid(const igdeMetaContext::Ref &context) const override{
				const auto c = context.DynamicCast<MetaContext>();
				return c && !c->IsDisposed();
			}
			
			inline igdeWSky &Wrapper(const igdeMetaContext::Ref &context) const{
				return context.DynamicCast<MetaContext>()->GetWrapperRef();
			}
			
		protected:
			virtual ~TBase() override{}
		};
		
		class DE_DLL_EXPORT Path : public TBase<igdeMetaPropertyPathStorage>{
		public:
			Path();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Path() override;
		};
		
	private:
		MetaProperties();
		
	public:
		deTObjectReference<Path> path;
		
		const igdeMetaContext::PropertyList::Ref properties;
		
		static MetaProperties global;
	};
	
	
	
private:
	igdeEnvironment &pEnvironment;
	
	MetaContext::Ref pMetaContext;
	
	deWorld::Ref pEngWorld;
	deSkyInstance::Ref pEngSkyInstance;
	float pMaxLightIntensity;
	igdeGDSky::Ref pGDSky;
	
	cAsyncLoadFinished *pAsyncLoadFinished;
	int pAsyncLoadCounter;
	
	
public:
	/** \brief Meta property sky path. */
	igdeMetaPropertyPathStorage::Storage path;
	
	/** \brief Sky changed event. */
	igdeTEvent<> onChanged;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky wrapper. */
	explicit igdeWSky(igdeEnvironment &environment);
	
	igdeWSky(const igdeWSky &copy) = delete;
	igdeWSky(igdeWSky &&move) = delete;
	igdeWSky& operator=(const igdeWSky &other) = delete;
	igdeWSky& operator=(igdeWSky &&other) = delete;
	
protected:
	/** \brief Clean up wrapper. */
	~igdeWSky() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Meta context. */
	inline const MetaContext::Ref &GetMetaContext() const{ return pMetaContext; }
	
	/** \brief World or nullptr. */
	inline const deWorld::Ref &GetWorld() const{ return pEngWorld; }
	
	/** \brief Set world or nullptr. */
	void SetWorld(deWorld *world);
	
	/** \brief Maximum light intensity found in the sky. */
	inline float GetMaxLightIntensity() const{ return pMaxLightIntensity; }
	
	/** \brief Get sky instance controller count. */
	int GetControllerCount() const;
	
	/** \brief Get sky instance controller. */
	const deSkyController &GetControllerAt(int index) const;
	
	/** \brief Set controller value. */
	void SetControllerValue(int index, float value);
	
	/** \brief Get sky or nullptr. */
	const deSky *GetSky() const;
	
	/** \brief Game definition sky or nullptr if sky is set manually. */
	inline const igdeGDSky::Ref &GetGDSky() const{ return pGDSky; }
	
	/** \brief Sky path or nullptr if sky is set manually. */
	inline const decString &GetPath() const{ return path; }
	
	/** \brief Set sky to use. */
	void SetSky(deSky *sky);
	
	/** \brief Set sky from default sky game definition if present. */
	void SetGDDefaultSky();
	
	/** \brief Set sky from game definition sky. */
	void SetGDSky(igdeGDSky *gdSky);
	
	/**
	 * \brief Set sky from path if present.
	 * 
	 * If path exists in game definition stores also the game definition.
	 */
	void SetPath(const char *path);
	
	
	
	/** \brief Asynchronous load finished listener or nullptr. */
	inline cAsyncLoadFinished *GetAsyncLoadFinished() const{ return pAsyncLoadFinished; }
	
	/**
	 * \brief Set asynchronous load finished listener or nullptr.
	 * 
	 * Caller is responsible to keep the listener alive. Wrapper does not hold reference.
	 */
	void SetAsyncLoadFinished(cAsyncLoadFinished *listener);
	
	
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * Call this method if you received an OnGameDefinitionChanged() on the parent module.
	 */
	void OnGameDefinitionChanged();
	/*@}*/
	
	
	
private:
	void pSetSky(deSky *sky);
	void pSetGDSky(igdeGDSky *gdsky);
	void pLoadSky(const char *path);
	void pCheckAsyncLoadFinished();
};

#endif
