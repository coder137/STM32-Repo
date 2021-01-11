- [Getting started](#getting-started)
  - [Links](#links)
  - [Pre-requisites](#pre-requisites)
  - [Running the code](#running-the-code)
- [TODO Running the test](#todo-running-the-test)
- [TODO for code](#todo-for-code)
- [TODO for build script](#todo-for-build-script)
- [TODO for quality of life](#todo-for-quality-of-life)

# Getting started

## Links

- [Cargo binutils](https://github.com/rust-embedded/cargo-binutils)
- [Embedded Rust book](https://doc.rust-lang.org/stable/embedded-book/)
- [Lowlevel Embedded Rust book](https://docs.rust-embedded.org/embedonomicon/)

## Pre-requisites

- `rustup default stable`
- `rustup target add <your_target>`
  - See **.cargo/config.toml** file
- `cargo install cargo-binutils`

## Running the code

- `cargo build`
- Run the code on the target board using the **.vscode/launch.json** configurations
  - These can also be manually run on the target using OpenOCD
  - Requires the **cortex-debug** vscode extension

# TODO Running the test

- `cargo test --target x86_64-pc-windows-msvc`
  - In my case I am running the code on a windows machine
  - This target would be your host operating system
  - Check using `rustup target list`

# TODO for code

- Understand the difference between
  - rust pointer
  - rust reference
  - rust mutable reference
  - rust mutable pointer
  - casting from reference to pointer
  - casting from pointer to reference

# TODO for build script

- Add custom cargo subcommands (for running post-build commands)
- Create crates and deploy
- Understand `std::process::Command` for invoking external commands during pre-build

# TODO for quality of life

- bindgen to convert `.h` files for `.rs` files
  - Other ways in which this can be converted
- Core packages that can be used in embedded rust development
- Using [**cargo make**](https://github.com/sagiegurari/cargo-make)
