# Snake game in SFML
![](https://img.shields.io/badge/made%20by-DarienMiller-blue)
![](https://img.shields.io/badge/C++-yellow)

### Screenshot from the game.
<img width="524" alt="image1" src="https://user-images.githubusercontent.com/32966645/221610520-305317c1-ad64-4444-84c0-631784a7d23c.png">

### Short recording of game in play.
![snakegif](https://user-images.githubusercontent.com/32966645/221616513-b396eca1-9c6a-4186-b873-e92477a001f6.gif)

### Built with
* [C++](https://cplusplus.com/)
* [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/)
* [Makefile](https://opensource.com/article/18/8/what-how-makefile)
* [Mingw-w64](https://www.mingw-w64.org/)

### Requirements
#### This repo comes with a pre-compiled .exe ready to run, but in the case of wanting to re-compile for your respective system, follow the below instructions.

#### Windows
* Clone the repository using `git clone https://github.com/darienmiller88/Snake-SFML`
* Navigate to `https://www.mingw-w64.org/` and download a Mingw complier to compile C++ programs.
* Download the version of SFML that matches your Mingw compiler, `GCC 7.3.0 MinGW (DW2) - 32-bit` for a Mingw32 or `GCC 7.3.0 MinGW (SEH) - 64-bit` for Mingw64.
* Extract the `SFML-2.5.1` folder into a reliable location, i.e. the C:\ or your desktop.
* Under the `Path` environmental variable for your OS (in my case windows), add the absolute path to your `SFML-2.5.1/bin` folder.
* In your cloned repo, run the command `g++ -c *.cpp -I<path-to-SFML-2.5.1>/include` to compile cpp files.
* Afterwards, run `g++ -o main *.o -L<path-to-SFML-2.5.1>/lib -lsfml-graphics -lsfml-window -lsfml-system` to link object files.
* Alternatively, you can include both commands in your local `Makefile` and run `mingw32-make` to complete both. 
* Finally, run `./<executablename>.exe` to run the program. 

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Feel free to leave suggestions as well, I'm always looking for ways to improve!

<p align="right">(<a href="#top">back to top</a>)</p>

## License
[MIT](https://choosealicense.com/licenses/mit/)