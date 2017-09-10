package monitor_barrera.app

import javafx.scene.image.Image
import tornadofx.*

class MainApp : App(MainView::class) {

    init {
        addStageIcon(Image("/app/logo.jpeg"))
    }

    override fun stop() {
        super.stop()

        Controller.setActiveSerialPort(null)

    }
}
