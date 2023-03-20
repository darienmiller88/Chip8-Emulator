# Chip8 Emulator inC++
![](https://img.shields.io/badge/made%20by-DarienMiller-blue)
![](https://img.shields.io/badge/C++-yellow)

### Screenshot of emulator testing.
<img width="644" alt="test opcode" src="https://user-images.githubusercontent.com/32966645/226221749-b5fe5994-52c3-4710-a32b-6289960d673d.png">
<img width="640" alt="pong" src="https://user-images.githubusercontent.com/32966645/226221771-4089dad9-6424-476b-b872-d764bb069b4a.png">
<img width="639" alt="fishie" src="https://user-images.githubusercontent.com/32966645/226221793-b3824097-1fbb-4b3b-9ca3-dcc7ae1e05c8.png">
<img width="642" alt="ex1" src="https://user-images.githubusercontent.com/32966645/226221804-da1a30f1-eb59-446a-a6cd-6ce255cf2f8c.png">


### List of compatible games that seem to function.

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
* Finally, run `./main` to run the program. 

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Feel free to leave suggestions as well, I'm always looking for ways to improve!

<p align="right">(<a href="#top">back to top</a>)</p>

## License
[MIT](https://choosealicense.com/licenses/mit/)
