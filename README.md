# Rustで作った関数をGo言語で利用する

RustもGoも強力な言語なのですが、その資産の再利用を上手に行いたいというモチベーションがあり、  
Go言語からRustでコンパイルされた関数を呼べるようにする例を示します  

この例では、ダイナミックライブラリをRustで作り、Goで呼べるようにします  

## Requirements
- Go( 1.8.3 >= )
- Rustc( 1.15.1 >= ) 

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
  let x = unsafe { CString::from_raw(x).into_string().unwrap() };                                                                                                                                    
  println!("Hello Rust {}", x );                                                                                                                                                                     
}   
 ```
なお、GoとRustの文字列のやりとりが厄介で、\*mut c_char型でRustに渡してやる必要があります  

これを入れることで、rustcで作成したダイナミックライブラリをnmコマンドで呼び出せる関数を探すと、echo_rust_iというものが存在するのがわかります  
```console
$ nm libsample_rust.so  | grep echo_rust_i
00000000002df8c8 d _ZN11sample_rust11echo_rust_i15__STATIC_FMTSTR17hb3346aedcb71a8fcE
00000000000529c0 T echo_rust_i
```

## Go言語側で呼び出す関数を記述したヘッダーファイルを用意する
GoはC言語との運用が可能なので、C言語の記述法で記述したヘッダーファイルを用意して、関数を認識させます
```c
// From C++
void echo();
void echoC(char *);
void echoI(int);

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
}
```
このようにすることで、Goから文字列を渡したり、数字を渡したりして結果を受け取るとうことが可能になります　　

## サンプルのコンパイルの仕方

```cosnole
$ make
```
mainというファイルができますので、実行してください　　　

