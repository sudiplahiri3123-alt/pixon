#!/usr/bin/env bash
set -e  # Exit on any error
set -u  # Treat unset variables as errors

# 1️⃣ Build the project
echo "Building Pixon..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)

# 2️⃣ Install binaries and headers
echo "Installing Pixon..."
sudo make install

# 3️⃣ Ensure /usr/local/lib is in the dynamic linker path
LIB_CONF_FILE="/etc/ld.so.conf.d/pixon.conf"

if [ ! -f "$LIB_CONF_FILE" ]; then
    echo "Configuring dynamic linker..."
    echo "/usr/local/lib" | sudo tee "$LIB_CONF_FILE"
    sudo ldconfig
else
    echo "Dynamic linker already configured."
fi

echo "Installation complete!"
echo "You can now run 'pixon' from anywhere."
