#pragma once

#include <cstdint>
#include <vector>

struct BufferStream {
	std::vector<uint8_t> buffer;
	size_t position;

public:
	BufferStream(const std::vector<uint8_t> &buffer) : buffer(buffer), position(0) {}

	template <typename T>
	T read() {
//		if ((position + sizeof(T)) > buffer.size()) throw "Out of range";
		return *(T*) &buffer[(position += sizeof(T)) - sizeof(T)];
	}

	std::vector<uint8_t> read(int size) {
//		if ((position + size) > buffer.size()) throw "Out of range";
		auto bytes = &buffer[(position += size) - size];
		return std::vector<uint8_t>(bytes, bytes + size);
	}

	operator bool() {
		return position < buffer.size();
	}

	bool empty() {
		return buffer.empty();
	}
};