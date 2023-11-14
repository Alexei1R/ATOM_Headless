# ATOM_Headless

>Atom application to run on Rc car 

Components:

- `app`.
- `libs/*`.


## Getting Started
<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with 

```bash
git clone --recursive https://github.com/Alexei1R/ATOM_Headless
```


Before build install
```bash
# Valve GNS libs
sudo apt-get install libssl-dev
sudo apt-get install libprotobuf-dev protobuf-compiler
```


If the repository was cloned non-recursively previously, use 
```bash
git submodule update --init
```
to clone the necessary submodules.

<ins>**2. Configuring the dependencies:**</ins>

## Usage

Create the `build` directory and configure the build system and run make:

```bash
mkdir build & cd build & cmake .. & make -j$(nproc)
```

To run the project cd `build` and run execurable

