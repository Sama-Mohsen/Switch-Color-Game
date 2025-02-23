## Switch Color OpenGL Game

### Overview
This project is an OpenGL-based **2D Color Switch game** that combines interactive gameplay mechanics with visually appealing graphics. It incorporates OpenGL rendering techniques along with sound integration using the *irrKlang* audio engine.

### Features
- **Classic Mode Gameplay**: The game features a dynamic environment where a ball moves through obstacles of different colors.
- **Physics-based Movements**: Smooth animations and collision detection for an engaging experience.
- **Sound Integration**: Background music and sound effects using the *irrKlang* library.
- **Pause & Resume Functionality**: Players can pause and resume the game seamlessly.

### Dependencies
- OpenGL (`GLUT`, `GLU`, `GL`)
- *irrKlang* Audio Library
- C++ Compiler (GCC, Clang, or MSVC)

### Installation & Usage
#### 1. Clone the Repository
```bash
git clone <repository_url>
cd <project_directory>
```

#### 2. Compile the Project
```bash
g++ main.cpp -o color_switch -lGL -lGLU -lglut -lirrKlang
```

#### 3. Run the Application
```bash
./color_switch
```

#### 4. Controls
- **SPACE**: Jump
- **P**: Pause/Resume
- **M**: Toggle music
- **N**: Toggle sound effects
- **ESC**: Exit the game

### Notes
- If you experience issues with *irrKlang*, try commenting out sound-related lines.
- Ensure all dependencies are properly installed before running the application.

