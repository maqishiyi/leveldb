// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_BLOCK_H_
#define STORAGE_LEVELDB_TABLE_BLOCK_H_

#include <stddef.h>
#include <stdint.h>
#include "leveldb/iterator.h"

namespace leveldb {

struct BlockContents;
class Comparator;
//块类
class Block {
 public:
  // Initialize the block with the specified contents.
  // 构造函数，参数为块内容，explicit抑制隐式类型转换进行
  explicit Block(const BlockContents& contents);

  ~Block();
  // 大小
  size_t size() const { return size_; }
  //构造新迭代器的方法
  Iterator* NewIterator(const Comparator* comparator);

 private:
  // 私有函数
  uint32_t NumRestarts() const;

  // 数据，大小，重新开始数组的偏移量，块是否拥有数据
  const char* data_;
  size_t size_;
  uint32_t restart_offset_;     // Offset in data_ of restart array
  bool owned_;                  // Block owns data_[]

  // No copying allowed
  // 不允许复制
  Block(const Block&);
  void operator=(const Block&);

  // 继承自iterator的类
  class Iter;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_BLOCK_H_
