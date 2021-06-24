#ifndef PT_CONVERSION_FILE_3DS_KEY_INL
#define PT_CONVERSION_FILE_3DS_KEY_INL

namespace pt { namespace conversion { namespace file_3ds {

template<typename T>
typed_key<T>::typed_key(int key_number, T const& value,
    acceleration_params const& acceleration)
: key(key_number, acceleration)
, m_value(value)
{
}

template<typename T>
inline T const& typed_key<T>::value() const
{
    return m_value;
}

}}} // namespace pt::conversion::file_3ds

#endif
