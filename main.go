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
