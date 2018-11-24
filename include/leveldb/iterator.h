// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// An iterator yields a sequence of key/value pairs from a source.
// The following class defines the interface.  Multiple implementations
// are provided by this library.  In particular, iterators are provided
// to access the contents of a Table or a DB.
//
// Multiple threads can invoke const methods on an Iterator without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same Iterator must use
// external synchronization.

#ifndef STORAGE_LEVELDB_INCLUDE_ITERATOR_H_
#define STORAGE_LEVELDB_INCLUDE_ITERATOR_H_

#include "leveldb/export.h"
#include "leveldb/slice.h"
#include "leveldb/status.h"

namespace leveldb {

// 迭代器类
class LEVELDB_EXPORT Iterator {
 public:
  //构造函数
  Iterator();

  Iterator(const Iterator&) = delete;
  Iterator& operator=(const Iterator&) = delete;

  //析构函数
  virtual ~Iterator();

  // An iterator is either positioned at a key/value pair, or
  // not valid.  This method returns true iff the iterator is valid.
  // 迭代器是否有效
  virtual bool Valid() const = 0;

  // Position at the first key in the source.  The iterator is Valid()
  // after this call iff the source is not empty.
  // 寻找数据库中的第一个key
  virtual void SeekToFirst() = 0;

  // Position at the last key in the source.  The iterator is
  // Valid() after this call iff the source is not empty.
  // 寻找数据库中的最后一个key
  virtual void SeekToLast() = 0;

  // Position at the first key in the source that is at or past target.
  // The iterator is Valid() after this call iff the source contains
  // an entry that comes at or past target.
  // 查询在指定的切片数据中是否存在指定的key
  virtual void Seek(const Slice& target) = 0;

  // Moves to the next entry in the source.  After this call, Valid() is
  // true iff the iterator was not positioned at the last entry in the source.
  // REQUIRES: Valid()
  // 移动到下一个条目
  virtual void Next() = 0;

  // Moves to the previous entry in the source.  After this call, Valid() is
  // true iff the iterator was not positioned at the first entry in source.
  // REQUIRES: Valid()
  // 移动到上一个条目
  virtual void Prev() = 0;

  // Return the key for the current entry.  The underlying storage for
  // the returned slice is valid only until the next modification of
  // the iterator.
  // REQUIRES: Valid()
  // 当前条目的key值
  virtual Slice key() const = 0;

  // Return the value for the current entry.  The underlying storage for
  // the returned slice is valid only until the next modification of
  // the iterator.
  // REQUIRES: Valid()
  // 当前条目的value值
  virtual Slice value() const = 0;

  // If an error has occurred, return it.  Else return an ok status.
  // 错误状态
  virtual Status status() const = 0;

  // Clients are allowed to register function/arg1/arg2 triples that
  // will be invoked when this iterator is destroyed.
  //
  // Note that unlike all of the preceding methods, this method is
  // not abstract and therefore clients should not override it.
  // 这个方法不是抽象方法，继承者不能重写
  using CleanupFunction = void (*)(void* arg1, void* arg2);
  void RegisterCleanup(CleanupFunction function, void* arg1, void* arg2);

 private:
  // Cleanup functions are stored in a single-linked list.
  // The list's head node is inlined in the iterator.
  // 清除节点结构体
  struct CleanupNode {
    // The head node is used if the function pointer is not null.
    CleanupFunction function;
    void* arg1;
    void* arg2;
    CleanupNode* next;

    // True if the node is not used. Only head nodes might be unused.
    bool IsEmpty() const { return function == nullptr; }
    // Invokes the cleanup function.
    void Run() { assert(function != nullptr); (*function)(arg1, arg2); }
  };
  // 头部清除节点
  CleanupNode cleanup_head_;
};

// Return an empty iterator (yields nothing).
// 返回一个空的迭代器
LEVELDB_EXPORT Iterator* NewEmptyIterator();

// Return an empty iterator with the specified status.
// 返回一个空的带错误状态的迭代器
LEVELDB_EXPORT Iterator* NewErrorIterator(const Status& status);

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_INCLUDE_ITERATOR_H_
