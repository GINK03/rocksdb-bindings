package main

/*
#cgo LDFLAGS: -ldl ./libwakati.so ./libsample_rust.so
#include <dlfcn.h>
#include <stdlib.h>
#include "bridge.h"
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func wakati(text string) {
	raw := C.wakati2(C.CString(text))
	spaced := C.GoString(raw)
	defer C.free(unsafe.Pointer(raw))
	fmt.Println(spaced)
}

func main() {
	for i := 0; i < 1000; i++ {
		go wakati("この世で最も美しいもの")
	}
}
