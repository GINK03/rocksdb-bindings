package main

/*
#cgo LDFLAGS: -ldl ./libwakati.so ./libsample_rust.so
#include <dlfcn.h>
#include <stdlib.h>
#include "bridge.h"
*/
import "C"
import (
	_ "compress/gzip"
	"fmt"
	"io/ioutil"
	_ "os"
	"path/filepath"
	"unsafe"
)

func wakati(text string) {
	raw := C.wakati2(C.CString(text))
	_ = C.GoString(raw)
	defer C.free(unsafe.Pointer(raw))
	//fmt.Println(space)
}

func main() {
	/*for i := 0; i < 1000; i++ {
		go wakati("この世で最も美しいもの")
	}*/
	files, _ := filepath.Glob("../../../each/*.plane")
	for _, file := range files {
		b, err := ioutil.ReadFile(file) // just pass the file name
		if err != nil {
			fmt.Print(err)
		}
		str := string(b)
		go wakati(str)
	}
}
