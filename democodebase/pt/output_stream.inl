#ifndef PT_OUTPUT_STREAM_INL
#define PT_OUTPUT_STREAM_INL

namespace pt {

//template<typename T>
//inline void write(output_stream& stream, T const& value)
//{
//    write(stream, value);
//}

inline void write(output_stream& stream, uint8 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, int8 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, uint16 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, int16 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, uint32 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, int32 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, uint64 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, int64 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, float32 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, float64 const& value)
{
    stream.write(&value, sizeof(value));
}

inline void write(output_stream& stream, std::string const& str)
{
    write(stream, str.c_str());
}

inline void write(output_stream& stream, bool const& value)
{
    int const tmp = (value ? 1 : 0);
    stream.write(&tmp, sizeof(tmp));
}

inline void write(output_stream& stream, char const* str)
{
    //size_t char_count = strlen(str);
    size_t char_count = strlen(str) + 1;
    if (str[char_count - 1] != 0)
    {
        ++char_count;
        //assert(str[char_count - 1] == 0);
    }
    stream.write(str, char_count * sizeof(char));

}

} // namespace pt

#endif
