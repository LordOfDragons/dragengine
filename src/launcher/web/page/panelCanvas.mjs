/**
 * Canvas panel.
 */
export class PanelCanvas {
	/**
	 * Create canvas panel.
	 * @param {String} panelId Panel identifier
	 */
	constructor (panelId) {
		this._panelId = panelId
		this._panel = document.getElementById(panelId)
		this._panelParent = this._panel.parentElement
	}
	
	/**
	 * Panel ID.
	 */
	panelId() {
		return this._panelId
	}
	
	/**
	 * Resize canvas to fit parent size.
	 */
	resizeCanvas() {
		this._panel.width = this._panelParent.innerWidth;
		this._panel.height = this._panelParent.innerHeight;
	}
	
	/**
	 * Fullscreen mode enabled.
	 */
	isFullScreen() {
		return this._panel.fullscreenElement === this._panel
	}
	
	/**
	 * Enable full screen mode.
	 */
	enableFullScreen() {
		if (this._panel.requestFullscreen) {
			this._panel.requestFullscreen()
		}
	}
	
	/**
	 * Disable full screen mode.
	 */
	disableFullScreen() {
		if (this._panel.exitFullscreen) {
			this._panel.exitFullscreen()
		}
	}
	
	/**
	 * Toggle full screen mode.
	 */
	toggleFullScreen() {
		if (this.isFullScreen()) {
			this.disableFullScreen()
		} else {
			this.enableFullScreen()
		}
	}
}
