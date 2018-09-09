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
