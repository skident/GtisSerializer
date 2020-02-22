#pragma once

namespace serializer
{

template <class T>
class Serializer
{
public:
    template<class OutSerialized, class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        return T::serialize(outSerialized, inSerializable);
    }

    template<class InSerialized, class OutDeserialized>
    static void deserialize(const InSerialized& inSerialized, OutDeserialized& outDeserialized)
    {
        return T::deserialize(inSerialized, outDeserialized);
    }
};

}
