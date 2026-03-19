# Guide to Running C Code on a Linux Vitrual Machine using CloudVeneto

This is a guide to creating, accessing, and using a Linux Virtual Machine on CloudVeneto. The guide explains, in particular, how to use the virtual environment to compile and run C/C++ programs. 

The guide assumes that the reader has access to CloudVeneto and is registered with a project. If this is not yet the case, or if the reader finds any trouble in attempting to do so, please consult the following link: https://userguide.cloudveneto.it/en/latest/Registration.html

## 1 Creating a Key Pair


## 2 Launching an Instance via CloudVeneto
aklfjdakfjad

## 3 Accessing a Linux VM from your Terminal

some instuctions 

then you should see: 
```
bash-5.1$ 
```

then you add that linux line
now you should see 
```
[almalinux@your-instance-name ~]$
```

## 4 Launching your First C/C++ code
### A Installing the required packages
In order to compile C programs one must first make sure that the correct packages are installed. The installation is achieved by running the following line of code in your VM:
```
  sudo dnf install nano
```
The installation should now proceed. If you have the packages installed already, once should see the following response:
```
  Package nano-8.1-3.el10.x86_64 is already installed.
  Dependencies resolved.
  Nothing to do.
  Complete!

```
### B Creating, editing, and running C/C++ files
Creating a program file is done with the ```nano``` command, followed by the name you would like to give your program. For example, if we want to create a C program called "solve_strong_CP", we write the following line:
```
nano solve_strong_CP.c
```
If the file already exists in the working directory, the existing file is opened. The procedure is similar for a .cpp file. Running your C code is done using the command ```gcc``` 
```
gcc solve_strong_CP.c -o solve_strong_CP
```
the new file solve_strong_CP contains the output of you code, and should appear in your direcotry. For a CPP file: 
```
g++ yourCPPfile.cpp -o yourCPPoutput
```
The output can be accessed via 
```
./yourCPPoutput
```
