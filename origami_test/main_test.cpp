#include <gtest/gtest.h>

int main(int t_argc, char** t_argv)
{
  ::testing::InitGoogleTest(&t_argc, t_argv);

#ifdef _DEBUG
  assert(false);
#endif

  return RUN_ALL_TESTS();
}