package monitor_barrera.app

import rx.Observable
import rx.Subscriber
import rx.functions.Func1
import java.util.*

fun <T> Observable<T>.bufferUntil(boundaryPredicate: (T) -> Boolean): Observable<List<T>> =
        lift(BufferUntil(Func1 { boundaryPredicate(it) }))

fun <T> Observable<T>.bufferUntil(boundaryPredicate: Func1<T, Boolean>): Observable<List<T>> =
        lift(BufferUntil(boundaryPredicate))

class BufferUntil<T>(internal val boundaryPredicate: Func1<T, Boolean>) : Observable.Operator<List<T>, T> {

    override fun call(child: Subscriber<in List<T>>): Subscriber<in T> {
        val parent = BufferWhileSubscriber(child)
        child.add(parent)
        return parent
    }

    internal inner class BufferWhileSubscriber(val actual: Subscriber<in List<T>>) : Subscriber<T>() {

        var buffer: MutableList<T> = ArrayList()

        override fun onNext(t: T) {
            buffer.add(t)
            if (boundaryPredicate.call(t)) {
                actual.onNext(buffer)
                buffer = ArrayList<T>()
            }
        }

        override fun onError(e: Throwable) {
            actual.onError(e)
        }

        override fun onCompleted() {
            if (!buffer.isEmpty()) {
                actual.onNext(buffer)
            }
            actual.onCompleted()
        }
    }
}