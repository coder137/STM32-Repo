- [Minimal Cpp Libraries](#minimal-cpp-libraries)
- [Libraries using the Stack](#libraries-using-the-stack)
  - [Containers](#containers)
  - [Utility](#utility)
- [Usage Overhead](#usage-overhead)
- [Libraries using the Heap](#libraries-using-the-heap)
- [Disabled](#disabled)
- [Check later](#check-later)
  - [Dynamic Memory Management](#dynamic-memory-management)
  - [Error Handling](#error-handling)
  - [Atomic and Threading Library](#atomic-and-threading-library)
  - [Miscellaneous Headers](#miscellaneous-headers)

# Minimal Cpp Libraries

Libraries that use the Stack space instead of Heap

- [cplusplus library header list](http://www.cplusplus.com/reference/)
- [cppreference library header list](https://en.cppreference.com/w/cpp/header)

# Libraries using the Stack

## Containers

- `<array>`

## Utility

- `<type_traits>`
- `<bit>`

# Usage Overhead

- `<initializer_list>`
- `<tuple>`

# Libraries using the Heap

- `<forward_list>`
- `<unordered_set>`
- `<unordered_map>`

# Disabled

- `<typeinfo>`
  - `-fno-rtti`

# Check later

## Dynamic Memory Management

- `<scoped_allocator>`

## Error Handling

- `<system_error>`

## Atomic and Threading Library

- `<atomic>`
- `<thread>`
- `<mutex>`
- `<future>`
- `condition_variable`

## Miscellaneous Headers

- `<chrono>`
- `<random>`
- `<ratio>`
- `<regex>`
