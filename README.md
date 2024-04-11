
# Singleton Pattern with Lazy Initialization in C++

This project demonstrates the implementation of the Singleton design pattern with lazy initialization in C++. The Singleton pattern ensures that a class has only one instance and provides a global point of access to that instance.

## Features

- **Lazy Initialization**: The Singleton instance is created only when it is first requested.
- **Thread Safety**: The implementation is thread-safe to guarantee correct behavior in multi-threaded environments.
- **Expensive Task Simulation**: Includes a method `expensiveTask()` to simulate an expensive operation, showcasing real-world usage scenarios.
- **Error Handling**: Proper error handling is implemented to handle invalid arguments and other exceptional situations.

## Compilers
This project is tested and compatible with the following compilers:
- Mingw g++
- Microsoft Visual C++ (MSVC)

## Compilers
This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.
Feel free to use this content directly in your README.md file on GitHub.

## Usage

To use the Singleton pattern in your project, follow these steps:

1. Include the `SingleTon.h` header file in your project.
2. Use the `getInstance()` method to obtain the Singleton instance.
3. Call the desired methods or perform operations on the Singleton object.

```cpp
#include "SingleTon.h"

int main() {
    // Obtain the Singleton instance
    SingleTon* singleton = SingleTon::getInstance();

    // Use the Singleton instance
    singleton->expensiveTask(42);

    return 0;
}


