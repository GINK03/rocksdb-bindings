
import org.rocksdb.RocksDB
import org.rocksdb.Options

fun main(args : Array<String>) {
  RocksDB.loadLibrary()
  // DBをなければ作成して開く
  val options = Options().setCreateIfMissing(true)
  val db = RocksDB.open(options, "/tmp/kotlin.rdb")

  // データのput
  val key1 = "key1".toByteArray()
  val value1 = "value1".toByteArray()
  db.put(key1, value1)

  val key2 = "key2".toByteArray()
  val value2 = "value2".toByteArray()
  db.put(key2, value2)

  //データのget
  val bvalue = db.get(key1)
  println(String(bvalue))

  // seek to end
  val iter = db.newIterator()
  iter.seekToFirst()
  while( iter.isValid() ) {
    println("${String(iter.key())} ${String(iter.value())}")
    iter.next()
  }


  // データの削除
  db.del(key1)
  db.del(key2)
}

