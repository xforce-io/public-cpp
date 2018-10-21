#include "../simple_trie.h"

SimpleTrie::SimpleTrie() : 
    num_children_(0), 
    refcnt_(0),
    child_(NULL) { }

int SimpleTrie::Insert(const char* buf, size_t len_buf) {
  if ( unlikely(0==len_buf) ) {
    ++refcnt_;
    return refcnt_>0 ? 0 : 1;
  }

  SimpleTrie* next_node_to_insert;
  if (0==num_children_) {
    child_ = new (std::nothrow) SimpleTrie;
    if (NULL==child_) return -1;
    num_children_=1;
    child_index_=*buf;
    next_node_to_insert = child_;
  } else if (1==num_children_) {
    if (SCAST<TypeChildIndex>(*buf) != child_index_) {
      SimpleTrie** tmp_children_ = new (std::nothrow) SimpleTrie* [kMaxNumChildren];
      if (NULL==tmp_children_) return -1;

      bzero(tmp_children_, sizeof(*tmp_children_)*kMaxNumChildren);
      tmp_children_[child_index_] = child_;
      tmp_children_[SCAST<TypeChildIndex>(*buf)] = new (std::nothrow) SimpleTrie;
      if (NULL==tmp_children_[SCAST<TypeChildIndex>(*buf)]) return -1;

      children_ = tmp_children_;
      ++num_children_;
      next_node_to_insert = children_[SCAST<TypeChildIndex>(*buf)];
    } else {
      next_node_to_insert = child_;
    }
  } else {
    if (NULL == children_[SCAST<TypeChildIndex>(*buf)]) {
      children_[SCAST<TypeChildIndex>(*buf)] = new (std::nothrow) SimpleTrie;
      if (NULL == children_[SCAST<TypeChildIndex>(*buf)]) return -1;
      ++num_children_;
    }
    next_node_to_insert = children_[SCAST<TypeChildIndex>(*buf)];
  }
  return next_node_to_insert->Insert(buf+1, len_buf-1);
}

bool SimpleTrie::Erase(const char* buf, size_t len_buf) {
  return Erase_(buf, len_buf) >= 0 ? true : false;
}

void SimpleTrie::Clear() {
  if (1==num_children_) {
    XFC_DELETE(child_);
  } else if (num_children_>1) {
    for (size_t i=0; i<kMaxNumChildren; ++i) {
      XFC_DELETE(children_[i])
    }
    XFC_DELETE_ARRAY(children_)
  }
  num_children_ = refcnt_ = 0;
}

SimpleTrie::~SimpleTrie() {
  Clear();
}

int SimpleTrie::Erase_(const char* buf, size_t len_buf) {
  if ( unlikely(0==len_buf) ) {
    if ( unlikely(0==refcnt_) ) return -1;

    --refcnt_;
    return (0==refcnt_ && 0==num_children_) ? 1 : 0;
  }

  SimpleTrie** next_node_to_erase;
  if (1==num_children_) {
    if (SCAST<const TypeChildIndex>(*buf) == child_index_) {
      next_node_to_erase = &child_;
    } else {
      return -1;
    }
  } else if (num_children_>1) {
    if (NULL != children_[SCAST<TypeChildIndex>(*buf)]) {
      next_node_to_erase = &(children_[SCAST<TypeChildIndex>(*buf)]);
    } else {
      return -1;
    }
  } else {
    return -1;
  }

  int ret = (*next_node_to_erase)->Erase_(buf+1, len_buf-1);
  if (0==ret) {
    return 0;
  } else if (ret>0) {
    XFC_DELETE(*next_node_to_erase)
    --num_children_;
    if (1==num_children_) {
      SimpleTrie* tmp_child=NULL;
      //most nodes in tree has num_children_==1, this `for` won't cost much
      for (size_t i=0; i<kMaxNumChildren; ++i) {
        if (children_[i]) {
          tmp_child = children_[i];
          child_index_ = i;
          break;
        }
      }

      delete [] children_;
      child_=tmp_child;
    }
    return (0==refcnt_ && 0==num_children_) ? 1 : 0;
  } else {
    return -1;
  }
}

bool SimpleTrie::IterToNextNode_(char child_index, const SimpleTrie** iter_trie) const {
  if (1 == (*iter_trie)->num_children_) {
    if (SCAST<TypeChildIndex>(child_index) == (*iter_trie)->child_index_) {
      *iter_trie = (*iter_trie)->child_;
    } else {
      return false;
    }
  } else if ((*iter_trie)->num_children_>1) {
    if (NULL != (*iter_trie)->children_[SCAST<TypeChildIndex>(child_index)]) {
      *iter_trie = (*iter_trie)->children_[SCAST<TypeChildIndex>(child_index)];
    } else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

std::ostringstream& operator<<(std::ostringstream& oss, const SimpleTrie& simple_trie) {
  oss << "{ \"num_children_\": " << simple_trie.num_children_ 
    << ", \"refcnt_\": " << simple_trie.refcnt_ 
    << ", \"child_index_\": " << simple_trie.child_index_ << ", ";

  if (1==simple_trie.num_children_) {
    oss << "\"" << 0 << "\":";
    oss << *(simple_trie.child_) << ", "; 
  } else if (simple_trie.num_children_>1) {
    for (size_t i=0; i<SimpleTrie::kMaxNumChildren; ++i) {
      if (NULL!=simple_trie.children_[i]) {
        oss << "\"" << i << "\":";
        oss << *(simple_trie.children_[i]) << ", "; 
      }
    }
  }  
  oss << "}";
  return oss;
}
