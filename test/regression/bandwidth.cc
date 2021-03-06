/*
 * Copyright 2012 Open Source Robotics Foundation
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

#include <unistd.h>

#include "ServerFixture.hh"

using namespace gazebo;

class BandwidthTest : public ServerFixture
{
};

boost::mutex g_mutex;
std::vector<int> g_bwBytes;
std::vector<common::Time> g_bwTime;


void BandwidthMsg(const std::string &_msg)
{
  boost::mutex::scoped_lock lock(g_mutex);
  g_bwBytes.push_back(_msg.size());
  g_bwTime.push_back(common::Time::GetWallTime());
}

TEST_F(BandwidthTest, Bandwidth)
{
  Load("worlds/pr2.world");

  transport::NodePtr node(new transport::Node());
  node->Init("default");

  std::string topic = "/gazebo/default/pose/info";

  transport::SubscriberPtr sub = node->Subscribe(topic, BandwidthMsg);

  while (true)
  {
    common::Time::MSleep(100);
    {
      boost::mutex::scoped_lock lock(g_mutex);
      if (g_bwBytes.size() >= 100)
      {
        std::sort(g_bwBytes.begin(), g_bwBytes.end());

        float sumSize = 0;
        unsigned int count = g_bwBytes.size();
        common::Time dt = g_bwTime[count - 1] - g_bwTime[0];

        for (unsigned int i = 0; i < count; ++i)
          sumSize += g_bwBytes[i];

        float meanSize = sumSize / count;
        float totalBw = sumSize / dt.Double();

        printf("Bandwidth:\n");
        printf("  Total[%6.2f B/s] Mean[%6.2f B] Messages[%d] Time[%4.2fs]\n",
               totalBw, meanSize, count, dt.Double());

        EXPECT_GT(totalBw, 1000.0);
        g_bwBytes.clear();
        g_bwTime.clear();

        break;
      }
    }
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
