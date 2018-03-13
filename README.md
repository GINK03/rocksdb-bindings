# RocksDBをさまざまな言語で利用する

[InstagramのCassandraのバックエンドをJVMベースのものから、RocksDBに切り替えたというニュース](https://engineering.instagram.com/open-sourcing-a-10x-reduction-in-apache-cassandra-tail-latency-d64f86b43589)が少し話題になりました。  

CassandraのJVMは定期的にガーベジコレクタが走って、よろしくないようです。

P99というテストケースではデフォルトのJVMからRocksDBに張り替えたところ10倍近くのパフォーマンスが得られたとのことです。
<div align="center">
  <img width="450px" src="https://cdn-images-1.medium.com/max/1600/1*E-2efj-mMo0dQWEvZyxn1g.png">
</div>

データ分析でもメモリ収まりきらないけど、Sparkのような分散システムを本格に用意する必要がない場合、NVMe上にLevelDBなどのKVSを用意して加工することがあります。  
ローカルで動作させるには最強の速度だし、文句のつけようもない感じです。  


## 前提
- RocksDBはSSDやnvmeで爆速を引き出すパーマネントKVSです  
- LevelDB, RocksDBはPythonで分析するときの必勝パターンに自分のスキルの中に入っているので、ぜひともRocksDBも開拓したい
- RocksDBはC++のインターフェースが美しい形で提供さており、他言語とのBindingが簡単そう

## RocksDBのインストール
Ubuntuですと標準レポジトリにないので、ビルドしてインストールする必要があります  
```console
$ git clone git@github.com:facebook/rocksdb.git
$ cd rocksdb
$ mkdir build
$ cd build
$ cmake ..
$ make -j12
$ sudo make install
```

## Pure C++
**注意**  
最新のClangでは構文エラーでコンパイラが通らないので、gcc(g++)を利用必要があります  

## C++bindings
C/C++でラッパーを書くことで任意のCのshared objectが利用できる言語とバインディングを行うことができます。  
```cpp
```
## Rust


## Kotlin
Kotlin, JavaではGradleに追加することで簡単に利用可能になります。  
```build.gradle
compile group: 'org.rocksdb', name: 'rocksdbjni', version: '5.10.3'
```
Interfaceも整理されており、以下のように簡単に、put, get, iterate, deleteが行えます  
```kotlin
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
  db.put(keygetvalue2)
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
```
## Python

