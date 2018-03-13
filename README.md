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

最新のClangでは構文エラーでコンパイラが通らないので、gcc(g++ > 7.2.0)を利用必要があります  

## C++bindings
C/C++でラッパーを書くことで任意のCのshared objectが利用できる言語とバインディングを行うことができます。  

extern "C"で囲んだ範囲が外部のプログラムで見える関数になります。
```cpp
extern "C" {
  void helloDB(const char* dbname);
  int putDB(const char* dbname, const char* key, const char* value);
  int getDB(const char* dbname, const char* key, char* value);
  int delDB(const char* dbname, const char* key);
  int keysDB(const char* dbname, char* keys);
}
```
サンプルのshared objectを作成するコードを用意したので、参考にしていただけると幸いです。
```console
$ cd cpp-shared
$ make 
$ ls librocks.so
$ ldd librocks.so
        linux-vdso.so.1 =>  (0x00007fff04ccd000)
        librocksdb.so.5 => /usr/lib/x86_64-linux-gnu/librocksdb.so.5 (0x00007fdaf33ab000)
        libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fdaf3025000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fdaf2e0e000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fdaf2a2e000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fdaf280f000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fdaf24b9000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fdaf3e77000)
```

## Rust
RustではC++のバインディングを利用してRocksDBにデータを格納したり取り出したりする方法を示します。  

サンプルコードを動作させるには、以下のようにterminalを操作します。　　
```console
$ cd rust
$ export LD_LIBRARY_PATH=../cpp-shared/:$LD_LIBRARY_PATH
$ make
$ ./sample
```


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

