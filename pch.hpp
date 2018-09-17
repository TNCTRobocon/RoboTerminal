#ifndef __PCH_HEADER_GUARD__
#define __PCH_HEADER_GUARD__ //pragma onceだと警告が出る
//作者 terurin
//用途 コンパイルの高速化

//基礎的なもの
#include <cstdint>
#include <memory>
#include <algorithm>
#include <functional>
//コレクション
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <optional>
//ストリーム系
#include <iostream>
#include <sstream>
#include <fstream>
//C互換
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//GUI
#include <gtk/gtk.h>

#endif