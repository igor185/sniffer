# Sniffer

## Team

 - [Igor Babin](https://github.com/igor185)

## Prerequisites

 - **C++ compiler** - needs to support **C++17** standard
 - **CMake** 3.15+
 
Dependencies (such as development libraries) can be found in the [dependencies folder](./dependencies) in the form of the text files with package names for different package managers.

## Installing

1. Clone the project.
    ```bash
    git clone git@github.com:igor185/sniffer.git
    ```
2. Install required packages.

   On Ubuntu:
   ```bash
   [[ -r dependencies/apt.txt ]] && sed 's/#.*//' dependencies/apt.txt | xargs sudo apt-get install -y
   ```
   On MacOS:
   ```bash
   [[ -r dependencies/homebrew.txt ]] && sed 's/#.*//' dependencies/homebrew.txt | xargs brew install
   ```
   Use Conan on Windows.
3. Build.
    ```bash
   mkdir build && cd build
    cmake -G"Unik Makefiles" ../
    cmake --build .
    ```

## Usage

```bash
sudo ./sniffer
```

Program to catch and analyze network traffic

Help flags `-h`/`--help` support is available.
