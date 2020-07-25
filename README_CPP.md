# CPP Guidelines

- Modern C++
  - C++17
- Guidelines
  - Objects and Classes
  - Constexpr 
  - Mostly Hardware Independent
  - Style conformity
  - Asserts
  - Compisition over Inheritance
  - Static Memory ONLY
  - Dependency injection over hard dependencies
- DO NOT USE
  - Standard C++ Library
    - String, iostream or algorithm
  - Use of double-precision
    - Use float
  - Avoid Class or function template
    - Use in constexpr is fine
    - Other cases could cause template explosion
  - #define macro
    - Use inline, const, constexpr
  - Runtime exception
  - Runtime casting
- Unit-Testing
  - Catch
  - FFF
  - FakeIt

