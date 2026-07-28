#pragma once

#include "Types.h"

namespace MRe {

class ENGINE_API IRenderAPI {
public:
    static auto Create() -> Scope<IRenderAPI>;
    
    
};

}
