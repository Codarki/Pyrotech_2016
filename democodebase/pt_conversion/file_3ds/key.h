#ifndef PT_CONVERSION_FILE_3DS_KEY_H
#define PT_CONVERSION_FILE_3DS_KEY_H

#include "pt/vector.h"

namespace pt { namespace conversion { namespace file_3ds {



/// Data structure for 3ds axis angle key.
struct axis_angle
{
    axis_angle(vector3f const& axis_, float angle_)
    :   axis(axis_)
    ,   angle(angle_)
    {
    }
    float angle;
    vector3f axis;
};

/// Base class for all 3ds keyframer keys.
class key
{
public:
    struct acceleration_params
    {
        acceleration_params();

        float tension;
        float continuity;
        float bias;
        float ease_to;
        float ease_from;
    };

public:
    key(int key_number, acceleration_params const& acceleration);
    int key_number() const;
    acceleration_params const& acceleration() const;

    virtual ~key() {}

private: // data members
    int m_key_number;
    acceleration_params m_acceleration;
};

/// 3ds keyframer key of type T
template<typename T>
class typed_key : public key
{
public:
    typed_key(int key_number, T const& value,
        acceleration_params const& acceleration);
    T const& value() const;

private: // data members
    T m_value;
};

typedef typed_key<axis_angle> typed_key_axis_angle;
typedef typed_key<float> typed_key_float;
typedef typed_key<vector3f> typed_key_vector3f;

}}} // namespace pt::conversion::file_3ds

#include "key.inl"

#endif
