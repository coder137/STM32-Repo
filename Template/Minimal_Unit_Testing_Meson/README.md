- [Unit Testing and Mocking with Meson](#unit-testing-and-mocking-with-meson)
  - [Configuring your Unit-Testing project](#configuring-your-unit-testing-project)
  - [Running the Unit Tests](#running-the-unit-tests)
- [Adding your custom Unit-Test](#adding-your-custom-unit-test)
  - [Writing your mocks](#writing-your-mocks)
  - [Writing your tests](#writing-your-tests)

# Unit Testing and Mocking with Meson

- Forked from Minimal_Unit_Testing
- Added `meson.build` files

## Configuring your Unit-Testing project

```meson
meson builddir --cross-file cross_toolchain.txt
```

## Running the Unit Tests

- Verbose output for each unit test
```
meson test -C builddir -v
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
- Update the root level `meson.build` file and update the `mock_inc_dirs` path
  - Contains the include path to your the mock header files

## Writing your tests

**See HAL/output_gpio/test**

- [Read Unity docs](https://github.com/ThrowTheSwitch/Unity)
- Add the source file you want to test out
- Create a `meson.build` file and create an **executable**
  - Contains **sources**
  - Contains **include directory** `inc_dirs`, `mock_inc_dirs` and `test_inc_dirs`
  - Contains **library** link_with `unity_lib`
  - Contains `native: true`
- Create an **test** with the above executable
  - `test('test_name', test_exe)`
