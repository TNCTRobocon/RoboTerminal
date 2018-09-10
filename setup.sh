#/bin/sh

# 作者　terurin
# 用途　開発環境の作成

# 初期化
apt update
apt upgrade -y
# コンパイラの導入
apt install gcc g++ gdb -y
# 開発ツールの導入
apt install clang-format make git -y
# ライブラリの導入
apt install libboost-dev libboost-system-dev -y