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
#include <string.h>
#include "math/Helpers.hh"
#include "transport/TransportTypes.hh"
#include "transport/Node.hh"

#include "rendering/RenderEngine.hh"
#include "rendering/Camera.hh"
#include "sensors/Sensors.hh"
#include "sensors/CameraSensor.hh"
#include "ServerFixture.hh"
#include "images_cmp.h"


using namespace gazebo;
class FactoryTest : public ServerFixture
{
};

TEST_F(FactoryTest, Box)
{
  math::Pose setPose, testPose;
  Load("worlds/empty.world");
  SetPause(true);

  for (unsigned int i = 0; i < 100; i++)
  {
    std::ostringstream name;
    name << "test_box_" << i;
    setPose.Set(math::Vector3(0, 0, i+0.5), math::Quaternion(0, 0, 0));
    SpawnBox(name.str(), math::Vector3(1, 1, 1), setPose.pos,
        setPose.rot.GetAsEuler());
    testPose = GetEntityPose(name.str());
    EXPECT_TRUE(math::equal(testPose.pos.x, setPose.pos.x, 0.1));
    EXPECT_TRUE(math::equal(testPose.pos.y, setPose.pos.y, 0.1));
    EXPECT_TRUE(math::equal(testPose.pos.z, setPose.pos.z, 0.1));
  }
}

TEST_F(FactoryTest, Sphere)
{
  math::Pose setPose, testPose;
  Load("worlds/empty.world");
  SetPause(true);

  for (unsigned int i = 0; i < 100; i++)
  {
    std::ostringstream name;
    name << "test_sphere_" << i;
    setPose.Set(math::Vector3(0, 0, i+0.5), math::Quaternion(0, 0, 0));
    SpawnSphere(name.str(), setPose.pos, setPose.rot.GetAsEuler());
    testPose = GetEntityPose(name.str());
    EXPECT_TRUE(math::equal(testPose.pos.x, setPose.pos.x, 0.1));
    EXPECT_TRUE(math::equal(testPose.pos.y, setPose.pos.y, 0.1));
    EXPECT_TRUE(math::equal(testPose.pos.z, setPose.pos.z, 0.1));
  }
}

TEST_F(FactoryTest, Cylinder)
{
  math::Pose setPose, testPose;
  Load("worlds/empty.world");
  SetPause(true);

  for (unsigned int i = 0; i < 100; i++)
  {
    std::ostringstream name;
    name << "test_cylinder_" << i;
    setPose.Set(math::Vector3(0, 0, i+0.5), math::Quaternion(0, 0, 0));
    SpawnCylinder(name.str(), setPose.pos, setPose.rot.GetAsEuler());
    testPose = GetEntityPose(name.str());
    EXPECT_TRUE(math::equal(testPose.pos.x, setPose.pos.x, 0.1));
    EXPECT_TRUE(math::equal(testPose.pos.y, setPose.pos.y, 0.1));
    EXPECT_TRUE(math::equal(testPose.pos.z, setPose.pos.z, 0.1));
  }
}

TEST_F(FactoryTest, Camera)
{
  // Disabling this test for now. Different machines return different
  // camera images. Need a better way to evaluate rendered content.
/*
  if (rendering::RenderEngine::Instance()->GetRenderPathType() ==
      rendering::RenderEngine::NONE)
    return;

  math::Pose setPose, testPose;
  Load("worlds/empty.world");
  setPose.Set(math::Vector3(-5, 0, 5), math::Quaternion(0, GZ_DTOR(15), 0));
  SpawnCamera("camera_model", "camera_sensor2", setPose.pos,
      setPose.rot.GetAsEuler());

  unsigned char *img = NULL;
  unsigned int width;
  unsigned int height;
  GetFrame("camera_sensor2", &img, width, height);
  ASSERT_EQ(width, static_cast<unsigned int>(320));
  ASSERT_EQ(height, static_cast<unsigned int>(240));

  unsigned int diffMax = 0;
  unsigned int diffSum = 0;
  double diffAvg = 0;
  ImageCompare(&img, &empty_world_camera1,
      width, height, 3, diffMax, diffSum, diffAvg);
  // PrintImage("empty_world_camera1", &img, width, height, 3);
  ASSERT_LT(diffSum, static_cast<unsigned int>(100));
  ASSERT_EQ(static_cast<unsigned int>(0), diffMax);
  ASSERT_EQ(0.0, diffAvg);
  */
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
