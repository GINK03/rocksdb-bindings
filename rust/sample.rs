
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;
use std::io::{BufReader, BufWriter};
use std::collections::btree_map;
use std::collections::HashMap;
use std::collections::hash_map::Entry::{Occupied, Vacant};
use std::ops::Shr;
use std::fmt::Debug;
use std::fmt::Display;
use std::ops::{Add};
use std::cmp::{Eq, Ord, PartialOrd};
use std::collections::HashSet;
use std::marker::Copy;
use std::os::raw::{c_char};
use std::ffi::CStr;

#[link(name = "rocks")]
extern {
  fn helloDB(name:*const c_char) -> ();
  fn putDB(dbName:*const c_char, key:*const c_char, value:*const c_char) -> i32;
  fn getDB(dbName:*const c_char, key:*const c_char, value:*mut c_char) -> i32;
  
}

pub struct Rocks {
  pub dbName:String,
  pub cursol:i32,
}
impl Rocks {
  pub fn new( dbName:&str) -> Rocks {
    let outName = format!("{}\0", dbName);
    unsafe { helloDB( outName.as_ptr() as *const c_char) };
    Rocks{ dbName:outName.to_string(), cursol:0 }
  }
}
impl Rocks {
  pub fn put(&self, key:&str, value:&str) -> i32 {
    let dbName = format!("{}\0", &*(self.dbName));
    let key = format!("{}\0",  key);
    let value = format!("{}\0", value);
    println!("{}", self.dbName);
    println!("{}", key);
    let sub = unsafe { putDB( (&*dbName).as_ptr() as *const c_char, key.as_ptr() as *const c_char, value.as_ptr() as *const c_char) };
    println!("{}",sub);
    sub
  }
}
type arr_type = [c_char; 2000];
impl Rocks {
  pub fn get(&self, key:&str) -> (i32,String) {
    let dbName = format!("{}\0", &*(self.dbName));
    let key = format!("{}\0",  key);
    println!("{}", self.dbName);
    println!("{}", key);
    let mut value : arr_type = [0; 2000]; 
    let sub = unsafe { getDB( (&*dbName).as_ptr() as *const c_char, key.as_ptr() as *const c_char, value.as_ptr() as *mut c_char) };
    let c_str: &CStr = unsafe { CStr::from_ptr(value.as_ptr()) };
    let str_slice: &str = c_str.to_str().unwrap();
    let str_buf: String = str_slice.to_owned();
    println!("{}",sub);

    println!("{}",str_buf);
    (sub,str_buf)
  }
}

fn main() {
  println!("hello world");
  let rocks = Rocks::new( "rocks.rdb" );
  rocks.put("some-key", "some-value");
  let (status, value) = rocks.get("some-key");
  println!("p {}", value);
}
