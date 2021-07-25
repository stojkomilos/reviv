#pragma once

#if RV_DEBUG
    #define RV_ASSERT(x, y) if(!(x)) { std::cout << "\033[1;31mASSERT FAILED:\033[0m " << y << std::endl; assert(false); }
    #define RV_DBG1 RV_ASSERT(!std::isnan(Scene::getPlayerEntity()->get<TransformComponent>()->position.x), "");
    //#define RV_ASSERT(x) if(!(x)) { assert(false); }
#else
    #define RV_ASSERT(x, ...) { }
    //#define RV_ASSERT(x) { }
#endif
