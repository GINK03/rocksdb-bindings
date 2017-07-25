
libwakati.so: main.go wakachi.cpp
	clang++ wakachi.cpp -fPIC -shared -o libwakati.so -std=c++1z -L/usr/lib -lmecab -lstdc++ -I/usr/include -Wc++11-compat-deprecated-writable-strings
	rustc sample_rust.rs --crate-type dylib --crate-name sample_rust
	go build main.go

.PHONY: clean
clean:
	rm main libwakati.so libsample_rust.so
