package monitor_barrera.app

import com.fazecast.jSerialComm.SerialPort
import javafx.collections.FXCollections
import javafx.scene.control.ListCell
import javafx.scene.control.SelectionMode
import rx.Subscription
import rx.javafx.kt.actionEvents
import tornadofx.*

class PortDialog: Fragment() {

    val availableSerialPorts = FXCollections.observableArrayList<SerialPort>()

    val serialPortSubscription: Subscription

    override val root = form {

        title = "Seleccionar puerto"

        val listView = listview(availableSerialPorts) {

            setCellFactory {
                object: ListCell<SerialPort>() {
                    override fun updateItem(item: SerialPort?, empty: Boolean) {
                        super.updateItem(item, empty)
                        if (item != null) {
                            graphic = text(item.descriptivePortName)
                        }
                    }
                }
            }

            onUserSelect {
                selectSerialPort(it)
            }

            maxHeight = 100.0
            prefWidth = 200.0

            selectionModel.selectionMode = SelectionMode.SINGLE

        }

        hbox {

            button("Seleccionar") {

                setOnAction {

                    listView.selectionModel.selectedItem.let {
                        if (it != null) {
                            selectSerialPort(it)
                        } else {
                            println("Ningun puerto Seleccionado")
                        }
                    }

                }

            }

            button("Recargar") {

                actionEvents().map { Unit }.subscribe(Channels.refreshSerialPorts)

            }

        }

    }

    init {

        Channels.refreshSerialPorts.onNext(Unit)

        serialPortSubscription = Controller.availableSerialPorts.subscribe { availableSerialPorts.setAll(it) }

    }

    fun selectSerialPort(port: SerialPort?) {
        Controller.setActiveSerialPort(port)
        close()
    }

    override fun onUndock() {
        super.onUndock()
        serialPortSubscription.unsubscribe()
    }

}