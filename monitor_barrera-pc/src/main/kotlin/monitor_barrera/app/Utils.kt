package monitor_barrera.app

import com.fazecast.jSerialComm.SerialPort
import com.fazecast.jSerialComm.SerialPortDataListener
import com.fazecast.jSerialComm.SerialPortEvent

fun SerialPort.onDataRecieved(callback: (ByteArray) -> Unit){

    addDataListener(object : SerialPortDataListener {
        override fun getListeningEvents(): Int {
            return SerialPort.LISTENING_EVENT_DATA_AVAILABLE
        }

        override fun serialEvent(event: SerialPortEvent) {

            if (event.eventType != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
                return

            val bytes = bytesAvailable()

            if(bytes > 0) {

                ByteArray(bytes).let {

                    readBytes(it, it.size.toLong())

                    callback(it)

                }

            }

        }
    })

}