#pragma once

class ISerializable {
public:
    virtual void serialize() = 0;
    virtual void deserialize() = 0;
};

