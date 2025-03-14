package ch.dragondreams.delauncher.ui.example

import android.graphics.ColorMatrix
import android.graphics.ColorMatrixColorFilter
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import ch.dragondreams.delauncher.databinding.FragmentExampleBinding

class AdapterExample(
    private val examples: List<Example>,
    private val listener: Listener
) : RecyclerView.Adapter<AdapterExample.ViewHolder>() {
    interface Listener {
        fun onItemClick(example: Example)
        fun onButtonDownload(example: Example)
        fun onButtonDelete(example: Example)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return ViewHolder(
            FragmentExampleBinding.inflate(
                LayoutInflater.from(parent.context),
                parent,
                false
            )
        )

    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val example = examples[position]
        holder.labName.text = example.name

        when (example.state) {
            Example.State.NotReady -> {
                holder.imgDownload.isEnabled = true
                holder.imgDownload.colorFilter = colorMatrixEnabled
                holder.imgDelete.isEnabled = false
                holder.imgDelete.colorFilter = colorMatrixDisabled
                holder.pbDownload.visibility = View.INVISIBLE
                holder.pbDownload.progress = 0
            }

            Example.State.Downloading -> {
                holder.imgDownload.isEnabled = false
                holder.imgDownload.colorFilter = colorMatrixDisabled
                holder.imgDelete.isEnabled = false
                holder.imgDelete.colorFilter = colorMatrixDisabled
                holder.pbDownload.visibility = View.VISIBLE
                holder.pbDownload.progress = example.downloadProgress
            }

            Example.State.Ready -> {
                holder.imgDownload.isEnabled = false
                holder.imgDownload.colorFilter = colorMatrixDisabled
                holder.imgDelete.isEnabled = true
                holder.imgDelete.colorFilter = colorMatrixEnabled
                holder.pbDownload.visibility = View.INVISIBLE
                holder.pbDownload.progress = 0
            }
        }

        example.uiUpdatePending = false
    }

    override fun getItemCount(): Int = examples.size

    inner class ViewHolder(binding: FragmentExampleBinding) :
        RecyclerView.ViewHolder(binding.root) {
        val labName = binding.labName
        val imgDownload = binding.imgDownload
        val imgDelete = binding.imgDelete
        val pbDownload = binding.pbDownload

        init {
            binding.root.setOnClickListener {
                listener.onItemClick(examples[bindingAdapterPosition])
            }
            binding.imgDownload.setOnClickListener {
                listener.onButtonDownload(examples[bindingAdapterPosition])
            }
            binding.imgDelete.setOnClickListener {
                listener.onButtonDelete(examples[bindingAdapterPosition])
            }
        }

        override fun toString(): String {
            return super.toString() + " '" + labName.text + "'"
        }
    }

    companion object {
        private val colorMatrixDisabled = ColorMatrixColorFilter(createColorMatrixDisabled())

        private fun createColorMatrixDisabled(): ColorMatrix {
            val m1 = ColorMatrix()
            m1.setSaturation(0.0f)
            val m2 = ColorMatrix()
            m2.set(floatArrayOf(
                0.5f,   0f,   0f,   0f, 128f,
                  0f, 0.5f,   0f,   0f, 128f,
                  0f,   0f, 0.5f,   0f, 128f,
                  0f,   0f,   0f, 0.5f,   0f))
            m1.postConcat(m2)
            return m1
        }

        private val colorMatrixEnabled = ColorMatrixColorFilter(ColorMatrix())
    }
}