# Voxign
A tool for voxelizing SDF models with preview window for output.

## Screen Shot
![Screenshot](https://raw.githubusercontent.com/paquinn/voxign/master/resources/basic.png)

## Usage
### GUI
The **Test** and **Preview** windows can be moved around as desired.

The **Voxelize** button will reload the file for the preview and the rendering.

The **Save** button will save each layer as a `.png` file in the current folder.

By clicking into the preview window after a model has been voxelized the render and preview can move and zoom with the mouse.
### Command Line
 
Currently the program internally sets the input file and voxel size.

The program must be run from the voxign directory in order to locate its python files.

```sh
voxign $ ./{build_directory}/voxign
```

## Building
The project requires a C++11 or C++17 compiler and CMake.

First clone the project and make sure to initialize its submodules:

```sh
$ git clone --recursive https://github.com/paquinn/voxign
```

If the project was cloned normally, initialize the submodules with:

```sh
$ git submodule update --init --recursive
```
Next create a build folder and run cmake. 

```sh
voxign $ mkdir build
voxign $ cd build
build $ cmake ..
build $ make -j
```

If all dependencies are found on the system, the project should compile. 

Tested on Arch Linux and Ubuntu. High probability of working on macOS and very low probability on Windows.