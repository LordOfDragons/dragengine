// import delauncherWasm from './delauncher-web.wasm'
// const delauncherJS = import('./delauncher-web.js')

var Module = {
	onRuntimeInitialized: function() {
		console.log('Runtime initialized')
	}
}

importScripts('./delauncher-web.js')

/**
 * Web launcher worker.
 */
class WebLauncherWorker{
	/**
	 * Send log event.
	 * 
	 * Event details contains:
	 * - severity {PanelLogs.Severity}: Log severity.
	 * - source {String}: Log source.
	 * - message {String}: Log message.
	 */
	static EventLog = 'delauncher:log'
	
	/**
	 * Web launcher has finished initialize event.
	 * 
	 * Event details contains:
	 * - error {Any}: Error or null if success.
	 */
	static EventInitialized = 'delauncher:initialized'
	
	/**
	 * Game running state changed.
	 * 
	 * Event details contains:
	 * - running {Boolean}: Game is running.
	 */
	static EventGameRunningChanged = 'delauncher:gameRunningChanged'
	
	/**
	 * Game can not run due to problems.
	 * 
	 * Event details contains:
	 * - gameProblems {String[]}: Game problems or undefined.
	 * - profileName {String}: Profile name or undefined.
	 * - profileProblems {String[]}: Profile problems or undefined.
	 * - patchProblems {String[]}: Patch problems or undefined.
	 */
	static EventRunProblems = 'delauncher:gameProblems'
	
	
	/**
	 * Create web launcher worker.
	 */
	constructor() {
		this.gameRunning = false
		onmessage = this.onWorkerMessage.bind(this)
		this.loadWasm()
	}
	
	/**
	 * Worker message received.
	 */
	onWorkerMessage(event) {
		const data = event.data
		const action = data['action']
		
		if (action == 'dummy') {
			
		}
	}
	
	/**
	 * Download and instantiate wasm.
	 */
	loadWasm() {
		this.logInfo('Loading WASM')
		
		/*
		import('./delauncher-web.js').then(result => {
			console.log(`CHECK3: ${WorkerGlobalScope}`)
			console.log(`CHECK2: ${result.values()}`)
			console.log(`CHECK: ${Module}`)
			myself.logInfo('WASM ready')
			myself.initFileSystem()
		})
		*/
		
		// import('./delauncher-web.js').then(result => { console.log('IMPORT DONE') })
		
		/*
		const request = new XMLHttpRequest()
		request.open('GET', 'delauncher-web.wasm')
		request.responseType = 'arraybuffer'
		request.send()
		
		const myself = this
		request.onload = () => {
			import('./delauncher-web.js').then(result => {
				console.log(`CHECK3: ${WorkerGlobalScope.Module}`)
				console.log(`CHECK2: ${result.values()}`)
				console.log(`CHECK: ${Module}`)
				myself.logInfo('WASM ready')
				myself.initFileSystem()
			})
			//delauncherJS({wasmBinary: request.response}).then(() => {
			//	myself.logInfo('WASM ready')
			//	myself.initFileSystem()
			//})
		}
		
		request.onprogress = (event) => {
			if (event.lengthComputable) {
				this.logInfo(`Received ${event.loaded}/${event.total} (${(event.loaded * 100 / event.total).toFixed(1)}%)`)
			}
		}
		*/
	}
	
	/**
	 * Initialize file system.
	 */
	initFileSystem() {
		this.logInfo('Initialize file system')
		FS.mkdir('/idbfs');
		FS.mount(FS.filesystems.IDBFS, {autoPersist: true}, '/idbfs');
		FS.syncfs(true, this.initFileSystem2.bind(this));
	}
	
	initFileSystem2(err) {
		if (err) {
			postMessage({
				action: WebLauncherWorker.EventInitialized,
				error: err
			})
			return
		}
		
		if (!FS.analyzePath('/idbfs/config').exists) {
			FS.mkdir('/idbfs/config');
		}
		if (!FS.analyzePath('/idbfs/cache').exists) {
			FS.mkdir('/idbfs/cache');
		}
		if (!FS.analyzePath('/idbfs/capture').exists) {
			FS.mkdir('/idbfs/capture');
		}
		
		FS.mkdir('/dragengine');
		FS.symlink('/idbfs/config', '/dragengine/userConfig');
		FS.symlink('/idbfs/cache', '/dragengine/userCache');
		FS.symlink('/idbfs/capture', '/dragengine/userCapture');
		
		if (!FS.analyzePath('/dragengine/userCache/delgas').exists) {
			FS.mkdir('/dragengine/userCache/delgas');
		}
		
		FS.mkdir('/dragengine/localDelgas');
		this.logInfo('File system ready')
		this.initLauncher()
	}
	
	/**
	 * Create launcher and initialize it.
	 */
	initLauncher() {
		this.logInfo('Create launcher')
		
		try {
			this.launcher = new Module.dewlLauncher()
			this.logInfo('Initinalize launcher')
			
			/*
			this._launcher.AddArgument("this_is_a_test")
			this._launcher.AddArgument("something_else")
			
			var args_in = this._launcher.GetArgumentList()
			var args = new Array(args_in.size()).fill(0).map((_,i) => args_in.get(i))
			this.logInfo("Arguments: " + args)
			*/
			
			this.launcher.init()
			
		} catch (e) {
			this.logError(`${e}: ${e.stack}`)
			this.disposeLauncher()
			postMessage({
				action: WebLauncherWorker.EventInitialized,
				error: e
			})
			return
		}
		this.logInfo('Launcher initialized')
		postMessage({ action: WebLauncherWorker.EventInitialized })
		
		// TEST
		this.logInfo(`TEST: ${FS.readdir('/dragengine')}`)
	}
	
	/**
	 * Shutdown launcher and dispose it if existing.
	 */
	disposeLauncher() {
		if (this.launcher === undefined) {
			return
		}
		
		this.logInfo('Dispose of launcher')
		try {
			this.launcher.cleanUp()
			
		} finally {
			this.launcher.delete()
			this.launcher = undefined
		}
		this.logInfo('Launcher disposed')
	}
	
	logInfo(message) {
		postMessage({
			action: WebLauncherWorker.EventLog,
			severity: 0,
			source: 'Launcher',
			message: message
		})
	}
	
	logWarn(message) {
		postMessage({
			action: WebLauncherWorker.EventLog,
			severity: 1,
			source: 'Launcher',
			message: message
		})
	}
	
	logError(message) {
		postMessage({
			action: WebLauncherWorker.EventLog,
			severity: 2,
			source: 'Launcher',
			message: message
		})
	}
}

const launcher = new WebLauncherWorker()
