- [Unit Testing and Mocking](#unit-testing-and-mocking)
  - [Configuring your Unit-Testing project](#configuring-your-unit-testing-project)
  - [Running the Unit Tests](#running-the-unit-tests)
- [Adding your custom Unit-Test](#adding-your-custom-unit-test)
  - [Writing your mocks](#writing-your-mocks)
  - [Writing your tests](#writing-your-tests)
- [IMPROVEMENTS](#improvements)

# Unit Testing and Mocking

- CMake can only have one compiler stored in its Cache
- When building your ARM project you cannot have your native compiler testing as well


## Configuring your Unit-Testing project

- Delete your build folder
- In the **root level CMakeLists.txt** set your **TESTING** flag to **ON**
- Reconfigure your project using the below command

```cmake
cmake -B build -G Ninja -D CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE
```

## Running the Unit Tests

- From the build directory
```
ctest -T test --output-on-failure
```

# Adding your custom Unit-Test

Add your own unit-tests on the HAL layer and above

- NOTE: Low level register code cannot be and **should not be** mocked since they are writing directly to hardware
- Your native compiler will throw a segmentation fault since you are using / reading and writing to memory addresses that are not permitted by your exectuable

## Writing your mocks

**See driver/gpio/mock**

- [Read FFF docs](https://github.com/meekrosoft/fff)
- To your dependent code create a header file and write your mocks there.
  - This is done to avoid code duplication
  - Later modules can easily reuse this interface
- Include your include dependencies and make them have a **PUBLIC SCOPE**
  - These will need to be used by Unit-Test executables
- Update the root level CMakeLists.txt
  - Under the `TESTING == TRUE` condition
- These mocks will be CMAKE INTERFACES that can be directly used by the unit-tests

## Writing your tests

**See HAL/output_gpio/test**

- [Read Unity docs](https://github.com/ThrowTheSwitch/Unity)
- Add the source file you want to test out
- Include the mocks INTERFACES
- Include the test libraries (FFF and Unity)
- Add your include paths
- Update the root level CMakeLists.txt

# IMPROVEMENTS

- [ ] For now we need to manually add our include path requirements for every mock
  - A common include path is setup in the root level CMakeLists.txt
  - This is reused by the mock INTERFACES
- [ ] We need to write a custom CMakeLists.txt every time we have we add a new mock or a unit-test
  - Create functions / macros that can automate this task in CMake
- [ ] UART Interrupt driver code is tightly coupled with FreeRTOS
  - Seperate out the UART Interrupt register calls into a different module
  - Shift th UART Interrupt code that is coupled with FreeRTOS into the HAL layer
  - Write unit-tests and mocks for this
