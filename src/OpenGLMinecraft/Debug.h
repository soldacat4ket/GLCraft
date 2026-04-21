#pragma once

#ifdef _DEBUG

#define ASSERT(x) if(!(x)) __debugbreak()
#define ASSERT_CLBCK(x, if_fail) if(!(x) if_fail; __debugbreak()
#define GUARDED_DELETE(x) if(x != nullptr) delete x
#define GUARDED_DELETE_ARRAY(x) if(x != nullptr) delete[] x

#else

#define ASSERT(x)
#define GUARDED_DELETE(x) delete x

#endif