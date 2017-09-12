package monitor_barrera.app

import javafx.beans.property.SimpleStringProperty
import javafx.collections.FXCollections
import javafx.collections.ObservableList
import javafx.event.EventHandler
import javafx.geometry.Pos
import javafx.scene.control.Button
import javafx.scene.control.Label
import javafx.scene.layout.BorderPane
import javafx.scene.layout.Priority
import javafx.scene.paint.Color
import javafx.scene.text.Font
import rx.javafx.kt.actionEvents
import tornadofx.*
import java.awt.event.KeyAdapter
import java.awt.event.KeyEvent
import javafx.scene.input.KeyCode



class MainView : View() {
    override val root = BorderPane()

    private val trackOccupationProp = BarrierMonitorParam(TrackState.propertyName, TrackState.FREE.tag)
    private val armPositionProp = BarrierMonitorParam(ArmPositionState.propertyName, ArmPositionState.UP.tag)
    private val armStateProp = BarrierMonitorParam(ArmState.propertyName, ArmState.HEALTHY.tag)
    private val lightStateProp = BarrierMonitorParam(LightState.propertyName, LightState.HEALTHY.tag)
    private val tableData : ObservableList<BarrierMonitorParam> = FXCollections.observableArrayList(
            //BarrierMonitorParam("Temperatura ambiente","26º7"),
            //BarrierMonitorParam("Temperatura del motor","45º3"),
            //BarrierMonitorParam("Corriente del motor","4 Amper"),
            trackOccupationProp,
            armPositionProp//,
            //armStateProp,
            //lightStateProp
    )
    private lateinit var normalOperationButton : Button
    private lateinit var safeErrorOperationButton : Button
    private lateinit var unsafeErrorOperationButton : Button
    private lateinit var actionLabel : Label

    private var actionText = SimpleStringProperty()
    private var trainModelMode = TrainModelMode.NORMAL
    private var barrierState = BarrierState.NO_TRAIN
    private var trainState = TrainState.ACTIVATED

    private fun refreshVisualData(){

        when(trainModelMode){
            TrainModelMode.NORMAL -> {
                when(barrierState){
                    BarrierState.NO_TRAIN -> {
                        trackOccupationProp.value = TrackState.FREE.tag
                        armPositionProp.value = ArmPositionState.UP.tag
                    }
                    BarrierState.TRAIN_FROM_LEFT ->  {
                        trackOccupationProp.value = TrackState.OCCUPIED.tag
                        armPositionProp.value = ArmPositionState.DOWN.tag
                    }
                    BarrierState.TRAIN_FROM_RIGHT -> {
                        trackOccupationProp.value = TrackState.OCCUPIED.tag
                        armPositionProp.value = ArmPositionState.DOWN.tag
                    }
                }

                normalOperationButton.style(true){
                    borderWidth += box(3.px)
                    padding = box(1.px)
                }
                safeErrorOperationButton.style(true){
                    borderWidth += box(1.px)
                    padding = box(3.px)
                }
                unsafeErrorOperationButton.style(true){
                    borderWidth += box(1.px)
                    padding = box(3.px)
                }
            }
            TrainModelMode.SAFE_FAIL -> {
                armPositionProp.value = ArmPositionState.DOWN.tag
                when(barrierState){
                    BarrierState.NO_TRAIN -> {
                        trackOccupationProp.value = TrackState.FREE.tag

                        actionText.value = RepairAction.SAFE_FAIL.tag
                        actionLabel.style(true){
                            backgroundColor += Color.YELLOW
                        }

                    }
                    BarrierState.TRAIN_FROM_LEFT ->  {
                        trackOccupationProp.value = TrackState.OCCUPIED.tag
                    }
                    BarrierState.TRAIN_FROM_RIGHT -> {
                        trackOccupationProp.value = TrackState.OCCUPIED.tag
                    }
                }

                normalOperationButton.style(true){
                    borderWidth += box(1.px)
                    padding = box(3.px)
                }
                safeErrorOperationButton.style(true){
                    borderWidth += box(3.px)
                    padding = box(1.px)
                }
                unsafeErrorOperationButton.style(true){
                    borderWidth += box(1.px)
                    padding = box(3.px)
                }
            }
            TrainModelMode.UNSAFE_FAIL -> {
                armPositionProp.value = ArmPositionState.UP.tag
                when(barrierState){
                    BarrierState.NO_TRAIN -> {
                        trackOccupationProp.value = TrackState.FREE.tag
                    }
                    BarrierState.TRAIN_FROM_LEFT ->  {
                        trackOccupationProp.value = TrackState.OCCUPIED.tag

                        actionText.value = RepairAction.UNSAFE_FAIL.tag
                        actionLabel.style(true){
                            backgroundColor += Color.RED
                        }
                    }
                    BarrierState.TRAIN_FROM_RIGHT -> {
                        trackOccupationProp.value = TrackState.OCCUPIED.tag

                        actionText.value = RepairAction.UNSAFE_FAIL.tag
                        actionLabel.style(true){
                            backgroundColor += Color.RED
                        }
                    }
                }

                normalOperationButton.style(true){
                    borderWidth += box(1.px)
                    padding = box(3.px)
                }
                safeErrorOperationButton.style(true){
                    borderWidth += box(1.px)
                    padding = box(3.px)
                }
                unsafeErrorOperationButton.style(true){
                    borderWidth += box(3.px)
                    padding = box(1.px)
                }
            }
        }
    }

