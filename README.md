
# About
Migrate resvg library to Qt image format plugins.

[resvg](https://github.com/RazrFalcon/resvg) supports both SVG1.1 and SVG2.0 specs.


## Build
First install depedencies:
- cmake
- cpp compiler
- Qt5
- rust environment, see [rustup.rs](https://rustup.rs/)

Then compile with cmake:
```bash
cmake -B build 
cmake --build build
```

If everything is well, `build/imageformats/librusty_resvg.so` will be generated.

Copy `build/imageformats/librusty_resvg.so` to plugin directory of your Qt app.
Like `$APP_DIR/plugins/imagesformats/librusty_resvg.so`.

Qt library path can be configured with:
```cpp
// Remeber to install librusty_resvg.so into $APP_DIR/plugins/imageformats/ first.
QDir app_dir = QCoreApplication::applicationDirPath();
const QString plugin_path = app_dir.absoluteFilePath("plugins");
// Prepend plugin path.
QCoreApplication::addLibraryPath(plugin_path);
```

## License
This project is released with Mozilla Public License Version 2.0
which can be found in [LICENSE file](LICENSE.txt).
