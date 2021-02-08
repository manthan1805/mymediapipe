// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mediapipe/util/color.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_mediapipe_2futil_2fcolor_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_mediapipe_2futil_2fcolor_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_mediapipe_2futil_2fcolor_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_mediapipe_2futil_2fcolor_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_mediapipe_2futil_2fcolor_2eproto;
namespace mediapipe {
class Color;
class ColorDefaultTypeInternal;
extern ColorDefaultTypeInternal _Color_default_instance_;
class ColorMap;
class ColorMapDefaultTypeInternal;
extern ColorMapDefaultTypeInternal _ColorMap_default_instance_;
class ColorMap_LabelToColorEntry_DoNotUse;
class ColorMap_LabelToColorEntry_DoNotUseDefaultTypeInternal;
extern ColorMap_LabelToColorEntry_DoNotUseDefaultTypeInternal _ColorMap_LabelToColorEntry_DoNotUse_default_instance_;
}  // namespace mediapipe
PROTOBUF_NAMESPACE_OPEN
template<> ::mediapipe::Color* Arena::CreateMaybeMessage<::mediapipe::Color>(Arena*);
template<> ::mediapipe::ColorMap* Arena::CreateMaybeMessage<::mediapipe::ColorMap>(Arena*);
template<> ::mediapipe::ColorMap_LabelToColorEntry_DoNotUse* Arena::CreateMaybeMessage<::mediapipe::ColorMap_LabelToColorEntry_DoNotUse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace mediapipe {

// ===================================================================

class Color :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:mediapipe.Color) */ {
 public:
  Color();
  virtual ~Color();

  Color(const Color& from);
  Color(Color&& from) noexcept
    : Color() {
    *this = ::std::move(from);
  }

  inline Color& operator=(const Color& from) {
    CopyFrom(from);
    return *this;
  }
  inline Color& operator=(Color&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Color& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Color* internal_default_instance() {
    return reinterpret_cast<const Color*>(
               &_Color_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Color& a, Color& b) {
    a.Swap(&b);
  }
  inline void Swap(Color* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Color* New() const final {
    return CreateMaybeMessage<Color>(nullptr);
  }

  Color* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Color>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Color& from);
  void MergeFrom(const Color& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Color* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "mediapipe.Color";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_mediapipe_2futil_2fcolor_2eproto);
    return ::descriptor_table_mediapipe_2futil_2fcolor_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRFieldNumber = 1,
    kGFieldNumber = 2,
    kBFieldNumber = 3,
  };
  // optional int32 r = 1;
  bool has_r() const;
  private:
  bool _internal_has_r() const;
  public:
  void clear_r();
  ::PROTOBUF_NAMESPACE_ID::int32 r() const;
  void set_r(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_r() const;
  void _internal_set_r(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // optional int32 g = 2;
  bool has_g() const;
  private:
  bool _internal_has_g() const;
  public:
  void clear_g();
  ::PROTOBUF_NAMESPACE_ID::int32 g() const;
  void set_g(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_g() const;
  void _internal_set_g(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // optional int32 b = 3;
  bool has_b() const;
  private:
  bool _internal_has_b() const;
  public:
  void clear_b();
  ::PROTOBUF_NAMESPACE_ID::int32 b() const;
  void set_b(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_b() const;
  void _internal_set_b(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:mediapipe.Color)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int32 r_;
  ::PROTOBUF_NAMESPACE_ID::int32 g_;
  ::PROTOBUF_NAMESPACE_ID::int32 b_;
  friend struct ::TableStruct_mediapipe_2futil_2fcolor_2eproto;
};
// -------------------------------------------------------------------

class ColorMap_LabelToColorEntry_DoNotUse : public ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<ColorMap_LabelToColorEntry_DoNotUse, 
    std::string, ::mediapipe::Color,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_MESSAGE,
    0 > {
public:
  typedef ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<ColorMap_LabelToColorEntry_DoNotUse, 
    std::string, ::mediapipe::Color,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_MESSAGE,
    0 > SuperType;
  ColorMap_LabelToColorEntry_DoNotUse();
  ColorMap_LabelToColorEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  void MergeFrom(const ColorMap_LabelToColorEntry_DoNotUse& other);
  static const ColorMap_LabelToColorEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const ColorMap_LabelToColorEntry_DoNotUse*>(&_ColorMap_LabelToColorEntry_DoNotUse_default_instance_); }
  static bool ValidateKey(std::string* s) {
#ifndef NDEBUG
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
       s->data(), static_cast<int>(s->size()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE, "mediapipe.ColorMap.LabelToColorEntry.key");
#endif
    return true;
 }
  static bool ValidateValue(void*) { return true; }
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& other) final;
  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_mediapipe_2futil_2fcolor_2eproto);
    return ::descriptor_table_mediapipe_2futil_2fcolor_2eproto.file_level_metadata[1];
  }

  public:
};

// -------------------------------------------------------------------

class ColorMap :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:mediapipe.ColorMap) */ {
 public:
  ColorMap();
  virtual ~ColorMap();

