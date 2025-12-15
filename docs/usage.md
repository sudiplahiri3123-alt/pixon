
# Usage

Pixon can be used via the CLI or programmatically in Python, Node.js, and Go.  
This document focuses on CLI usage.

---

## Single Operation Examples

```bash
# Resize an image
pixon input.png resize 400 300 0 output_resize.jpeg

# Rotate 90 degrees
pixon input.png rotate 90 output_rotate.jpeg

# Crop: x=50, y=50, width=200, height=150
pixon input.png crop 50 50 200 150 output_crop.jpeg

# Blur
pixon input.png blur 25 output_blur.jpeg

# Sharpen
pixon input.png sharpen output_sharpen.jpeg

# Flip vertically
pixon input.png flip output_flip.jpeg

# Flop horizontally
pixon input.png flop output_flop.jpeg

# Composite: overlay.png at x=50, y=50, opacity=0.7
pixon input.png composite overlay.png 50 50 0.7 output_composite.jpeg

# Color Adjust: brightness=0.1, contrast=1.2, saturation=1.3
pixon input.png coloradjust 0.1 1.2 1.3 output_color.jpeg

# Add alpha channel
pixon input.png addalpha output_alpha.png

# Remove alpha channel
pixon output_alpha.png removealpha output_remove_alpha.jpeg

# Extend canvas: top, bottom, left, right
pixon input.png extend 10 10 20 20 output_extend.jpeg

# Trim transparent edges
pixon output_alpha.png trim output_trim.png
````

---

## Multiple Operations in a Single Command

```bash
# Resize → Sharpen → Color Adjust
pixon input.png resize 400 300 0 sharpen coloradjust 0.1 1.2 1.3 output1.jpeg

# Flip → Composite → Blur
pixon input.png flip composite overlay.png 20 20 0.5 blur 25 output2.jpeg

# Rotate → Extend → Sharpen → Color Adjust
pixon input.png rotate 180 extend 5 5 5 5 sharpen coloradjust 0.2 1.1 1.0 output3.jpeg

# Crop → Add Alpha → Trim → Composite
pixon input.png crop 50 50 200 150 addalpha trim composite overlay.png 0 0 0.8 output4.png
```

---

## Operations & Parameters Table

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

### Notes

* You can chain multiple operations in a single command for efficiency.
* Output file must be specified as the last argument.
* CLI is consistent across Python, Node.js, and Go usage via subprocess execution.
