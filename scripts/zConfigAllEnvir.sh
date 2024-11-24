# 作者声明：本脚本纯AI生成，仅供参考，作者全部是源码编译的环境，请自行配置，作者不承担任何责任
# 权限设置为只读，防止误伤

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

QT_VERSION="5.14.2"
NODE_VERSION="16.20.2"
BOOST_VERSION="1.74.0"
QT_INSTALL_DIR="/opt/qt5.14"
BOOST_INSTALL_DIR="/usr/local"

if [ "$EUID" -ne 0 ]; then 
    echo -e "${RED}请使用root权限运行此脚本${NC}"
    exit 1
fi

log() {
    echo -e "${GREEN}[$(date +'%Y-%m-%d %H:%M:%S')] $1${NC}"
}

error() {
    echo -e "${RED}[$(date +'%Y-%m-%d %H:%M:%S')] ERROR: $1${NC}"
}

warning() {
    echo -e "${YELLOW}[$(date +'%Y-%m-%d %H:%M:%S')] WARNING: $1${NC}"
}

install_basic_tools() {
    log "正在安装基本工具..."
    apt-get update
    apt-get install -y \
        build-essential \
        git \
        cmake \
        wget \
        curl \
        pkg-config \
        autoconf \
        libtool \
        python3 \
        python3-pip \
        libgl1-mesa-dev \
        libxkbcommon-x11-dev \
        libfontconfig1-dev \
        libfreetype6-dev \
        libx11-dev \
        libx11-xcb-dev \
        libxext-dev \
        libxfixes-dev \
        libxi-dev \
        libxrender-dev \
        libxcb1-dev \
        libxcb-glx0-dev \
        libxcb-keysyms1-dev \
        libxcb-image0-dev \
        libxcb-shm0-dev \
        libxcb-icccm4-dev \
        libxcb-sync0-dev \
        libxcb-xfixes0-dev \
        libxcb-shape0-dev \
        libxcb-randr0-dev \
        libxcb-render-util0-dev
}

install_qt() {
    log "正在安装Qt 5.14..."
    
    wget https://download.qt.io/archive/online_installers/3.2/qt-unified-linux-x64-3.2.3-online.run
    chmod +x qt-unified-linux-x64-3.2.3-online.run
    
    warning "请手动运行Qt安装器并安装Qt 5.14.2:"
    warning "./qt-unified-linux-x64-3.2.3-online.run"
    warning "安装路径请选择: ${QT_INSTALL_DIR}"
    warning "请在安装完成后按回车继续..."
    read
    
    echo "export PATH=\$PATH:${QT_INSTALL_DIR}/gcc_64/bin" >> /etc/profile
    echo "export QT_DIR=${QT_INSTALL_DIR}/gcc_64" >> /etc/profile
    source /etc/profile
}

install_boost() {
    log "正在安装Boost ${BOOST_VERSION}..."
    
    wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION//./_}.tar.gz
    tar xzf boost_${BOOST_VERSION//./_}.tar.gz
    cd boost_${BOOST_VERSION//./_}
    
    ./bootstrap.sh --prefix=${BOOST_INSTALL_DIR}
    ./b2 install -j$(nproc)
    cd ..
    rm -rf boost_${BOOST_VERSION//./_}*
}

install_nodejs() {
    log "正在安装Node.js ${NODE_VERSION}..."
    
    curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash
    
    export NVM_DIR="$HOME/.nvm"
    [ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
    
    nvm install ${NODE_VERSION}
    nvm use ${NODE_VERSION}
    nvm alias default ${NODE_VERSION}
    
    cd ../ServerPanel/VarifyServer
    npm install
    cd ../../scripts
}

install_grpc() {
    log "正在安装gRPC..."
    apt-get install -y \
        libgrpc++-dev \
        libgrpc-dev \
        protobuf-compiler-grpc \
        libprotobuf-dev \
        protobuf-compiler
}

install_redis() {
    log "正在安装Redis..."
    apt-get install -y redis-server
    systemctl enable redis-server
    systemctl start redis-server

    sed -i 's/bind 127.0.0.1/bind 127.0.0.1/g' /etc/redis/redis.conf
    systemctl restart redis-server
}

install_mysql() {
    log "正在安装MySQL..."
    apt-get install -y mysql-server
    systemctl enable mysql
    systemctl start mysql
    
    warning "请手动设置MySQL root密码:"
    mysql_secure_installation
}

check_environment() {
    log "检查环境配置..."
    
    if ! command -v qmake &> /dev/null; then
        error "Qt安装失败"
        exit 1
    fi
    
    if ! command -v node &> /dev/null; then
        error "Node.js安装失败"
        exit 1
    fi
    
    if ! command -v protoc &> /dev/null; then
        error "gRPC安装失败"
        exit 1
    fi
    
    if ! systemctl is-active --quiet redis-server; then
        error "Redis服务未运行"
        exit 1
    fi
    
    if ! systemctl is-active --quiet mysql; then
        error "MySQL服务未运行"
        exit 1
    fi
    
    log "环境检查完成!"
}

setup_configs() {
    log "配置文件处理..."
    
    if [ ! -f "../ServerPanel/VarifyServer/config.json" ]; then
        cp ../ServerPanel/VarifyServer/config.json.example ../ServerPanel/VarifyServer/config.json
        warning "请修改 VarifyServer/config.json 中的配置"
    fi
    
    if [ ! -f "../QtPanel/resources/config.ini" ]; then
        cp ../QtPanel/resources/config.ini.example ../QtPanel/resources/config.ini
        warning "请修改 QtPanel/resources/config.ini 中的配置"
    fi
}

cleanup() {
    log "清理临时文件..."
    rm -f qt-unified-linux-x64-3.2.3-online.run
}

main() {
    log "开始安装环境..."
    
    install_basic_tools
    install_qt
    install_boost
    install_nodejs
    install_grpc
    install_redis
    install_mysql
    
    setup_configs
    check_environment
    cleanup
    
    log "环境安装完成!"
    warning "请确保已经:"
    warning "1. 完成Qt 5.14.2的手动安装"
    warning "2. 修改了所有必要的配置文件"
    warning "3. 设置了MySQL root密码"
    warning "4. 执行 source /etc/profile 使环境变量生效"
}

main