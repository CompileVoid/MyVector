# MyVector

A lightweight, header-only C++17 implementation of a dynamic array (`std::vector` alternative) built from scratch with manual memory management, RAII compliance, move semantics, and comprehensive unit testing.

## Features

* **Header-Only Library:** Easy to integrate into any C++17 (or newer) project via CMake `INTERFACE` target.
* **Rule of Five & RAII:** Full support for copy/move constructors and copy/move assignment operators with strong self-assignment safety and proper resource cleanup.
* **Flexible Initialization:** Supports default construction, size-based construction, fill construction, and `std::initializer_list` brace initialization.
* **Dynamic Capacity Management:** Automatic exponential reallocation (`capacity * 2`) alongside explicit `reserve()`, `resize()`, and `clear()` operations.
* **Safe & Unsafe Element Access:** Fast unchecked `operator[]` and bounds-checked `at()` throwing `std::out_of_range`.
* **STL Compatibility:** Raw pointer iterators (`begin()`, `end()`) allowing seamless integration with range-based `for` loops and `<algorithm>` utilities.
* **Modern Build & Testing Setup:** Configured with `CMakePresets.json`, `vcpkg` manifest mode, `.clang-format`, and full unit test coverage using **GoogleTest**.

## Project Structure

```text
MyVector/
├── include/
│   └── MyVector.h          # Core template class implementation
├── tests/
│   └── test_my_vector.cpp  # GoogleTest unit test suite
├── .clang-format           # Code style configuration
├── .gitignore              # Git ignore rules for CMake, CLion, and vcpkg
├── CMakeLists.txt          # CMake build and test configuration
├── CMakePresets.json       # Ninja + clang-cl / vcpkg build presets
└── vcpkg.json              # Dependency manifest (gtest)
```

## Prerequisites

* **Compiler:** C++17 compatible compiler (MSVC, `clang-cl`, GCC, or Clang)
* **Build System:** CMake 3.25+ and Ninja
* **Package Manager:** [vcpkg](https://github.com)

## Building and Running Tests

1. **Clone the repository:**
   ```bash
   git clone https://github.com<your-username>/MyVector.git
   cd MyVector
   ```

2. **Configure the project using CMake Presets:**
   *(Make sure the toolchain path in `CMakePresets.json` matches your local `vcpkg` installation).*
   ```bash
   cmake --preset dev
   ```

3. **Build the test suite:**
   ```bash
   cmake --build --preset dev
   ```

4. **Run unit tests via CTest:**
   ```bash
   ctest --preset dev
   ```

## Usage Example

```cpp
#include <iostream>
#include "MyVector.h"

int main() {
    MyVector<int> vec = {10, 20, 30};
    vec.pushBack(40);
    vec.pushBack(50);

    vec.at(0) = 5;

    for (const auto& item : vec) {
        std::cout << item << ' ';
    }
    // Output: 5 20 30 40 50

    return 0;
}
```

## Integrating into Another CMake Project

Add the repository as a subdirectory and link against the `MyVector` interface target:

```cmake
add_subdirectory(MyVector)
target_link_libraries(your_target PRIVATE MyVector)
```
