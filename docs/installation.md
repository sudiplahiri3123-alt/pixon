Perfect! Here's a polished `docs/installation.md` for your Pixon docs folder:

````markdown
# Installation

Pixon is a native, high-performance image processing library.  
This document explains how to install Pixon on Linux via the official PPA and outlines development installation.

---

## 🐧 Linux (APT / PPA)

Once the PPA is published, you can install Pixon with:

```bash
sudo add-apt-repository ppa:yourname/pixon
sudo apt update
sudo apt install pixon
````

> Replace `yourname` with the Launchpad username you used to create the PPA.

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

* The easiest and recommended method is via the official PPA once available.
* Development build is intended for testing, contributions, and experimentation.
* Users do not need to build from source for normal usage; the PPA version suffices.







