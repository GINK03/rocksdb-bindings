# RustやC++で作った関数をGo言語で利用する

RustもGoも強力な言語なのですが、その資産の再利用を上手に行いたいというモチベーションがあり、  
Go言語からRustでコンパイルされた関数を呼べるようにする例を示します  

この例では、ダイナミックライブラリをRustで作り、Goで呼べるようにします  

## Requirements
- Go( 1.8.3 >= )
- Rustc( 1.15.1 >= ) 
- Clang( 4.0.0 >= )

## Rustのマングル化の停止と、extern "C"オプションの追記
Rustではマングル化という処理が行われて、ダイナミックライブラリが作成されても他のプログラムでは容易に再利用できなくなっています  
そのため、各関数の上にマングル化を行わないようにするアノテーションをつけます
　
 ```rust
 #[no_mangle]
pub extern "C" fn echo_rust_i(x:i32) {
  println!("Hello Rust {}", x);
}
#[no_mangle]
pub extern "C" fn echo_rust_string(x: *mut c_char) {
  let x = unsafe { CStr::from_ptr(x) }; // <- 推奨関数がCStringではなくてCStr関数が良いとのことです
  if let Ok(x) = x.to_str() {
    println!("Hello Rust {}", x );
  }
}   
 ```
なお、GoとRustの文字列のやりとりが厄介で、\*mut c_char型でRustに渡してやる必要があります  

これを入れることで、rustcで作成したダイナミックライブラリをnmコマンドで呼び出せる関数を探すと、echo_rust_iというものが存在するのがわかります  
```console
$ nm libsample_rust.so  | grep echo_rust_i
00000000002df8c8 d _ZN11sample_rust11echo_rust_i15__STATIC_FMTSTR17hb3346aedcb71a8fcE
00000000000529c0 T echo_rust_i
```

## C++は関数定義をextern "C"で囲う
C++もマングル化が行われ、外部のGoなどのプログラムから参照できなくなってしまいます。  
そのため、C++で任意の実装をする際には、ヘッダーファイルを外出ししてこのように記述すると、　Go言語から利用できます  
```cpp
extern "C" {
  void echo();
  void echoC(const char *);
  void echoI(const int);
  const char* wakati2(const char*);
}
```

このような書き方で、　ダイナミックオブジェクトを作成して、nmコマンドで中身を確認すると、このように、echo関数が見える状態になっていることがわかるかと思います  
```console
$ nm libsample.so  | grep echo
0000000000000a40 T echo
0000000000000a80 T echoC
0000000000000ac0 T echoI
```

## Go言語側で呼び出す関数を記述したヘッダーファイルを用意する
GoはC言語との運用が可能なので、C言語の記述法で記述したヘッダーファイルを用意して、関数を認識させます
```c
// From C++
void echo();
void echoC(char *);
void echoI(int);
char* wakati2(char*);

// From Rust
void echo_rust_i(int);
void echo_rust_string(char *);
```
これを*brigh.h*としました

## Go言語側での設定
import "C"をする上の部分に必要なダイナミックリンクの記述と、ヘッダーファイルの読み込みのコメントを書きます  
```go
package main

/*
#cgo LDFLAGS: -ldl ./libsample.so ./libsample_rust.so
#include <dlfcn.h>
#include <stdlib.h>
#include "bridge.h"
*/
import "C"
import "fmt"

func main() {
        C.echo()
        C.echoC(C.CString("眠い"))
        C.echoI(1234)
        C.echo_rust_i(3210)
        C.echo_rust_string(C.CString("ねむすぎる"))
        fmt.Println("result as", C.rust_multiply(2, 3))
        
        raw := C.wakati2(C.CString(text))
        _ = C.GoString(raw)
        defer C.free(unsafe.Pointer(raw)) // <- Rustや　C++で呼び出された関数に関しては、メモリが解放されないので、明示的にfreeする必要がある
}
```
このようにすることで、Goから文字列を渡したり、数字を渡したりして結果を受け取るとうことが可能になります　　
Goのデータ構造にさっさとコピーして、渡ってきたデータをfreeしないと、予期しないメモリーリークの元になります  

## サンプルのコンパイルの仕方

```cosnole
$ make
```
mainというファイルができますので、実行してください　　　

# benchmark. goroutineを利用した並列形態素解析
Go言語の強さの一端は、Goroutineという並列処理を行うことで、どの程度、形態素解析が高速化されるか検証する  
比較として、Python3のMeCabと今回作成した、Goの分かち書きで、160万の日本語のニュースを分かち書きした際に、どの程度の速度の差がでるか  

```console
$ time python3 benchmark.py 
real    0m6.761s
user    0m6.288s
sys     0m0.468s
```

```console
$ cd misc.mecab-binding
$ make
$ time ./main 
real    0m6.572s
user    0m6.288s
sys     0m0.544s
```
ほとんど変わらないし、速度的な側面で言えば、Golangで形態素解析する意味はないように見える

## Appendix
Rust内部でのc_charの取り回し、クッソめんどくさいので、Rust内部でのリテラルの文字列を\*mut c_charに変換するのは、マクロか、関数化しておくといいかもしれない  
```rust
　　let x_ptr = CString::new("world").unwrap().as_ptr();
　　echo_rust_string(x_ptr);
```
なお、以下のやり方だと、メモリが関数を抜けると同時に解放されてしまい、うまくいかないらしい
```rust
let x_ptr = CString::new("world").unwrap().as_ptr();
echo_rust_string(x_ptr); // 呼び出す前に一時オブジェクトが破棄されるためポインタの指す値は無効
```
書き方でメモリが解放されるか、されないかが決まるらしいので、要注意


## 参考文献
[1] [Rustでリテラルの文字を*mut c_char型する方法](https://teratail.com/questions/85658#reply-134128)
