#/bin/sh

# 作者　terurin
# 用途　開発環境の作成

# 初期化
apt update
apt upgrade -y
# コンパイラの導入
apt install -y gcc g++ gdb
# 開発ツールの導入
apt install -y clang-format make git
# ライブラリの導入
apt install -y libboost-dev libboost-system-dev libgtk-3-dev
# gcc8.1.0の導入
cd
git clone https://bitbucket.org/sol_prog/raspberry-pi-gcc-binary.git
cd raspberry-pi-gcc-binary
tar xf gcc-8.1.0.tar.bz2
mv gcc-8.1.0 /usr/local
echo 'export PATH=/usr/local/gcc-8.1.0/bin:$PATH' >> .bashrc
source .bashrc
cd
rm -r raspberry-pi-gcc-binary
mv /usr/bin/gcc /usr/bin/gcc-backup
mv /usr/bin/g++ /usr/bin/g++-backup
ln -s /usr/local/gcc-8.1.0/bin/gcc-8.1.0 /usr/bin/gcc
ln -s /usr/local/gcc-8.1.0/bin/g++-8.1.0 /usr/bin/g++
