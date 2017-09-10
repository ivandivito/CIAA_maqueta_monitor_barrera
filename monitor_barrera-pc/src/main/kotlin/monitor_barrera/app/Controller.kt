package monitor_barrera.app

import com.fazecast.jSerialComm.SerialPort
import rx.Observable
import rx.lang.kotlin.BehaviorSubject
import rx.lang.kotlin.PublishSubject

/*
    Lo que entendí de Reactivex hasta ahora

    Un evento puede ser de cualquier tipo (clase) y se manifiesta como un llamado a las funciones
    onNext/onError/onComplete de un Observer

    Observer es algo que recibe eventos, es decir, recibe llamadas a sus funciones onNext/onError/onComplete.

    Observable es algo que produce eventos, es decir, llama funciones onNext/onError/onComplete.

    A un Observable se subscriben uno o más Observer para recibir sus eventos .El Observable itera por sus subscriptores
    y llama a onNext/onError/onComplete segun corresponda.

    A un Observable se le puede aplicar un Operation para obtener otro Observable. Esto lo que hace es crear otro
    Observable que produce eventos en funcion de los eventos del Observable original. Por ejemplo Filter o Map.

    Un Subject es Observer y Observable, es decir, produce y recibe eventos, es decir, recibe llamadas a sus funciones
    onNext/onError/onComplete e itera sus subscriptores llamando a onNext/onError/onComplete segun corresponda. Los puristas
    ReactiveX dicen que nunca hace falta usar un Subject, pero hay que ser pro Rx para poder plantearlo de esa manera.

    PublishSubject funciona como Bus/Canal/Stream donde se puede publicar y los subscriptores reciben los eventos.

    BehaviorSubject vendria a ser una variable observable en Reactivex. Es un Subject, se puede subcribir a él y puede
    recibir eventos. En la propiedad value almacena el "valor actual", es decir, el último evento recibido/enviado.
    Cuando se subcribe a él, el observer recibe inmediatamente un evento con el valor actual.


    La implementacion de ReactiveX se da en OnSubscribe. Al subscribir un Observer a un Observable, se llama en el Observable
    el callback OnSubscribe que corre la lógica de propagación de los eventos. Por ejemplo, en un Observable a partir
    de un iterable, al subcribir se llama a onNext del subcriptor inmediatamente sucesivamente con todos los elementos. En
    una Operation, se subcribe al Observable fuente un subcriptor especial que encapsula al receptor y que transforma los
    llamados a onNext/onError/onComplete correspondientes.


     */

/*

$S                                          empieza a leer ble
$E                                          deja de leer ble
$R {major} {minor}                          lee el estado de major minor    "report: addr: %d %d, presence: %s, last distance: %d\r\n"
$D                                          borra base de datos
${indice led} {indice cb} {major} {minor}   setea callback


 */

object Channels {

    //Input
    val usbInputChar = PublishSubject<Char>()

    val usbInputLine = usbInputChar
            .bufferUntil { it == '\n'}
            .map { String(it.toCharArray()).trim() }

    //Output
    val usbOutputLine = PublishSubject<String>()

    //Control

    val refreshSerialPorts = PublishSubject<Unit>()

    val changeTrainModelMode = PublishSubject<TrainModelMode>()

    val changeTrainState : Observable<TrainState> =
            usbInputLine.filter{
                it.startsWith(SerialMsgType.CHANGE_TRAIN_STATE.prefix)
            }
            .map {
                TrainState.fromTag(
                        it.substring(SerialMsgType.CHANGE_TRAIN_STATE.prefix.length)
                )
            }

    val changeBarrierState : Observable<BarrierState> =
            usbInputLine.filter{
                it.startsWith(SerialMsgType.CHANGE_BARRIER_STATE.prefix)
            }
            .map {
                BarrierState.fromTag(
                        it.substring(SerialMsgType.CHANGE_BARRIER_STATE.prefix.length)
                )
            }

    init {
        usbOutputLine.let {

            changeTrainModelMode.map { "${'$'}M=${it.id}" }.subscribe(it)

        }

    }

}

object Controller {

    val availableSerialPorts = Channels.refreshSerialPorts.startWith(Unit).map {
        SerialPort.getCommPorts().toList().filterNotNull()
    }

    val activeSerialPortSubject = BehaviorSubject<SerialPort?>(null)

    val activeSerialPort: Observable<SerialPort?> get() = activeSerialPortSubject


    init {

        Channels.usbOutputLine.map { "$it\n" }.subscribe {
            activeSerialPortSubject.value?.writeBytes(it.toByteArray(), it.length.toLong())
        }

    }

    fun setActiveSerialPort(serialPort: SerialPort?) {

        activeSerialPortSubject.value?.removeDataListener()
        activeSerialPortSubject.value?.closePort()

        if (serialPort != null) {

            serialPort.baudRate = 115200

            serialPort.openPort()

            serialPort.onDataRecieved {
                it.forEach {
                    Channels.usbInputChar.onNext(it.toChar())
                }
            }

        }

        activeSerialPortSubject.onNext(serialPort)
    }

}