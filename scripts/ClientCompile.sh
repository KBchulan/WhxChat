BUILD_DIR="../QtPanel/build"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

qmake ../WhxChat.pro -spec linux-g++ CONFIG+=release
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "编译成功,正在启动程序..."
    ./WhxChat
else
    echo "编译失败!"
    exit 1
fi