    init {

        shortcut("Ctrl+Z") {
            (SerialMsgType.CHANGE_BARRIER_STATE.prefix + BarrierState.TRAIN_FROM_LEFT.tag + "\r\n").forEach { Channels.usbInputChar.onNext(it) }
        }

        shortcut("Ctrl+X") {
            (SerialMsgType.CHANGE_BARRIER_STATE.prefix + BarrierState.NO_TRAIN.tag + "\r\n").forEach { Channels.usbInputChar.onNext(it) }
        }

        shortcut("Ctrl+C") {
            (SerialMsgType.CHANGE_BARRIER_STATE.prefix + BarrierState.TRAIN_FROM_RIGHT.tag + "\r\n").forEach { Channels.usbInputChar.onNext(it) }
        }

        title = "Monitor de barrera"

        Channels.changeTrainModelMode.subscribe{

            trainModelMode = it

            runAsync {
            } ui {
                actionText.value = ""
                actionLabel.style(true){
                    backgroundColor += Color.GREY
                }

                refreshVisualData()
            }
        }

        Channels.changeBarrierState.subscribe{
            barrierState = it
            runAsync {
            } ui {
                refreshVisualData()
            }
        }


        Channels.changeTrainState.subscribe{
            trainState = it
            /*
            when(it){
                TrainState.ACTIVATED -> TODO()
                TrainState.DEACTIVATED -> TODO()
                else -> TODO()
            }*/
        }

        Controller.activeSerialPort.filter{it != null}.map { trainModelMode }.subscribe(Channels.changeTrainModelMode)

        with(root) {

            prefWidth = 1200.0


            top = menubar {

                menu("Opciones") {

                    item("Conectar") {

                        setOnAction {

                            find(PortDialog::class).openModal()

                        }

                    }

                }

            }


            center = form {

                vbox(20) {

                    hbox(spacing=20, alignment = Pos.CENTER) {

                        vgrow = Priority.ALWAYS
                        useMaxSize = true


                        button("Funcionamiento normal") {
                            normalOperationButton = this
                            useMaxSize = true
                            hgrow = Priority.ALWAYS
                            font = Font(28.0)
                            actionEvents().map { TrainModelMode.NORMAL }.subscribe(Channels.changeTrainModelMode)

                            style {
                                backgroundColor += Color.LIGHTGREEN
                                borderWidth += box(1.px)
                                borderColor += box(Color.BLACK)
                            }
                        }

                        button("Falla segura") {
                            safeErrorOperationButton = this
                            useMaxSize = true
                            hgrow = Priority.ALWAYS
                            font = Font(28.0)
                            actionEvents().map { TrainModelMode.SAFE_FAIL }.subscribe(Channels.changeTrainModelMode)

                            style {
                                backgroundColor += Color.YELLOW
                                borderWidth += box(1.px)
                                borderColor += box(Color.BLACK)
                            }
                        }

                        button("Falla critica") {
                            unsafeErrorOperationButton = this
                            useMaxSize = true
                            hgrow = Priority.ALWAYS
                            font = Font(28.0)
                            actionEvents().map { TrainModelMode.UNSAFE_FAIL }.subscribe(Channels.changeTrainModelMode)
                            style {
                                backgroundColor += Color.RED
                                borderWidth += box(1.px)
                                borderColor += box(Color.BLACK)
                            }
                        }
                    }

                    tableview(tableData) {
                        useMaxSize = true
                        vgrow = Priority.SOMETIMES
                        alignment = Pos.CENTER
                        column("Parametro", BarrierMonitorParam::nameProperty)
                        column("Valor", BarrierMonitorParam::valueProperty)
                        columnResizePolicy = SmartResize.POLICY
                        style {
                            font = Font(40.0)
                            prefRowCount = 7
                        }
                    }

                    hbox(20) {

                        alignment = Pos.BOTTOM_CENTER

                        label(RepairAction.propertyName) {
                            style {
                                font = Font(25.0)
                            }
                        }

                        label("") {
                            actionLabel = this
                            style {
                                font = Font(30.0)
                                backgroundColor += Color.YELLOW
                            }
                            bind(actionText)
                        }

                    }
                }

            }

        }

        refreshVisualData()

    }

}

/*
fun colorText(string: String, color: Color): Text {
    return Text().apply {
        text = string
        fill = color
    }
}

fun TextFlow.appendText(string: String, color: Color = Color.BLACK) {
    children.add(colorText(string, color))
}*/