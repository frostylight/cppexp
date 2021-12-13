#include "GameObjectCore.hpp"

using namespace FROSTYLIB;
using namespace ObjectCore;


namespace ObjectInterface {
    template<>
    constexpr uint IBox<Reimu>::_boxwidth = 24;
    template<>
    constexpr uint IBox<Reimu>::_boxheight = 40;
    template<>
    constexpr uint IBox<Reimu>::_halfboxwidth = IBox<Reimu>::_boxwidth >> 1;
    template<>
    constexpr uint IBox<Reimu>::_halfboxheight = IBox<Reimu>::_boxheight >> 1;


} // namespace ObjectInterface