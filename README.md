
# Photo Editor

Professional image editor written in C++ using wxWidgets and OpenCV, featuring an advanced graphical interface and AI-powered capabilities.

## Main Features
- **Open and save images** in multiple formats.
- **Unlimited undo/redo** for edits.
- **Transformation tools:** crop, rotate, flip, resize, aspect ratio adjustment.
- **Filters and effects:** grayscale, sepia, negative, blur, sharpen, edge detection, cartoon, watercolor, mosaic, oil painting, pencil sketch.
- **Advanced adjustments:** brightness, contrast, saturation, hue, gamma, temperature, shadows/highlights, auto white balance, auto color enhance, noise reduction, adaptive sharpening.
- **Additions to images:** borders, vignette effect, watermark, text.
- **AI support:** background removal using ONNX models (optional).
- **Modern user interface:** menu and toolbars, interactive canvas panel with zoom, grid, and info support.
- **Multi-language support:** English and Arabic.
- **Professional splash screen.**
- **Project and settings management** (integrated database).

## Requirements
- C++17 or newer
- wxWidgets
- OpenCV
- ONNX Runtime (optional for AI features)

## Build & Run
1. Install dependencies:
   - wxWidgets
   - OpenCV
   - ONNX Runtime (optional)
2. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
3. Run the application:
   ```bash
   ./PhotoEditor
   ```

## Structure
- `src/core/` : Image processing and filters
- `src/core/ai/` : AI features
- `src/database/` : Project and settings management
- `src/ui/` : Graphical user interface
- `main.cpp` & `src/app.cpp` : Entry point

## Contributions
All contributions are welcome! Please open a pull request or contact via issues.

## License
Open source project (specify the appropriate license here)

---
Photo Editor - A professional Arabic project for advanced image editing with AI features.
