if [ -d "../QtPanel/build" ]; then
    echo "正在删除 ../QtPanel/build ..."
    rm -rf ../QtPanel/build
fi

if [ -d "../ServerPanel/ChatServer/build" ]; then
    echo "正在删除 ../ServerPanel/ChatServer/build ..."
    rm -rf ../ServerPanel/ChatServer/build
fi

if [ -d "../ServerPanel/GateServer/build" ]; then
    echo "正在删除 ../ServerPanel/GateServer/build ..."
    rm -rf ../ServerPanel/GateServer/build
fi

if [ -d "../ServerPanel/StatusServer/build" ]; then
    echo "正在删除 ../ServerPanel/StatusServer/build ..."
    rm -rf ../ServerPanel/StatusServer/build
fi

if [ -d "../ServerPanel/VarifyServer/node_modules" ]; then
    echo "正在删除 ../ServerPanel/VarifyServer/node_modules ..."
    rm -rf ../ServerPanel/VarifyServer/node_modules
fi

echo "所有指定目录清理完成"

