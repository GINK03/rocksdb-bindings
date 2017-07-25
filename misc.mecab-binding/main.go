package main

/*
#cgo LDFLAGS: -ldl ./libwakati.so ./libsample_rust.so
#include <dlfcn.h>
#include "bridge.h"
*/
import "C"
import "fmt"

func main() {
	//C.echo()
	//C.echoC(C.CString("眠い"))
	raw := C.wakati(C.CString("この世で最も眠い"))
	wakati := C.GoString(raw)

	fmt.Println(wakati)

	raw = C.wakati(C.CString("　ブームのきっかけは､5000兆円が欲しい絵師のケー・スワベ氏がpixivやTwitterで公開したこの5000兆円が欲しい!と思いたくなる画像である。 "))
	wakati = C.GoString(raw)

	fmt.Println(wakati)

	C.echo_rust_i(3210)

	C.echo_rust_string(C.CString("ねむすぎる"))

	fmt.Println("result as", C.rust_multiply(2, 3))
}
