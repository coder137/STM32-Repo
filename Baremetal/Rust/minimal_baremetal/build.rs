//! This build script copies the `memory.x` file from the crate root into
//! a directory where the linker can always find it at build time.
//! For many projects this is optional, as the linker always searches the
//! project root directory -- wherever `Cargo.toml` is. However, if you
//! are using a workspace or have a more complicated build setup, this
//! build script becomes required. Additionally, by requesting that
//! Cargo re-run the build script whenever `memory.x` is changed,
//! updating `memory.x` ensures a rebuild of the application with the
//! new memory settings.

use std::env;
use std::path::PathBuf;

use std::process::Command;

fn reference() {
    let out: &PathBuf = &PathBuf::from(env::var_os("OUT_DIR").unwrap());
    println!("cargo:warning={:?}", out.display());
}

fn linker_script() {
    println!("cargo:rerun-if-changed=gcc_arm.ld");
}

fn c_library_build() {
    const CMAKE: &str = "cmake";
    const C_FOLDER: &str = "libraries";
    const C_BUILD_FOLDER: &str = "libraries/build";
    const C_LIBRARIES: [&str; 1] = ["device"];

    println!("cargo:rerun-if-changed={}/CMakeLists.txt", C_FOLDER);
    println!("cargo:rerun-if-changed={}", C_BUILD_FOLDER);

    let c_lib_build = Command::new(CMAKE)
        .args(&["-S", C_FOLDER, "-B", C_BUILD_FOLDER, "-G", "Ninja"])
        .output()
        .unwrap();
    if c_lib_build.status.success() {
        println!(
            "cargo:warning={:?}",
            String::from_utf8(c_lib_build.stdout).unwrap()
        );
    } else {
        println!(
            "cargo:warning={:?}",
            String::from_utf8(c_lib_build.stderr).unwrap()
        );
    }

    // Compile the library
    let c_lib_build = Command::new(CMAKE)
        .args(&["--build", C_BUILD_FOLDER])
        .output()
        .unwrap();
    if c_lib_build.status.success() {
        println!(
            "cargo:warning={:?}",
            String::from_utf8(c_lib_build.stdout).unwrap()
        );
    } else {
        println!(
            "cargo:warning={:?}",
            String::from_utf8(c_lib_build.stderr).unwrap()
        );
    }

    // This is where the library is installed
    println!("cargo:rustc-link-search=all={}", C_BUILD_FOLDER);
    for lib in C_LIBRARIES.iter() {
        println!("cargo:rustc-link-lib=static={}", lib);
    }
}

fn main() {
    reference();
    linker_script();
    c_library_build();
}
