// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: raw.proto

#include "raw.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace message {
PROTOBUF_CONSTEXPR Data::Data(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.uid_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.dest_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.raw_data_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.seq_)*/int64_t{0}
  , /*decltype(_impl_.crc_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct DataDefaultTypeInternal {
  PROTOBUF_CONSTEXPR DataDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~DataDefaultTypeInternal() {}
  union {
    Data _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 DataDefaultTypeInternal _Data_default_instance_;
}  // namespace message
namespace message {

// ===================================================================

class Data::_Internal {
 public:
};

Data::Data(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:message.Data)
}
Data::Data(const Data& from)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite() {
  Data* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.uid_){}
    , decltype(_impl_.dest_){}
    , decltype(_impl_.raw_data_){}
    , decltype(_impl_.seq_){}
    , decltype(_impl_.crc_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
  _impl_.uid_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.uid_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_uid().empty()) {
    _this->_impl_.uid_.Set(from._internal_uid(), 
      _this->GetArenaForAllocation());
  }
  _impl_.dest_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.dest_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_dest().empty()) {
    _this->_impl_.dest_.Set(from._internal_dest(), 
      _this->GetArenaForAllocation());
  }
  _impl_.raw_data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.raw_data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_raw_data().empty()) {
    _this->_impl_.raw_data_.Set(from._internal_raw_data(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.seq_, &from._impl_.seq_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.crc_) -
    reinterpret_cast<char*>(&_impl_.seq_)) + sizeof(_impl_.crc_));
  // @@protoc_insertion_point(copy_constructor:message.Data)
}

inline void Data::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.uid_){}
    , decltype(_impl_.dest_){}
    , decltype(_impl_.raw_data_){}
    , decltype(_impl_.seq_){int64_t{0}}
    , decltype(_impl_.crc_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.uid_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.uid_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.dest_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.dest_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.raw_data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.raw_data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

Data::~Data() {
  // @@protoc_insertion_point(destructor:message.Data)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<std::string>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Data::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.uid_.Destroy();
  _impl_.dest_.Destroy();
  _impl_.raw_data_.Destroy();
}

void Data::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Data::Clear() {
// @@protoc_insertion_point(message_clear_start:message.Data)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.uid_.ClearToEmpty();
  _impl_.dest_.ClearToEmpty();
  _impl_.raw_data_.ClearToEmpty();
  ::memset(&_impl_.seq_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.crc_) -
      reinterpret_cast<char*>(&_impl_.seq_)) + sizeof(_impl_.crc_));
  _internal_metadata_.Clear<std::string>();
}

const char* Data::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bytes uid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_uid();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes dest = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_dest();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 seq = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _impl_.seq_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 crc = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          _impl_.crc_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes raw_data = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          auto str = _internal_mutable_raw_data();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<std::string>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Data::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:message.Data)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes uid = 1;
  if (!this->_internal_uid().empty()) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_uid(), target);
  }

  // bytes dest = 2;
  if (!this->_internal_dest().empty()) {
    target = stream->WriteBytesMaybeAliased(
        2, this->_internal_dest(), target);
  }

  // int64 seq = 3;
  if (this->_internal_seq() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(3, this->_internal_seq(), target);
  }

  // int32 crc = 4;
  if (this->_internal_crc() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(4, this->_internal_crc(), target);
  }

  // bytes raw_data = 5;
  if (!this->_internal_raw_data().empty()) {
    target = stream->WriteBytesMaybeAliased(
        5, this->_internal_raw_data(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = stream->WriteRaw(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).data(),
        static_cast<int>(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:message.Data)
  return target;
}

size_t Data::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:message.Data)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes uid = 1;
  if (!this->_internal_uid().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_uid());
  }

  // bytes dest = 2;
  if (!this->_internal_dest().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_dest());
  }

  // bytes raw_data = 5;
  if (!this->_internal_raw_data().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_raw_data());
  }

  // int64 seq = 3;
  if (this->_internal_seq() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_seq());
  }

  // int32 crc = 4;
  if (this->_internal_crc() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_crc());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    total_size += _internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size();
  }
  int cached_size = ::_pbi::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Data::CheckTypeAndMergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::MessageLite& from) {
  MergeFrom(*::_pbi::DownCast<const Data*>(
      &from));
}

void Data::MergeFrom(const Data& from) {
  Data* const _this = this;
  // @@protoc_insertion_point(class_specific_merge_from_start:message.Data)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_uid().empty()) {
    _this->_internal_set_uid(from._internal_uid());
  }
  if (!from._internal_dest().empty()) {
    _this->_internal_set_dest(from._internal_dest());
  }
  if (!from._internal_raw_data().empty()) {
    _this->_internal_set_raw_data(from._internal_raw_data());
  }
  if (from._internal_seq() != 0) {
    _this->_internal_set_seq(from._internal_seq());
  }
  if (from._internal_crc() != 0) {
    _this->_internal_set_crc(from._internal_crc());
  }
  _this->_internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
}

void Data::CopyFrom(const Data& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:message.Data)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Data::IsInitialized() const {
  return true;
}

void Data::InternalSwap(Data* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.uid_, lhs_arena,
      &other->_impl_.uid_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.dest_, lhs_arena,
      &other->_impl_.dest_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.raw_data_, lhs_arena,
      &other->_impl_.raw_data_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Data, _impl_.crc_)
      + sizeof(Data::_impl_.crc_)
      - PROTOBUF_FIELD_OFFSET(Data, _impl_.seq_)>(
          reinterpret_cast<char*>(&_impl_.seq_),
          reinterpret_cast<char*>(&other->_impl_.seq_));
}

std::string Data::GetTypeName() const {
  return "message.Data";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace message
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::message::Data*
Arena::CreateMaybeMessage< ::message::Data >(Arena* arena) {
  return Arena::CreateMessageInternal< ::message::Data >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>