package main

/*
#cgo LDFLAGS: -ldl ./libsample.so ./libsample_rust.so
#include <stdlib.h>
#include <dlfcn.h>
#include "bridge.h"
*/
import "C"
import "unsafe"
import "fmt"

func main() {
	C.echo()
	C.echoC(C.CString("眠い"))
	C.echoI(1234)

	C.echo_rust_i(3210)

	// これだとメモリが自動で解放されないから気をつける
	//C.echo_rust_string( C.CString("ねむすぎる"))
	s := C.CString("本日は晴天なり")
	C.echo_rust_string(s)
	C.free(unsafe.Pointer(s))

	fmt.Println("result as", C.rust_multiply(2, 3))
}
