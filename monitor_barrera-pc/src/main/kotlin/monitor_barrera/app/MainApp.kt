package monitor_barrera.app

import javafx.scene.image.Image
import tornadofx.*
import java.util.Scanner

import java.awt.event.KeyAdapter
import java.awt.event.KeyEvent
import javax.swing.JFrame
import javax.swing.SwingUtilities

class MainApp : App(MainView::class) {

    init {
        //addStageIcon(Image("/app/logo.jpeg"))


    }

    override fun stop() {
        super.stop()

        Controller.setActiveSerialPort(null)

    }
}
