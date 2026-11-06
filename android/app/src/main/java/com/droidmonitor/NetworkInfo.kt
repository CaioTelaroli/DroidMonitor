package com.droidmonitor

import java.net.Inet4Address
import java.net.NetworkInterface

object NetworkInfo {

    fun getIPv4Addresses(): List<String> {

        val result = mutableListOf<String>()

        try {
            val interfaces = NetworkInterface.getNetworkInterfaces()

            while (interfaces.hasMoreElements()) {

                val network = interfaces.nextElement()

                if (!network.isUp || network.isLoopback)
                    continue

                val addresses = network.inetAddresses

                while (addresses.hasMoreElements()) {

                    val address = addresses.nextElement()

                    if (
                        address is Inet4Address &&
                        !address.isLoopbackAddress &&
                        !address.isLinkLocalAddress
                    ) {
                        result.add(address.hostAddress ?: "")
                    }
                }
            }

        } catch (e: Exception) {
            e.printStackTrace()
        }

        return result.distinct()
    }
}