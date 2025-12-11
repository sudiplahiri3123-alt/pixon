
# Pixon API Documentation

Pixon can be used programmatically in **Python**, **Node.js**, and **Go**.  
This document provides examples for calling Pixon from these languages using the CLI.

---

## Python

You can call Pixon operations using the `subprocess` module:

### Single Operation Example
```python
import subprocess

# Resize an image
subprocess.run([
    "pixon",
    "input.png",
    "resize", "400", "300", "0",
    "output_py_single.jpeg"
])
````

### Multiple Operations Example

```python
import subprocess

subprocess.run([
    "pixon",
    "input.png",
    "resize", "400", "300", "0",
    "sharpen",
    "coloradjust", "0.1", "1.2", "1.3",
    "flip",
    "composite", "overlay.png", "20", "20", "0.5",
    "output_py_multi.jpeg"
])
```

---

## Node.js

Use `child_process.execFile` to run Pixon commands:

### Single Operation Example

```javascript
const { execFile } = require('child_process');

execFile('pixon', ['input.png', 'resize', '400', '300', '0', 'output_node_single.jpeg'], (err, stdout) => {
    if (err) throw err;
    console.log(stdout);
});
```

### Multiple Operations Example

```javascript
const { execFile } = require('child_process');

execFile('pixon', [
    'input.png', 'resize', '400', '300', '0',
    'sharpen', 'coloradjust', '0.1', '1.2', '1.3',
    'flip', 'composite', 'overlay.png', '20', '20', '0.5',
    'output_node_multi.jpeg'
], (err, stdout) => {
    if (err) throw err;
    console.log(stdout);
});
```

---

## Go

Use `os/exec` to run Pixon commands from Go:

### Single Operation Example

```go
package main

import (
    "fmt"
    "os/exec"
)

func main() {
    cmd := exec.Command("pixon", "input.png", "resize", "400", "300", "0", "output_go_single.jpeg")
    out, err := cmd.CombinedOutput()
    if err != nil {
        fmt.Println(err)
    }
    fmt.Println(string(out))
}
```

### Multiple Operations Example

```go
package main

import (
    "fmt"
    "os/exec"
)

func main() {
    cmdMulti := exec.Command(
        "pixon",
        "input.png", "resize", "400", "300", "0",
        "sharpen", "coloradjust", "0.1", "1.2", "1.3",
        "flip", "composite", "overlay.png", "20", "20", "0.5",
        "output_go_multi.jpeg",
    )
    out, err := cmdMulti.CombinedOutput()
    if err != nil {
        fmt.Println(err)
    }
    fmt.Println(string(out))
}
```

---

## Notes

* Pixon must be installed and available in the system `PATH` for these examples to work.
* The examples assume the source images (`input.png`, `overlay.png`) exist in the current working directory.
* You can chain multiple operations in a single command to improve efficiency.

For a full list of operations and parameters, see the [Operations Table](usage.md#operations--parameters-table).


