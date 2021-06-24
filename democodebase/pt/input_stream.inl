#ifndef PT_INPUT_STREAM_INL
#define PT_INPUT_STREAM_INL

namespace pt {

//template<typename T>
//inline void read(input_stream& stream, T& value)
//{
//    stream.read(&value, sizeof(value));
//}

inline void read(input_stream& stream, uint8& value)
{
    stream.read(&value, sizeof(uint8));
}

inline void read(input_stream& stream, int8& value)
{
    stream.read(&value, sizeof(int8));
}

inline void read(input_stream& stream, uint16& value)
{
    stream.read(&value, sizeof(uint16));
}

inline void read(input_stream& stream, int16& value)
{
    stream.read(&value, sizeof(int16));
}

inline void read(input_stream& stream, uint32& value)
{
    stream.read(&value, sizeof(uint32));
}

inline void read(input_stream& stream, int32& value)
{
    stream.read(&value, sizeof(int32));
}

inline void read(input_stream& stream, uint64& value)
{
    stream.read(&value, sizeof(uint64));
}

inline void read(input_stream& stream, int64& value)
{
    stream.read(&value, sizeof(int64));
}

inline void read(input_stream& stream, float32& value)
{
    stream.read(&value, sizeof(float32));
}

inline void read(input_stream& stream, float64& value)
{
    stream.read(&value, sizeof(float64));
}

//

inline void read_array(input_stream& stream, size_t count, uint8* destination)
{
    stream.read(destination, count * sizeof(uint8));
}

inline void read_array(input_stream& stream, size_t count, int8* destination)
{
    stream.read(destination, count * sizeof(int8));
}

inline void read_array(input_stream& stream, size_t count, uint16* destination)
{
    stream.read(destination, count * sizeof(uint16));
}

inline void read_array(input_stream& stream, size_t count, int16* destination)
{
    stream.read(destination, count * sizeof(int16));
}

inline void read_array(input_stream& stream, size_t count, pt::uint32* destination)
{
    stream.read(destination, count * sizeof(pt::uint32));
}

inline void read_array(input_stream& stream, size_t count, int32* destination)
{
    stream.read(destination, count * sizeof(int32));
}

inline void read_array(input_stream& stream, size_t count, uint64* destination)
{
    stream.read(destination, count * sizeof(uint64));
}

inline void read_array(input_stream& stream, size_t count, int64* destination)
{
    stream.read(destination, count * sizeof(int64));
}

inline void read_array(input_stream& stream, size_t count, float32* destination)
{
    stream.read(destination, count * sizeof(float32));
}

inline void read_array(input_stream& stream, size_t count, float64* destination)
{
    stream.read(destination, count * sizeof(float64));
}

inline void read(input_stream& s, bool& x)
{
    uint32 tmp;
    read(s, tmp);
    x = (tmp != 0);
}

template<typename T>
inline T read(input_stream& s)
{
	T t;
	read(s, t);
	return t;
}

} // namespace pt

#endif
