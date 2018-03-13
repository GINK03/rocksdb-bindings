# RocksDBをさまざまな言語で利用する

[InstagramのCassandraのバックエンドをJVMベースのものから、RocksDBに切り替えたというニュース](https://engineering.instagram.com/open-sourcing-a-10x-reduction-in-apache-cassandra-tail-latency-d64f86b43589)が少し話題になりました。  

CassandraのJVMは定期的にガーベジコレクタが走って、よろしくないようです。

P999というテストケースではデフォルトのJVMからRocksDBに張り替えたところ10倍近くのパフォーマンスが得られたとのことです。
<div align="center">
  <img width="450px" src="https://cdn-images-1.medium.com/max/1600/1*E-2efj-mMo0dQWEvZyxn1g.png">
</div>

## 前提
- RocksDBはSSDやnvmeで爆速を引き出すパーマネントKVSです  
- LevelDB, RocksDBはPythonで分析するときの必勝パターンに自分のスキルの中に入っているので、ぜひともRocksDBも開拓したい
- RocksDBはC++のインターフェースが美しい形で提供さており、多言語とのBindingが簡単そう

## Pure C++
**注意**  
最新のClangでは構文エラーでコンパイラが通らないので、gcc(g++)を利用する

## Rust

## Kotlin

## Python

