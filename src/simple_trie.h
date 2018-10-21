#pragma once

#include <sstream>
#include "common.h"

/*
 * @notice : (1) no pool is used here, libs such as tcmalloc should be used to 
 *           speed up the process of insert and erase
 *           (2) remove `virtual` from destruct function to save memory, this class 
 *           should not be inherited
 */
class SimpleTrie {
 public:
  typedef uint8_t TypeChildIndex;
   
 public:
  static const size_t kMaxNumChildren = 256;

 public:
  explicit SimpleTrie();

  int Insert(const char* buf, size_t len_buf);
  bool Erase(const char* buf, size_t len_buf);
  inline bool Find(const char* buf, size_t len_buf) const;
  inline int HasPrefixOf(const char* buf, size_t len_buf) const;
  void Clear();

  ~SimpleTrie();

 private: 
  /*
   * @return : >0 : this node should be deleted
   *          ==0 : succ
   *           <0 : no such string
   */
  int Erase_(const char* buf, size_t len_buf);
  bool IterToNextNode_(char child_index, const SimpleTrie** iter_trie) const;

 private:
  uint16_t num_children_;
  uint8_t child_index_;
  uint32_t refcnt_;
  union {
    SimpleTrie* child_;
    SimpleTrie** children_;
  };

  friend std::ostringstream& operator<<(std::ostringstream& oss, 
      const SimpleTrie& simple_trie);
};

bool SimpleTrie::Find(const char* buf, size_t len_buf) const {
  const char* buf_tmp = buf;
  size_t len_tmp = len_buf;
  const SimpleTrie* iter_trie = this;
  while ( unlikely(0!=len_tmp) ) {
    if (true != IterToNextNode_(*buf_tmp, &iter_trie)) return false;

    ++buf_tmp;
    --len_tmp;
  }
  return iter_trie->refcnt_>0 ? true : false;
}

int SimpleTrie::HasPrefixOf(const char* buf, size_t len_buf) const {
  const char* buf_tmp = buf;
  size_t len_tmp = len_buf;
  const SimpleTrie* iter_trie = this;
  for (;;) {
    if ( unlikely(iter_trie->refcnt_>0) ) return len_buf - len_tmp;
    if (0==len_tmp || true != IterToNextNode_(*buf_tmp, &iter_trie)) return -1;
 
    ++buf_tmp;
    --len_tmp;
  }
  return -1;
}

std::ostringstream& operator<<(std::ostringstream& oss, const SimpleTrie& simple_trie);
