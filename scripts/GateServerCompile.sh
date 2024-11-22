BUILD_DIR="../ServerPanel/GateServer/build"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

cmake ..
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "编译成功,正在启动程序..."
    ./bin/GateServer
else
    echo "编译失败!"
    exit 1
fi
