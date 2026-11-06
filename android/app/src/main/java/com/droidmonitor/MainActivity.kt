package com.droidmonitor

import android.app.Activity
import android.os.Bundle
import android.view.Gravity
import android.widget.Button
import android.widget.LinearLayout
import android.widget.ScrollView
import android.widget.TextView

class MainActivity : Activity() {

    private var server: TcpServer? = null

    private lateinit var statusText: TextView
    private lateinit var addressText: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        createInterface()
        refreshAddresses()
    }

    private fun createInterface() {

        val layout = LinearLayout(this).apply {

            orientation = LinearLayout.VERTICAL

            setPadding(40, 50, 40, 40)

            gravity = Gravity.CENTER_HORIZONTAL
        }

        val title = TextView(this).apply {

            text = "DroidMonitor"

            textSize = 30f
        }

        addressText = TextView(this).apply {

            textSize = 16f

            setPadding(0, 40, 0, 40)
        }

        statusText = TextView(this).apply {

            text = "Servidor desligado"

            textSize = 16f
        }

        val startButton = Button(this).apply {

            text = "INICIAR SERVIDOR"

            setOnClickListener {
                startServer()
            }
        }

        val stopButton = Button(this).apply {

            text = "PARAR SERVIDOR"

            setOnClickListener {
                stopServer()
            }
        }

        layout.addView(title)
        layout.addView(addressText)
        layout.addView(statusText)
        layout.addView(startButton)
        layout.addView(stopButton)

        val scroll = ScrollView(this)

        scroll.addView(layout)

        setContentView(scroll)
    }

    private fun refreshAddresses() {

        val addresses = NetworkInfo.getIPv4Addresses()

        addressText.text =
            "Endereços IPv4 disponíveis:\n\n" +
            addresses.joinToString("\n")
                .ifEmpty { "Nenhum endereço encontrado" }
    }

    private fun startServer() {

        if (server != null)
            return

        val instance = TcpServer { message ->

            runOnUiThread {
                statusText.text = message
                refreshAddresses()
            }
        }

        server = instance

        Thread {
            instance.start()

            runOnUiThread {
                if (server === instance) {
                    server = null
                    statusText.text = "Servidor desligado"
                }
            }

        }.start()

        statusText.text = "Iniciando servidor..."
    }

    private fun stopServer() {

        server?.stop()

        server = null

        statusText.text = "Servidor desligado"
    }

    override fun onDestroy() {

        stopServer()

        super.onDestroy()
    }
}