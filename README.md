

# Pixon

> A fast, native image processing CLI tool with chainable operations.

Pixon is a lightweight, high-performance image manipulation library built on **C++**.  
It supports resizing, cropping, rotating, flipping, color adjustments, brightness enhancement, filters, and more — all via a simple CLI or programmatically in Python, Node.js, and Go.  
Output formats supported: **PNG, JPG, JPEG, WebP**.

---

## Features

- Resize, Crop, Rotate  
- Flip (vertical), Flop (horizontal)  
- Adjust brightness, contrast, saturation  
- Blur, Sharpen  
- Composite images  
- Add/Remove Alpha channel  
- Extend canvas and Trim transparent edges  
- Save images in PNG, JPEG, WebP formats  
- Fully chainable CLI operations  
- Works programmatically in Python, Node.js, Go  

---

## 🖥️ Platform Support

| Platform    | Support Status      | Notes                             |
| ----------- | ----------------- | --------------------------------- |
| Linux (x64) | ✅ Fully supported | Prebuilt CLI binaries available; works natively. |
| Linux (ARM) | ⚠️ Experimental    | Build from source recommended.    |
| Windows     | ❌ Not supported   | Native build not available; WSL may work. |
| macOS       | ⚠️ Experimental    | Build from source required; no prebuilt binaries yet. |


---

## Installation

> Builds the project, installs binaries, and configures the linker.

## From GitHub (Manual Installation)

```bash
git clone https://github.com/sudiplahiri3123-alt/pixon.git
cd pixon
mv docs/development_mode/install.sh install.sh
chmod +x install.sh
./install.sh
```

### From PPA (Recommended for Linux)

```bash
sudo add-apt-repository ppa:sudip3123/pixon
sudo apt update
sudo apt install pixon
````


---

## Usage

### Single Operation Commands

```bash
# Resize
pixon input.png resize 400 300 0 resize.jpeg

# Rotate
pixon input.png rotate 90 rotate.jpeg

# Crop
pixon input.png crop 50 50 200 150 crop.jpeg

# Blur
pixon input.png blur 25 blur.jpeg

# Sharpen
pixon input.png sharpen sharpen.jpeg

# Flip vertically
pixon input.png flip flip.jpeg

# Flop horizontally
pixon input.png flop flop.jpeg

# Composite
pixon input.png composite overlay.png 50 50 0.7 composite.jpeg

# Color Adjust
pixon input.png coloradjust 0.1 1.2 1.3 coloradjust.jpeg

# Add alpha channel
pixon input.png addalpha addalpha.png

# Remove alpha channel
pixon addalpha.png removealpha removealpha.jpeg

# Extend
pixon input.png extend 10 10 20 20 extend.jpeg

# Trim
pixon addalpha.png trim trim.png
```

### Multiple Operations in a Single Command

```bash
# Resize → Sharpen → Color Adjust
pixon input.png resize 400 300 0 sharpen coloradjust 0.1 1.2 1.3 output1.jpeg

# Flip → Composite → Blur
pixon input.png flip composite overlay.png 20 20 0.5 blur 25 output2.jpeg

# Rotate → Extend → Sharpen → Color Adjust
pixon input.png rotate 180 extend 5 5 5 5 sharpen coloradjust 0.2 1.1 1.0 output3.jpeg

# Crop → Add Alpha → Trim → Composite
pixon input.png crop 50 50 200 150 addalpha trim composite overlay.png 0 0 0.8 output4.png

# All operations together
pixon input.png resize 400 300 0 rotate 90 crop 50 50 200 150 blur 25 sharpen flip flop composite overlay.png 10 10 0.5 coloradjust 0.1 1.2 1.3 addalpha removealpha extend 10 10 10 10 trim all_ops.jpeg
```

---

## Operations & Parameters

| Operation     | Parameters & Defaults            | Description                           |
| ------------- | -------------------------------- | ------------------------------------- |
| `resize`      | `width height crop=0`            | Resize image; crop center if `crop=1` |
| `rotate`      | `angle`                          | Rotate image by degrees               |
| `crop`        | `x y width height`               | Crop rectangle                        |
| `blur`        | `sigma`                          | Gaussian blur                         |
| `sharpen`     | `sigma=1.0`                      | Sharpen image                         |
| `flip`        | none                             | Flip vertically                       |
| `flop`        | none                             | Flip horizontally                     |
| `composite`   | `overlay_path x y opacity=1.0`   | Overlay another image                 |
| `coloradjust` | `brightness contrast saturation` | Adjust image colors                   |
| `addalpha`    | none                             | Add alpha channel (RGBA)              |
| `removealpha` | none                             | Remove alpha channel                  |
| `extend`      | `top bottom left right [color]`  | Extend canvas with optional color     |
| `trim`        | none                             | Remove transparent edges              |

---

## Python Example

```python
import subprocess

