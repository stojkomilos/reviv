#pragma once

#if RV_DEBUG
    #define RV_ASSERT(x, y) if(!(x)) { std::cout << y << std::endl; assert(false); }
    //#define RV_ASSERT(x) if(!(x)) { assert(false); }
#else
    #define RV_ASSERT(x, ...) { }
    //#define RV_ASSERT(x) { }
#endif