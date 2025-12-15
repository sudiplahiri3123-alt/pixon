# Installation

Pixon is a native, high-performance image processing library.  
This document explains how to install Pixon on Linux via the official PPA and outlines development installation.

---

## 🐧 Linux (PPA)

```bash
sudo add-apt-repository ppa:sudip3123/pixon
sudo apt update
sudo apt install pixon
````



This installs the CLI binary and libraries to your system, making `pixon` available globally.

---

## 🛠️ Development Installation (From Source)

For testing or contributing, you can build Pixon from source. This requires:

* `CMake`
* `g++` or `clang++`
* `libvips-dev`
* `libopencv-dev`

Steps:



```bash
git clone https://github.com/sudiplahiri3123-alt/pixon.git
cd pixon
mv docs/development_mode/install.sh install.sh
chmod +x install.sh
./install.sh
```

> This will build the shared library `libpixon_core.so` and install headers and binaries to `/usr/local`.

---

## 📌 Notes

* The easiest and recommended method for installing Pixon is via the official PPA.
* The development build is intended for testing, development contributions, and experimentation.
* For typical usage, building from source is unnecessary; the PPA version is sufficient.







