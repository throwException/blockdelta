#pragma once

inline uint64_t file_size(char* path)
{
	struct stat buf;
	stat(path, &buf);
	return buf.st_size;
}

template <typename T>
inline T host_to_big_endian(T x)
{
	uint8_t v[sizeof(T)];
	for (unsigned i = 0; i < sizeof(T); i++) {
		unsigned const shift = ((unsigned)sizeof(T) - i - 1) * 8;
		v[i] = (uint8_t)((x & (0xffllu << shift)) >> shift);
	}
	return *(T *)v;
}

