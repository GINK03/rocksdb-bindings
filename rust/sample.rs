
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

#[link(name = "rocks")]
extern {
  fn hello() -> ();
}

pub struct Rocks {
  pub dbName:String,
  pub cursol:i32,
}
impl Rocks {
  pub fn new( dbName:&str) -> Rocks {
    Rocks{ dbName:dbName.to_string(), cursol:0 }
  }
}


fn main() {
  println!("hello world");
  let rocks = Rocks::new( "rocks.rdb" );
  unsafe { hello() };
}
