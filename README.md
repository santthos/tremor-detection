# Tremor Detection System

A medical AI robotics project for detecting and analyzing hand tremors using video analysis.

## Project Overview

This system combines multiple technologies to create a comprehensive tremor detection solution:
- Python for video capture and preprocessing
- C++ for real-time processing
- Qt for the user interface
- MATLAB for signal processing and analysis

## System Requirements

- Raspberry Pi 4 (recommended)
- Camera module (Raspberry Pi Camera V2 or better)
- MATLAB Runtime
- Python 3.8+
- Qt 6.0+
- C++17 compatible compiler
- OpenCV 4.5+

## Project Structure

```
├── src/
│   ├── python/           # Python video capture and preprocessing
│   ├── cpp/             # C++ core processing
│   ├── qt/              # Qt GUI application
│   └── matlab/          # MATLAB analysis scripts
├── include/             # Header files
├── build/              # Build directory
├── docs/               # Documentation
└── tests/              # Test files
```

## Installation

1. Install dependencies:
```bash
sudo apt-get update
sudo apt-get install python3-opencv python3-numpy python3-pip
sudo apt-get install qt6-base-dev
sudo apt-get install build-essential cmake
```

2. Install Python dependencies:
```bash
pip3 install -r requirements.txt
```

3. Build the C++ components:
```bash
mkdir build && cd build
cmake ..
make
```

## Usage

1. Start the GUI application:
```bash
./build/tremor_detection_gui
```

2. Connect the camera and ensure proper lighting conditions
3. Follow the on-screen instructions to record hand movements
4. The system will analyze the video and provide tremor intensity measurements

## License

This project is licensed under the MIT License - see the LICENSE file for details. 