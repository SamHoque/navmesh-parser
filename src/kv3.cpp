#include "kv3.h"

namespace navmesh {
namespace {
/* clang-format off */
constexpr std::uint32_t 
    kMagic0 = 0x03564B56, // VKV3
    kMagic1 = 0x4B563301, // KV3\x01
    kMagic2 = 0x4B563302, // KV3\x02
    kMagic3 = 0x4B563303, // KV3\x03
    kMagic4 = 0x4B563304, // KV3\x04
    kMagic5 = 0x4B563305; // KV3\x05
/* clang-format on */
auto
IsKv3Magic(std::uint32_t magic) -> bool
{
  return magic == kMagic0 || magic == kMagic1 || magic == kMagic2 ||
         magic == kMagic3 || magic == kMagic4 || magic == kMagic5 ||
         (magic & 0xFFFFFF00) == 0x4B563300; // KV3\x00
}
} // namespace

auto
KV3::IsBinary(Buffer &buffer) -> bool
{
  if (buffer.bytes_remaining() < 4)
    return false;

  auto start_pos = buffer.position();
  while (buffer.bytes_remaining() > 0 && buffer.Peek<std::uint8_t>() == 0)
    buffer.Skip(1);

  if (buffer.bytes_remaining() < 4) {
    buffer.set_position(start_pos);
    return false;
  }

  auto magic = buffer.Peek<std::uint32_t>();
  auto result = IsKv3Magic(magic);

  buffer.set_position(start_pos);
  return result;
}

auto
KV3::Skip(Buffer &buffer) -> bool
{
  try {
    auto start_pos = buffer.position();
    auto bytes = std::min<std::size_t>(256, buffer.bytes_remaining());
    std::vector<char> header(bytes);

    auto current_pos = buffer.position();
    buffer.Read(header.data(), bytes);
    buffer.set_position(current_pos);

    std::string_view header_str(header.data(), bytes);
    if (header_str.find("<!-- kv3") != std::string_view::npos) {
      buffer.Skip(buffer.bytes_remaining());
      return true;
    }

    if (IsBinary(buffer)) {
      KV3Object dummy;
      return ParseBinary(buffer, dummy);
    }

    buffer.set_position(start_pos);
    return false;
  } catch (...) {
    return false;
  }
}

auto
KV3::ParseBinary(Buffer &buffer, KV3Object &result) -> bool
{
  try {
    auto start_pos = buffer.position();
    while (buffer.bytes_remaining() > 0 && buffer.Peek<std::uint8_t>() == 0)
      buffer.Skip(1);

    if (buffer.bytes_remaining() < 4) {
      buffer.set_position(start_pos);
      return false;
    }

    auto magic = buffer.Read<std::uint32_t>();
    if (magic == kMagic0) {
      buffer.Skip(buffer.bytes_remaining());
      return true;
    }

    auto version = magic & 0xFF;
    auto signature = magic & 0xFFFFFF00;

    if (signature != 0x4B563300) { /* "KV3" + null */
      buffer.set_position(start_pos);
      return false;
    }

    if (version < 1 || version > 5) {
      buffer.set_position(start_pos);
      return false;
    }

    /* ignore guid */
    buffer.Skip(16);

    auto compression_method = buffer.Read<std::uint32_t>();
    if (version == 1) {
      [[maybe_unused]] auto count_bytes1 = buffer.Read<std::int32_t>();
      [[maybe_unused]] auto count_bytes4 = buffer.Read<std::int32_t>();
      [[maybe_unused]] auto count_bytes8 = buffer.Read<std::int32_t>();

      auto size_uncompressed_total = buffer.Read<std::int32_t>();
      buffer.Skip(size_uncompressed_total);
    } else {
      [[maybe_unused]] auto compression_dictionary_id =
          buffer.Read<std::uint16_t>();
      [[maybe_unused]] auto compression_frame_size =
          buffer.Read<std::uint16_t>();
      [[maybe_unused]] auto count_bytes1 = buffer.Read<std::int32_t>();
      [[maybe_unused]] auto count_bytes4 = buffer.Read<std::int32_t>();
      [[maybe_unused]] auto count_bytes8 = buffer.Read<std::int32_t>();
      [[maybe_unused]] auto count_types = buffer.Read<std::int32_t>();
      [[maybe_unused]] auto count_objects = buffer.Read<std::uint16_t>();
      [[maybe_unused]] auto count_arrays = buffer.Read<std::uint16_t>();

      auto size_uncompressed_total = buffer.Read<std::int32_t>();
      auto size_compressed_total = buffer.Read<std::int32_t>();
      auto count_blocks = buffer.Read<std::int32_t>();
      auto size_binary_blobs_bytes = buffer.Read<std::int32_t>();

      if (version >= 4) {
        [[maybe_unused]] auto count_bytes2 = buffer.Read<std::int32_t>();
        [[maybe_unused]] auto size_block_compressed_sizes_bytes =
            buffer.Read<std::int32_t>();
      }

      if (version >= 5) {
        auto size_uncompressed_buffer1 = buffer.Read<std::int32_t>();
        auto size_compressed_buffer1 = buffer.Read<std::int32_t>();
        auto size_uncompressed_buffer2 = buffer.Read<std::int32_t>();
        auto size_compressed_buffer2 = buffer.Read<std::int32_t>();
        [[maybe_unused]] auto count_bytes1_buffer2 =
            buffer.Read<std::int32_t>();
        [[maybe_unused]] auto count_bytes2_buffer2 =
            buffer.Read<std::int32_t>();
        [[maybe_unused]] auto count_bytes4_buffer2 =
            buffer.Read<std::int32_t>();
        [[maybe_unused]] auto count_bytes8_buffer2 =
            buffer.Read<std::int32_t>();
        buffer.Skip(4); // unk13
        [[maybe_unused]] auto count_objects_buffer2 =
            buffer.Read<std::int32_t>();
        [[maybe_unused]] auto count_arrays_buffer2 =
            buffer.Read<std::int32_t>();
        buffer.Skip(4); // unk16

        if (compression_method == 0) {
          if (size_uncompressed_buffer1 > 0)
            buffer.Skip(size_uncompressed_buffer1);

          if (size_uncompressed_buffer2 > 0)
            buffer.Skip(size_uncompressed_buffer2);
        } else {
          if (size_compressed_buffer1 > 0)
            buffer.Skip(size_compressed_buffer1);

          if (size_compressed_buffer2 > 0)
            buffer.Skip(size_compressed_buffer2);
        }
      } else {
        if (compression_method == 0)
          buffer.Skip(size_uncompressed_total);
        else
          buffer.Skip(size_compressed_total);
      }

      if (count_blocks > 0) {
        if (compression_method == 0)
          buffer.Skip(size_binary_blobs_bytes);

        if (count_blocks > 0)
          buffer.Skip(4);
      } else {
        if (buffer.bytes_remaining() >= 4) {
          auto possible_trailer = buffer.Peek<std::uint32_t>();
          if (possible_trailer == 0xFFEEDD00)
            buffer.Skip(4);
        }
      }
    }

    return true;
  } catch (...) {
    return false;
  }
}
} // namespace navmesh
