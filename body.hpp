#pragma once
#include "material.hpp"

class body
{
public:
    virtual bool trace(const ray& r, float max, hit& h) const = 0;
};