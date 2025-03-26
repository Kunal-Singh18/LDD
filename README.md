# Linux Device Driver (LDD) Basics

This repository contains basic examples of Linux device drivers, including registering major and minor numbers, registering devices, and performing IOCTL operations.

## Features
- Registering major and minor numbers
- Device registration
- Implementing IOCTL (Input/Output Control)

## Prerequisites
- Linux environment (Ubuntu or any other distribution)
- Kernel headers installed
- Basic knowledge of Linux modules

## Building the Driver

1. Clone the repository:
    ```bash
    git clone https://github.com/Kunal-Singh18/LDD.git
    cd LDD
    ```
2. Build the driver using `make`:
    ```bash
    make
    ```
3. To clean build files:
    ```bash
    make clean
    ```

## Inserting and Removing the Driver

- Insert the driver:
    ```bash
    sudo insmod driver.ko
    ```
- Check driver logs using dmesg:
    ```bash
    dmesg | tail
    ```
- Remove the driver:
    ```bash
    sudo rmmod driver
    ```

## Working with IOCTL
- Perform IOCTL operations using a custom application.
- Ensure correct major and minor numbers are assigned and device file is created using:
    ```bash
    sudo mknod /dev/mydevice c <major_number> 0
    ```
## Editing Makefile
To build different drivers, update the `Makefile` by modifying the following line:
```makefile
obj-m += ioctl.o
```
Replace ioctl.o with the desired driver object file, e.g., obj-m += mydriver.o
THEN run

```
make
sudo insmod mydriver.ko
```