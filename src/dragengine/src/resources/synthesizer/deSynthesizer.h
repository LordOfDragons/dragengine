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

#ifndef _DESYNTHESIZER_H_
#define _DESYNTHESIZER_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/collection/decObjectOrderedSet.h"

class deRig;
class deSynthesizerSource;
class deSynthesizerLink;
class deSynthesizerManager;
class deSynthesizerController;
class deBaseSynthesizerSynthesizer;


/**
 * \brief Synthesizer producing dynamic sounds from sources.
 * 
 * Contains the definition for producing sounds. Use deSynthesizerInstance
 * for to actually produce sounds using individual parameters. Multiple
 * synthesizer instances can share the same synthesizer.
 */
class DE_DLL_EXPORT deSynthesizer : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizer> Ref;
	
	
	
private:
	decObjectOrderedSet pControllers;
	decObjectOrderedSet pLinks;
	decObjectOrderedSet pSources;
	
	int pChannelCount;
	int pSampleRate;
	int pBytesPerSample;
	int pSampleCount;
	
	deBaseSynthesizerSynthesizer *pPeerSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer. */
	deSynthesizer( deSynthesizerManager *manager );
	
protected:
	/**
	 * \brief Clean up synthesizer.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of channels. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Set number of channels. */
	void SetChannelCount( int channelCount );
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Set sample rate. */
	void SetSampleRate( int sampleRate );
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Set bytes per sample. */
	void SetBytesPerSample( int bytesPerSample );
	
	/** \brief Number of samples to create. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Set number of samples to create. */
	void SetSampleCount( int sampleCount );
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Number of controllers. */
	int GetControllerCount() const;
	
	/**
	 * \brief Controller at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or larger than or equal to GetControllerCount().
	 */
	deSynthesizerController *GetControllerAt( int index ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOfController( deSynthesizerController *controller ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOfControllerNamed( const char *name ) const;
	
	/** \brief Controller is present. */
	bool HasController( deSynthesizerController *controller ) const;
	
	/** \brief Add controller. */
	void AddController( deSynthesizerController *controller );
	
	/**
	 * \brief Remove controller.
	 * \throws deeInvalidParam \em controller is absent.
	 */
	void RemoveController( deSynthesizerController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAllControllers();
	
	/** \brief Notify peers controllers changed. */
	void NotifyControllersChanged();
	/*@}*/
	
	
	
	/** \name Links */
	/*@{*/
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/**
	 * \brief Link at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or larger than or equal to GetLinkCount().
	 */
	deSynthesizerLink *GetLinkAt( int index ) const;
	
	/** \brief Index of link or -1 if absent. */
	int IndexOfLink( deSynthesizerLink *link ) const;
	
	/** \brief Link is present. */
	bool HasLink( deSynthesizerLink *link ) const;
	
	/** \brief Add link. */
	void AddLink( deSynthesizerLink *link );
	
	/**
	 * \brief Remove link.
	 * \throws deeInvalidParam \em link is absent.
	 */
	void RemoveLink( deSynthesizerLink *link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Notify peers links changed. */
	void NotifyLinksChanged();
	/*@}*/
	
	
	
	/** \name Sources */
	/*@{*/
	/** \brief Number of sources. */
	int GetSourceCount() const;
	
	/**
	 * \brief Source at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or larger than or equal to GetSourceCount().
	 */
	deSynthesizerSource *GetSourceAt( int index ) const;
	
	/** \brief Index of source or -1 if absent. */
	int IndexOfSource( deSynthesizerSource *source ) const;
	
	/** \brief Source is present. */
	bool HasSource( deSynthesizerSource *source ) const;
	
	/** \brief Add source. */
	void AddSource( deSynthesizerSource *source );
	
	/**
	 * \brief Remove source.
	 * \throws deeOutOfBoundary \em source is absent.
	 */
	void RemoveSource( deSynthesizerSource *source );
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	/** \brief Notify peers sources changed. */
	void NotifySourcesChanged();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Synthesizer system peer or NULL if not set. */
	inline deBaseSynthesizerSynthesizer *GetPeerSynthesizer() const{ return pPeerSynthesizer; }
	
	/** \brief Set synthesizer system peer. */
	void SetPeerSynthesizer( deBaseSynthesizerSynthesizer *peer );
	/*@}*/
};

#endif
