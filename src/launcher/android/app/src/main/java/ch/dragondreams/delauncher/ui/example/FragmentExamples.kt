package ch.dragondreams.delauncher.ui.example

import android.content.pm.PackageManager
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.RecyclerView
import ch.dragondreams.delauncher.FileDownloader
import ch.dragondreams.delauncher.MainActivity
import ch.dragondreams.delauncher.PermissionRequestor
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.UIHelper
import ch.dragondreams.delauncher.ui.remote.FragmentRemoteLauncher
import java.io.File

class FragmentExamples : Fragment(), PermissionRequestor.Listener {
    class ExampleListener(
        private val owner: FragmentExamples
    ) : AdapterExample.Listener {
        override fun onItemClick(example: Example) {
            example.run(owner.requireActivity())
        }

        override fun onButtonDownload(example: Example) {
            owner.downloadExample(example)
        }

        override fun onButtonDelete(example: Example) {
            owner.deleteExample(example)
        }
    }

    class DownloadExample(
        private val owner: FragmentExamples,
        private val example: Example
    ) : FileDownloader.DownloadListener {
        override fun onProgress(downloader: FileDownloader, progress: Int) {
            example.downloadProgress = progress
            updateUI()
        }

        override fun onSuccess(downloader: FileDownloader) {
            try {
                example.moveDownloadToInstalled()
            } catch (e: Exception) {
                owner.requireActivity().runOnUiThread {
                    e.message?.let { m -> UIHelper.showError(owner.requireActivity(), m) }
                        ?: UIHelper.showError(owner.requireActivity(), R.string.error_examples_move_file)
                }
            }

            example.downloader = null
            example.updateState()
            owner.requireActivity().runOnUiThread {
                updateUI()
            }
        }

        override fun onFailed(downloader: FileDownloader, exception: Exception) {
            downloader.deleteDownloadFile()
            example.downloader = null
            example.updateState()

            owner.requireActivity().runOnUiThread {
                updateUI()
                exception.message?.let { m -> UIHelper.showError(owner.requireActivity(), m) }
                    ?: UIHelper.showError(owner.requireActivity(), R.string.error_download_failed)
            }
        }

        override fun onCancelled(downloader: FileDownloader) {
            downloader.deleteDownloadFile()
            example.downloader = null
            example.updateState()

            owner.requireActivity().runOnUiThread {
                updateUI()
            }
        }

        private fun updateUI(){
            if (example.uiUpdatePending) {
                return
            }

            example.uiUpdatePending = true
            owner.requireActivity().runOnUiThread {
                owner.exampleUpdateUi(example)
            }
        }
    }

    private var adapterExample: AdapterExample? = null

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_examples, container, false)
        if (view is RecyclerView) {
            (context as PermissionRequestor.Interface).getPermissionRequestor().addListener(
                REQUEST_CODE_PERMISSION_INTERNET, this)
            initExamples()
            adapterExample = AdapterExample(MainActivity.examples, ExampleListener(this))
            view.adapter = adapterExample
        }
        return view
    }

    override fun onDestroyView() {
        (context as PermissionRequestor.Interface).getPermissionRequestor().removeListener(
            REQUEST_CODE_PERMISSION_INTERNET, this)

        super.onDestroyView()
    }

    private fun getExamplesDir(): File {
        return File(requireContext().filesDir, "examples")
    }

    private fun getDownloadDir(): File {
        return File(getExamplesDir(), "download")
    }

    private fun initExamples() {
        val pathExamples = getExamplesDir()
        val pathDownload = getDownloadDir()

        MainActivity.examples.forEach { e ->
            e.pathInstalled = File(pathExamples, e.delgaFilename)
            e.pathDownloading = File(pathDownload, e.delgaFilename)
            e.updateState()
        }
    }

    private fun exampleUpdateUi(example: Example) {
        adapterExample?.notifyItemChanged(MainActivity.examples.indexOf(example))
    }

    private fun downloadExample(example: Example) {
        when (example.state) {
            Example.State.Downloading -> return
            else -> {}
        }

        example.downloader?.let { _ -> return }

        (requireContext() as PermissionRequestor.Interface).getPermissionRequestor().requiresPermissionInternet(
            requireActivity(), FragmentRemoteLauncher.REQUEST_CODE_PERMISSION_INTERNET,
            R.string.message_requires_permission_internet_download) {
                example.state = Example.State.Downloading
                example.downloader = FileDownloader(example.url,
                    example.pathDownloading!!, DownloadExample(this, example))
                example.downloader?.download()
                exampleUpdateUi(example)
        }
    }

    private fun deleteExample(example: Example) {
        if (example.pathInstalled?.exists() == false) {
            return
        }

        requireActivity().runOnUiThread {
            UIHelper.confirm(requireActivity(), R.string.message_example_delete_confirm,
                R.string.title_example_delete, R.string.label_delete, R.string.label_cancel) { accepted ->
                    if (!accepted) {
                        return@confirm
                    }

                    try {
                        example.deleteInstalledFile()
                    } catch (e: Exception) {
                        e.message?.let { m -> UIHelper.showError(requireActivity(), m) }
                            ?: UIHelper.showError(requireActivity(), R.string.error_delete_example_file)
                    }
                    example.updateState()
                    requireActivity().runOnUiThread {
                        exampleUpdateUi(example)
                    }
            }
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int,
        permissions: Array<out String>, grantResults: IntArray) {
        when (requestCode) {
            REQUEST_CODE_PERMISSION_INTERNET -> {
                // If request is cancelled, the result arrays are empty.
                if((grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
                    // nothing
                } else {
                    requireActivity().runOnUiThread {
                        UIHelper.showError(requireActivity(),  R.string.message_download_denied)
                    }
                }
            }
        }
    }

    companion object{
        val REQUEST_CODE_PERMISSION_INTERNET = PermissionRequestor.nextRequestCode()
    }
}