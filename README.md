# Chip8 Emulator inC++
![](https://img.shields.io/badge/made%20by-DarienMiller-blue)
![](https://img.shields.io/badge/C++-yellow)

### Screenshot of emulator testing.
<img width="644" alt="test opcode" src="https://user-images.githubusercontent.com/32966645/226221749-b5fe5994-52c3-4710-a32b-6289960d673d.png">
<img width="640" alt="pong" src="https://user-images.githubusercontent.com/32966645/226221771-4089dad9-6424-476b-b872-d764bb069b4a.png">
<img width="639" alt="fishie" src="https://user-images.githubusercontent.com/32966645/226221793-b3824097-1fbb-4b3b-9ca3-dcc7ae1e05c8.png">
<img width="642" alt="ex1" src="https://user-images.githubusercontent.com/32966645/226221804-da1a30f1-eb59-446a-a6cd-6ce255cf2f8c.png">

### Game recordings

#### Tetris
![tetris](https://user-images.githubusercontent.com/32966645/226222657-3ae7b431-52a5-4477-936c-5f561605f3e1.gif)

#### Pong
![pong](https://user-images.githubusercontent.com/32966645/226222739-c892ea7b-ccc3-4c6e-b563-1875475db264.gif)

#### Keypad
![keypad](https://user-images.githubusercontent.com/32966645/226222729-945dfa82-bb59-4e3f-b71f-9e1cf6ffb296.gif)

#### Breakout
![breakout](https://user-images.githubusercontent.com/32966645/226222709-40a2a1e8-aa7e-426f-8c84-e5407ee6a49a.gif)


### How to run Chip-8 roms:
`./<executable-name.exe> <rom-folder-name>/<chip8rom.ch8>`
Ex. `./main "games/Tetris [Fran Dachille, 1991].ch8" `

### Built with
* [C++](https://cplusplus.com/)
* [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/)
* [Makefile](https://opensource.com/article/18/8/what-how-makefile)
* [Mingw-w64](https://www.mingw-w64.org/)

### Requirements
#### This repo comes with a pre-compiled .exe ready to run, but in the case of wanting to re-compile for your respective system, follow the below instructions.

#### Windows
* Clone the repository using `git clone https://github.com/darienmiller88/Chip8-Emulator`
* Navigate to `https://www.mingw-w64.org/` and download a 64-bit Mingw complier to compile C++ programs.
* In your cloned repo, run the command `g++ -c *.cpp -I./include` to compile cpp files.
* Afterwards, run `g++ -o main *.o -L./lib -lsfml-graphics -lsfml-window -lsfml-system` to link object files.
* Alternatively, you can include both commands in your local `Makefile` and run `mingw32-make` to complete both. 
* Finally, run `./main <rom-folder-name>/<chip8rom.ch8>` to run the program. 

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Feel free to leave suggestions as well, I'm always looking for ways to improve!

<p align="right">(<a href="#top">back to top</a>)</p>

## License
[MIT](https://choosealicense.com/licenses/mit/)
