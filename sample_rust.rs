use std::os::raw::{c_char};
use std::ffi::CString;

#[no_mangle]
pub extern "C" fn echo_rust_i(x:i32) {
  println!("Hello Rust {}", x);
}

#[no_mangle]
pub extern "C" fn echo_rust_string(x: *mut c_char) {
  let x = unsafe { CString::from_raw(x).into_string().unwrap() };
  println!("Hello Rust {}", x );
}

#[no_mangle]
pub extern "C" fn rust_multiply(x:i32, y:i32) -> i32 {
  x*y
}

fn main() {
  println!("hello world");
  echo_rust_i(123);
  //echo_rust_string( b"Test\0");
}
