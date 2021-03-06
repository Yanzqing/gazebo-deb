/*
 * Copyright 2013 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>

#include "gazebo/common/Time.hh"
#include "gazebo/util/Diagnostics.hh"

using namespace gazebo;

TEST(DiagnosticsTest, Diagnostics)
{
#ifdef ENABLE_DIAGNOSTICS
  util::DiagnosticManager *mgr = util::DiagnosticManager::Instance();
  EXPECT_TRUE(mgr != NULL);

  common::Time prev = common::Time::GetWallTime();
  {
    mgr->StartTimer("test");
    mgr->StopTimer("test");
    EXPECT_STREQ("test", mgr->GetLabel(0).c_str());
    EXPECT_EQ(1, mgr->GetTimerCount());
  }
  common::Time after = common::Time::GetWallTime();

  EXPECT_TRUE(mgr->GetTime(0) == mgr->GetTime("test"));
  EXPECT_TRUE(mgr->GetTime(0) <= after - prev);
#endif
}


/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
