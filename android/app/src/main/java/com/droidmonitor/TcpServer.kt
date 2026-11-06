package com.droidmonitor

import java.io.DataInputStream
import java.io.DataOutputStream
import java.net.ServerSocket
import java.net.Socket
import java.net.SocketException

class TcpServer(
    private val onStatus: (String) -> Unit
) {

    companion object {
        const val PORT = 24816
        const val VERSION = 1
        const val MAX_PAYLOAD = 1024 * 1024
    }

    @Volatile
    private var running = false

    private var serverSocket: ServerSocket? = null

    fun start() {

        if (running)
            return

        running = true

        try {

            serverSocket = ServerSocket(PORT)

            onStatus("Servidor iniciado na porta $PORT")

            while (running) {

                val client = serverSocket!!.accept()

                Thread {
                    handleClient(client)
                }.start()
            }

        } catch (e: Exception) {

            if (running) {
                onStatus("Erro: ${e.message}")
            }

        } finally {

            running = false

            try {
                serverSocket?.close()
            } catch (_: Exception) {
            }
        }
    }

    private fun handleClient(socket: Socket) {

        socket.use { client ->

            try {

                client.soTimeout = 10000

                val input = DataInputStream(
                    client.getInputStream()
                )

                val output = DataOutputStream(
                    client.getOutputStream()
                )

                onStatus(
                    "Conexão recebida: ${client.inetAddress.hostAddress}"
                )

                while (running && !client.isClosed) {

                    val magic = ByteArray(4)
                    input.readFully(magic)

                    if (!magic.contentEquals(
                        byteArrayOf(68, 77, 79, 78)
                    )) {
                        throw Exception("Magic inválido")
                    }

                    val version = input.readUnsignedShort()
                    val type = input.readUnsignedShort()
                    val length = input.readInt()

                    if (version != VERSION)
                        throw Exception("Versão incompatível")

                    if (length < 0 || length > MAX_PAYLOAD)
                        throw Exception("Payload inválido")

                    val payload = ByteArray(length)

                    input.readFully(payload)

                    val message = String(
                        payload,
                        Charsets.UTF_8
                    )

                    when (type) {

                        1 -> {

                            if (message != "DROIDMONITOR_HOST")
                                throw Exception("Cliente não reconhecido")

                            sendMessage(
                                output,
                                2,
                                "DROIDMONITOR_ANDROID"
                            )

                            onStatus("Handshake concluído!")
                        }

                        3 -> {

                            sendMessage(
                                output,
                                4,
                                message
                            )
                        }

                        5 -> {

                            onStatus("Windows desconectou")
                            break
                        }

                        else -> {
                            throw Exception("Mensagem desconhecida: $type")
                        }
                    }
                }

            } catch (e: SocketException) {

                onStatus("Conexão encerrada")

            } catch (e: Exception) {

                onStatus("Erro de comunicação: ${e.message}")
            }
        }
    }

    private fun sendMessage(
        output: DataOutputStream,
        type: Int,
        payload: String
    ) {

        val data = payload.toByteArray(Charsets.UTF_8)

        require(data.size <= MAX_PAYLOAD)

        synchronized(output) {

            output.write(byteArrayOf(68, 77, 79, 78))

            output.writeShort(VERSION)
            output.writeShort(type)
            output.writeInt(data.size)

            output.write(data)
            output.flush()
        }
    }

    fun stop() {

        running = false

        try {
            serverSocket?.close()
        } catch (_: Exception) {
        }
    }
}