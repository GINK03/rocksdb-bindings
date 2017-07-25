
libsample.so: main.go sample.cpp
	clang++ sample.cpp -fPIC -shared -o libsample.so -Wc++11-compat-deprecated-writable-strings
	rustc sample_rust.rs --crate-type dylib --crate-name sample_rust
	go build main.go

.PHONY: clean
clean:
	rm main libsample.so libsample_rust.so