# Single operation
subprocess.run(["pixon","input.png","resize","400","300","0","output_py_single.jpeg"])

# Multiple operations
subprocess.run([
    "pixon","input.png","resize","400","300","0",
    "sharpen","coloradjust","0.1","1.2","1.3","flip",
    "composite","overlay.png","20","20","0.5",
    "output_py_multi.jpeg"
])
```

---

## Node.js Example

```js
const { execFile } = require('child_process');

// Single operation
execFile('pixon', ['input.png','resize','400','300','0','output_node_single.jpeg'], (err, stdout) => {
    if(err) throw err;
    console.log(stdout);
});

// Multiple operations
execFile('pixon', [
    'input.png','resize','400','300','0','sharpen',
    'coloradjust','0.1','1.2','1.3','flip',
    'composite','overlay.png','20','20','0.5',
    'output_node_multi.jpeg'
], (err, stdout) => {
    if(err) throw err;
    console.log(stdout);
});
```

---

## Go Example

```go
package main

import (
    "fmt"
    "os/exec"
)

func main() {
    cmd := exec.Command("pixon", "input.png", "resize", "400", "300", "0", "output_go_single.jpeg")
    out, err := cmd.CombinedOutput()
    if err != nil { fmt.Println(err) }
    fmt.Println(string(out))

    cmdMulti := exec.Command("pixon",
        "input.png", "resize","400","300","0",
        "sharpen","coloradjust","0.1","1.2","1.3",
        "flip","composite","overlay.png","20","20","0.5",
        "output_go_multi.jpeg")
    outMulti, err := cmdMulti.CombinedOutput()
    if err != nil { fmt.Println(err) }
    fmt.Println(string(outMulti))
}
```

---

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Commit your changes (`git commit -am 'Add feature'`)
4. Push (`git push origin feature/my-feature`)
5. Open a Pull Request

---

## License

**Pixon Source-Available License (PSAL) v2.0**

Pixon is licensed under PSAL v2.0:

* Viewing and forking the source code for **testing, experimentation, or contribution**
* Using the software for **any purpose, including commercial use**
* Submitting **pull requests or patches** to the official repository

**Redistribution rules**

* You **cannot redistribute the Software**, modified or unmodified, without **explicit permission** from the project maintainer.
* Permission requests can be made via the contact information in this README. Approved redistributors will be recorded (name and email).

**Why we chose this approach**

* Maintains a **single, high-quality source**
* Allows collaborators to **improve and contribute**
* Users can freely use the software, including **commercially**, via the official PPA

---

## Notes

* **Installation:**
  The easiest way to install Pixon is via the official PPA:


  ```bash
    sudo add-apt-repository ppa:sudip3123/pixon
    sudo apt update
    sudo apt install pixon
  ```


* **Dependencies:**
  OpenCV, libvips, CMake, g++, and other required libraries.
  These dependencies are **installed automatically** when using the official **PPA**.
  For development or building from source, ensure they are installed manually.


* **Redistribution:**
  Not allowed without explicit permission. Contact maintainer for requests.

---

### Image License / Attribution

Images used were generated by Google Gemini.
They depict fictional AI-generated individuals, not real people.
Images may be used, modified, and redistributed in open-source projects.

---

## Badges

![License](https://img.shields.io/badge/License-PSAL%20v2.0-blue)
![Build](https://img.shields.io/badge/build-passing-brightgreen)
![GitHub stars](https://img.shields.io/github/stars/sudiplahiri3123-alt/pixon?style=social)
![GitHub forks](https://img.shields.io/github/forks/sudiplahiri3123-alt/pixon?style=social)
![GitHub issues](https://img.shields.io/github/issues/sudiplahiri3123-alt/pixon)



## 🤝 Looking for Contributors

Pixon is an active project, and contributions are very welcome!  
If you would like to help improve the library, add new features, or assist with Windows/macOS support, feel free to get involved.

### How to Contribute
1. Open an issue or discussion describing what you want to work on.
2. Fork the repository and create a feature branch.
3. Submit a Pull Request — PRs are reviewed promptly.

### Want to Become a Project Collaborator?
If you're interested in long-term contribution, you can email me directly:

📧 **[sudiplahiri3123@gmail.com]**

Active contributors may be added as collaborators on the GitHub repository.



