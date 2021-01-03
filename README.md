# Sniffer
   Final project in OS course. Sniffer for traffic capturing. Based on libpcap for capture packages and Qt for viewing and analysing them. Support gui and cli 
### Team

 - [Igor Babin](https://github.com/igor185)

### Prerequisites

 - **C++ compiler** - needs to support **C++17** standard
 - **CMake** 3.16+
 - **libpcap** 
 - **Qt**
 
Dependencies (such as development libraries) can be found in the [dependencies folder](./dependencies) in the form of the text files with package names for different package managers.

### Installing

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
    make build && cd build && cmake .. && make
    ```

### Usage

```bash
sudo ./sniffer --help
```


### Project structure

```
├── cmake
├── dependencies -- dependencies of project
├── resources -- Qt resources folder
│   └── images
└── src 
    ├── core
    │   ├── core -- main logic for package capturing
    │   └── socket -- definition of packate structure and main function for it
    ├── IO -- IO part of project
    ├── UI 
    │   ├── Details -- Window for full information about package
    │   ├── Filters -- Window for filters set up
    │   ├── HexView -- View for package in hex
    │   ├── InitPage -- Main logic for UI
    │   └── Table -- Table for package preview displaying
    └── utils -- helpers functions
```

### Add new package parsing
1. Open src/core/sockets.h
2. Define class representing your packet
3. Extend base_socket class or more suitable
4. Add <name_of_protocol>.cpp in src/core/socket folder
5. Override all needed methods
6. Add this class in src/core/socket/general.cpp
