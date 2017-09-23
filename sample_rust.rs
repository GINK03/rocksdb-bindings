use std::os::raw::{c_char};
use std::ffi::CString;
use std::ffi::CStr;

#[no_mangle]
pub extern "C" fn echo_rust_i(x:i32) {
  println!("Hello Rust {}", x);
}

#[no_mangle]
pub extern "C" fn echo_rust_string(x: *const c_char) {
  let x = unsafe { CStr::from_ptr(x).to_str() };
  let x = x.unwrap();
  println!("Hello Rust {}", x );
}

#[no_mangle]
pub extern "C" fn rust_multiply(x:i32, y:i32) -> i32 {
  x*y
}

fn main() {
  println!("hello world");
  echo_rust_i(123);

  let xin = concat!( "world", "\0");
  let xin = xin.as_ptr() as *const c_char;
  echo_rust_string( xin );
}
