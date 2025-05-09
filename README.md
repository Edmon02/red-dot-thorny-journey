# Orbital Pathways - Dynamic Circle Animation in C

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
![Language](https://img.shields.io/badge/language-C-brightgreen)
![SDL2](https://img.shields.io/badge/SDL-2.0-orange)
![Status](https://img.shields.io/badge/status-stable-green)

<p align="center">
  <img src="assets/demo.gif" alt="Orbital Pathways Animation" width="600"/>
</p>

## 🌟 Overview

**Orbital Pathways** is a mesmerizing visual simulation that recreates a mathematical concept originally expressed in Wolfram Language. The animation displays a dynamic system of orbiting circles with a unique "red dot journey" that transfers between circles when they overlap.

This project demonstrates:
- Complex orbital motion simulation
- Event-driven state transitions
- Elegant mathematical visualization
- SDL2 rendering with alpha blending for trails

## 📋 Description

In this simulation, multiple circles orbit around a central point at varying distances and speeds. The red circle uniquely "jumps" to a new orbit when it overlaps with another circle, creating a thorny journey through geometric space.

The visual elements include:
- A red circle that transfers between orbits
- White trails showing the recent path of each circle
- Connecting lines from the center to each circle
- A striking black background enhancing visual contrast

Originally titled "Red dot thorny journey through the realms of time: random walk shaped by overlap events", this implementation faithfully reproduces the mathematical beauty of the original Wolfram code in high-performance C.

## 🎬 Demo  

[![Demo Video](https://img.youtube.com/vi/eAUD9P5aRsc/0.jpg)](https://www.youtube.com/watch?v=eAUD9P5aRsc)

## 🛠️ Installation

### Prerequisites

- C compiler (GCC, Clang)
- SDL2 library
- Basic build tools

### Compilation Instructions

#### macOS:
```bash
# Install SDL2 using Homebrew
brew install sdl2

# Compile with pkg-config
gcc -o thorny_journey thorny_journey.c $(pkg-config --cflags --libs sdl2) -lm
```

#### Linux:
```bash
# Install SDL2 development libraries
sudo apt-get install libsdl2-dev

# Compile
gcc -o thorny_journey thorny_journey.c $(sdl2-config --cflags --libs) -lm
```

#### Windows (with MinGW):
```bash
# After installing SDL2 development libraries
gcc -o thorny_journey thorny_journey.c -IC:/path/to/SDL2/include -LC:/path/to/SDL2/lib -lSDL2main -lSDL2 -lm
```

## 🎮 Usage

After compilation, simply run the executable:

```bash
./thorny_journey
```

Controls:
- Close the window or press ESC to exit the program

## 📊 Technical Implementation

The core algorithm:
1. Initializes circles with unique radii and angular velocities
2. Updates positions of all circles based on time
3. Detects overlap events between the red circle and others
4. Implements state transitions when overlaps occur
5. Renders the visual elements with proper trails and outlines

Key technical features:
- Custom dynamic array implementation for tracking state
- Efficient collision detection using Euclidean distance calculations
- Alpha-blended rendering for motion trails
- Frame timing for consistent animation speed

## 🧮 Mathematical Foundation

The simulation is based on a set of parametric equations that define circle positions:
- Position calculation: `center + r * (cos(angle), sin(angle))`
- Angular progression: `angle = (frame + offset) * delta`
- Overlap detection: `distance(circle1, circle2) < radius1 + radius2`

The seed value (5) ensures reproducible randomness in the angular velocities.

## 🔄 Original Wolfram Implementation

This C implementation is based on the following Wolfram code:

```wolfram
SeedRandom[5];
rList     = Range[30, 300, 10];
deltaList = RandomReal[{-0.5, 0.5}, Length[rList]]/60;
radiiList = (30 + rList^2/10000)/2;
center    = {250, 250};
trailLen  = 33;
DynamicModule[{redIdx = 1, inactiveSet = {}},
 Manipulate[
  Module[{angles, posRed, overlaps, available, newIdx, blackDisks, redDisk},
   ...
  ],
  {f, 0, 10000, Animator,
     AnimationRate   -> 30,
     AnimationRunning-> True}
 ]
]
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments

- Original mathematical concept inspired by Wolfram Language
- SDL2 community for the excellent graphics library
- Mathematical visualization community

---

<p align="center">
  Made with ❤️ by a math and visualization enthusiast
</p>