  ColorMap(const ColorMap& from);
  ColorMap(ColorMap&& from) noexcept
    : ColorMap() {
    *this = ::std::move(from);
  }

  inline ColorMap& operator=(const ColorMap& from) {
    CopyFrom(from);
    return *this;
  }
  inline ColorMap& operator=(ColorMap&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const ColorMap& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ColorMap* internal_default_instance() {
    return reinterpret_cast<const ColorMap*>(
               &_ColorMap_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(ColorMap& a, ColorMap& b) {
    a.Swap(&b);
  }
  inline void Swap(ColorMap* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ColorMap* New() const final {
    return CreateMaybeMessage<ColorMap>(nullptr);
  }

  ColorMap* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ColorMap>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ColorMap& from);
  void MergeFrom(const ColorMap& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ColorMap* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "mediapipe.ColorMap";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_mediapipe_2futil_2fcolor_2eproto);
    return ::descriptor_table_mediapipe_2futil_2fcolor_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  enum : int {
    kLabelToColorFieldNumber = 1,
  };
  // map<string, .mediapipe.Color> label_to_color = 1;
  int label_to_color_size() const;
  private:
  int _internal_label_to_color_size() const;
  public:
  void clear_label_to_color();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >&
      _internal_label_to_color() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >*
      _internal_mutable_label_to_color();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >&
      label_to_color() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >*
      mutable_label_to_color();

  // @@protoc_insertion_point(class_scope:mediapipe.ColorMap)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::MapField<
      ColorMap_LabelToColorEntry_DoNotUse,
      std::string, ::mediapipe::Color,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_MESSAGE,
      0 > label_to_color_;
  friend struct ::TableStruct_mediapipe_2futil_2fcolor_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Color

// optional int32 r = 1;
inline bool Color::_internal_has_r() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool Color::has_r() const {
  return _internal_has_r();
}
inline void Color::clear_r() {
  r_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Color::_internal_r() const {
  return r_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Color::r() const {
  // @@protoc_insertion_point(field_get:mediapipe.Color.r)
  return _internal_r();
}
inline void Color::_internal_set_r(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000001u;
  r_ = value;
}
inline void Color::set_r(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_r(value);
  // @@protoc_insertion_point(field_set:mediapipe.Color.r)
}

// optional int32 g = 2;
inline bool Color::_internal_has_g() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool Color::has_g() const {
  return _internal_has_g();
}
inline void Color::clear_g() {
  g_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Color::_internal_g() const {
  return g_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Color::g() const {
  // @@protoc_insertion_point(field_get:mediapipe.Color.g)
  return _internal_g();
}
inline void Color::_internal_set_g(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  g_ = value;
}
inline void Color::set_g(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_g(value);
  // @@protoc_insertion_point(field_set:mediapipe.Color.g)
}

// optional int32 b = 3;
inline bool Color::_internal_has_b() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool Color::has_b() const {
  return _internal_has_b();
}
inline void Color::clear_b() {
  b_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Color::_internal_b() const {
  return b_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Color::b() const {
  // @@protoc_insertion_point(field_get:mediapipe.Color.b)
  return _internal_b();
}
inline void Color::_internal_set_b(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000004u;
  b_ = value;
}
inline void Color::set_b(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_b(value);
  // @@protoc_insertion_point(field_set:mediapipe.Color.b)
}

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// ColorMap

// map<string, .mediapipe.Color> label_to_color = 1;
inline int ColorMap::_internal_label_to_color_size() const {
  return label_to_color_.size();
}
inline int ColorMap::label_to_color_size() const {
  return _internal_label_to_color_size();
}
inline void ColorMap::clear_label_to_color() {
  label_to_color_.Clear();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >&
ColorMap::_internal_label_to_color() const {
  return label_to_color_.GetMap();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >&
ColorMap::label_to_color() const {
  // @@protoc_insertion_point(field_map:mediapipe.ColorMap.label_to_color)
  return _internal_label_to_color();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >*
ColorMap::_internal_mutable_label_to_color() {
  return label_to_color_.MutableMap();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, ::mediapipe::Color >*
ColorMap::mutable_label_to_color() {
  // @@protoc_insertion_point(field_mutable_map:mediapipe.ColorMap.label_to_color)
  return _internal_mutable_label_to_color();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace mediapipe

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_mediapipe_2futil_2fcolor_2eproto