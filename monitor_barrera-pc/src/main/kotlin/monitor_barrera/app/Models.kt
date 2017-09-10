package monitor_barrera.app

import javafx.beans.property.SimpleStringProperty
import tornadofx.setValue
import tornadofx.getValue

enum class TrainModelMode(val id:Int) {
    NORMAL(0),
    SAFE_FAIL(1),
    UNSAFE_FAIL(2);

    companion object {
        private val map = TrainModelMode.values().associateBy(TrainModelMode::id)
        fun fromId(id: Int) = map[id]
    }
}

enum class BarrierState(val id: Int, val tag: String) {
    NO_TRAIN(0,"sin tren"),
    TRAIN_FROM_LEFT(1,"tren desde la izquierda"),
    TRAIN_FROM_RIGHT(2,"tren desde la derecha");

    companion object {
        private val idMap = BarrierState.values().associateBy(BarrierState::id)
        private val tagMap = BarrierState.values().associateBy(BarrierState::tag)
        fun fromId(id: Int) = idMap[id]
        fun fromTag(tag: String) = tagMap[tag]
    }
}

enum class TrainState(val id: Int, val tag: String) {
    ACTIVATED(0,"activado"),
    DEACTIVATED(1,"desactivado");

    companion object {
        private val idMap = TrainState.values().associateBy(TrainState::id)
        private val tagMap = TrainState.values().associateBy(TrainState::tag)
        fun fromId(id: Int) = idMap[id]
        fun fromTag(tag: String) = tagMap[tag]
    }
}

enum class SerialMsgType(val id: Int, val prefix: String) {
    CHANGE_BARRIER_STATE(0,"estado barrera: "),
    CHANGE_TRAIN_STATE(1,"estado tren: ");

    companion object {
        private val idMap = SerialMsgType.values().associateBy(SerialMsgType::id)
        private val prefixMap = SerialMsgType.values().associateBy(SerialMsgType::prefix)
        fun fromId(id: Int) = idMap[id]
        fun fromPrefix(tag: String) = prefixMap[tag]
    }
}

enum class TrackState(val id: Int, val tag: String) {
    FREE(0,"Desocupada"),
    OCCUPIED(1,"Ocupada");

    companion object {
        val propertyName = "Estado de la vía"
    }
}

enum class ArmPositionState(val id: Int, val tag: String) {
    DOWN(0,"Abajo"),
    UP(1,"Arriba"),
    TRANSITION(2,"Transición");
    companion object {
        val propertyName = "Posición del brazo"
    }
}

enum class ArmState(val id: Int, val tag: String) {
    HEALTHY(0,"Integro"),
    BROKEN(1,"Roto");
    companion object {
        val propertyName = "Estado del brazo"
    }
}

enum class LightState(val id: Int, val tag: String) {
    HEALTHY(0,"Operativo"),
    BROKEN(1,"Mal funcionamiento");
    companion object {
        val propertyName = "Estado del semaforo"
    }
}

enum class RepairAction(val id: Int, val tag: String) {
    SAFE_FAIL(0,"Revisar el mecanismo"),
    UNSAFE_FAIL(1,"Revisar el mecanismo de manera urgente");
    companion object {
        val propertyName = "Acción que debe realizarse:"
    }
}


class BarrierMonitorParam(name: String? = null, value: String? = null){

    val nameProperty = SimpleStringProperty(this, "name", name)
    var name by nameProperty

    val valueProperty = SimpleStringProperty(this, "value", value)
    var value by valueProperty

}


/*
enum class BeaconPresenceState(val tag: String) {
    PRESENT("present"),
    MISSING("missing"),
    LOST("lost");

    companion object {
        private val map = BeaconPresenceState.values().associateBy(BeaconPresenceState::tag)
        fun fromTag(tag: String) = map[tag]
    }
}

enum class AccessControlLed(val id: Int) {
    LED_RGB(0),
    LED_1(1),
    LED_2(2),
    LED_3(3)
}

enum class AccessControlMode(val id: Int) {
    PRESENCE_MODE(0),
    DISTANCE_MODE(1)
}

data class BeaconAddr(val major: Int = -1,val minor:Int = -1)

data class BeaconState(val addr: BeaconAddr, val presence: BeaconPresenceState, val distance: Int)

data class BeaconControlMode(val addr: BeaconAddr, val led: AccessControlLed, val mode: AccessControlMode)

private val BEACON_STATE_REGEX = """report: addr: ([0-9]+) ([0-9]+), presence: ([a-z]+), last distance: ([0-9]+)""".toRegex()

//TODO revisar
fun String.toBeaconState() : BeaconState {
    val data = BEACON_STATE_REGEX.matchEntire(this)?.groups
    return data.let {
        BeaconState(
                BeaconAddr(
                        data?.get(1)?.value?.toIntOrNull() ?: -1,
                        data?.get(2)?.value?.toIntOrNull() ?: -1),
                data?.get(2)?.value?.let { BeaconPresenceState.fromTag(it) } ?: BeaconPresenceState.MISSING,
                data?.get(2)?.value?.toIntOrNull() ?: -1
        )
    }
}
*/
