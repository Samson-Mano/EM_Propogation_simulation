# EM Propagation Visualization Tool

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Analysis](#analysis)
- [Visualization](#visualization)
- [Example](#example)
- [Dependencies](#dependencies)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The EM Propagation Visualization Tool is a C++ application capable of simulating Larmor delayed potential fields of oscillating monopoles. This tool allows the application of oscillation frequency and the selection of standard paths for oscillation. Five paths are implemented: 
1. Linear 
2. Circular 
3. Elliptical 
4. Cardioid 
5. TriFoliate. 

Additional paths can be added with slight modifications to the code.
This tool utilizes C++17 and OpenGL 3.3 for rendering the dynamic results.

## Installation

1. Clone the repository: `git clone https://github.com/Samson-Mano/EM_Propagation_simulation.git`
2. Navigate to the project directory: `cd EM_Propagation_simulation`
3. Compile the project using your C++17 compiler: `g++ -std=c++17 -o EM_Propagation_simulation.cpp`
4. Run the compiled executable: `./EM_Propagation_simulation`

## Analysis

This tool performs Larmor delayed field potential simulation for the pre-selected charge paths and oscillation frequency.

## Visualization

The visualization is powered by OpenGL 3.3. The tool offers batch rendering of the dynamic response.

## Example

Here are some screenshots of the EM Propagation Visualization Tool in action:

### Linear Path Oscillation

![Linear Path oscillation](Images/linear_path_gif.gif)

### Circular Path Oscillation

![Circular Path oscillation](Images/circular_path_gif.gif)

###Cardioid Path Oscillation

![Cardioid Path oscillation](Images/cardioid_path_gif.gif)


## Dependencies

- C++17 Compiler
- OpenGL 3.3
- Third-party libraries (if any)

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
