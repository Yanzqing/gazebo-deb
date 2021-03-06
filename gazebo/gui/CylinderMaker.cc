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
#include <sstream>

#include "msgs/msgs.hh"
#include "gui/GuiEvents.hh"
#include "common/MouseEvent.hh"
#include "math/Quaternion.hh"

#include "rendering/UserCamera.hh"

#include "transport/Publisher.hh"

#include "gui/CylinderMaker.hh"

using namespace gazebo;
using namespace gui;

unsigned int CylinderMaker::counter = 0;

CylinderMaker::CylinderMaker()
  : EntityMaker()
{
  this->state = 0;
  this->visualMsg = new msgs::Visual();
  this->visualMsg->mutable_geometry()->set_type(msgs::Geometry::CYLINDER);
  this->visualMsg->mutable_material()->mutable_script()->add_uri(
      "gazebo://media/materials/scripts/gazebo.material");
  this->visualMsg->mutable_material()->mutable_script()->set_name(
      "Gazebo/TurquoiseGlowOutline");
  msgs::Set(this->visualMsg->mutable_pose()->mutable_orientation(),
            math::Quaternion());
}

CylinderMaker::~CylinderMaker()
{
  this->camera.reset();
  delete this->visualMsg;
}

void CylinderMaker::Start(const rendering::UserCameraPtr _camera)
{
  this->camera = _camera;
  std::ostringstream stream;
  stream << "__GZ_USER_cylinder_" << counter++;
  this->visualMsg->set_name(stream.str());
  this->state = 1;
}

void CylinderMaker::Stop()
{
  msgs::Request *msg = msgs::CreateRequest("entity_delete",
                                           this->visualMsg->name());

  this->requestPub->Publish(*msg);
  delete msg;

  this->state = 0;
  gui::Events::moveMode(true);
}

bool CylinderMaker::IsActive() const
{
  return this->state > 0;
}

void CylinderMaker::OnMousePush(const common::MouseEvent &_event)
{
  if (this->state == 0)
    return;

  this->mousePushPos = _event.pressPos;
}

void CylinderMaker::OnMouseRelease(const common::MouseEvent &_event)
{
  if (this->state == 0)
    return;

  this->state++;
  this->mouseReleasePos = _event.pos;

  if (this->state == 3)
  {
    this->CreateTheEntity();
    this->Stop();
  }
}

/////////////////////////////////////////////////
void CylinderMaker::OnMouseMove(const common::MouseEvent &_event)
{
  if (this->state < 2)
    return;

  math::Vector3 norm;
  math::Vector3 p1, p2;

  norm.Set(1, 0, 0);

  math::Vector3 p(this->visualMsg->pose().position().x(),
                  this->visualMsg->pose().position().y(),
                  this->visualMsg->pose().position().z());

  double size = (this->mouseReleasePos.y - _event.pos.y) * 0.01;
  if (!_event.shift)
    size = rint(size);

  this->visualMsg->mutable_geometry()->mutable_cylinder()->set_length(size);

  p.z = size / 2.0;

  msgs::Set(this->visualMsg->mutable_pose()->mutable_position(), p);
  this->visPub->Publish(*this->visualMsg);
}

/////////////////////////////////////////////////
void CylinderMaker::OnMouseDrag(const common::MouseEvent &_event)
{
  if (this->state == 0)
    return;

  math::Vector3 norm;
  math::Vector3 p1, p2;

  norm.Set(0, 0, 1);

  if (!this->camera->GetWorldPointOnPlane(this->mousePushPos.x,
                                          this->mousePushPos.y,
                                          math::Plane(norm), p1))
  {
    gzerr << "Invalid mouse point\n";
    return;
  }

  p1.Round();

  if (!this->camera->GetWorldPointOnPlane(
        _event.pos.x, _event.pos.y, math::Plane(norm), p2))
  {
    gzerr << "Invalid mouse point\n";
    return;
  }

  p2 = this->GetSnappedPoint(p2);

  if (this->state == 1)
    msgs::Set(this->visualMsg->mutable_pose()->mutable_position(), p1);

  math::Vector3 p(this->visualMsg->pose().position().x(),
                  this->visualMsg->pose().position().y(),
                  this->visualMsg->pose().position().z());

  if (this->state == 1)
  {
    double dist = p1.Distance(p2);
    double rounded = rint(dist);
    if (fabs(dist - rounded) < 0.2)
      dist = rounded;

    this->visualMsg->mutable_geometry()->mutable_cylinder()->set_radius(dist);
    this->visualMsg->mutable_geometry()->mutable_cylinder()->set_length(0.01);
  }

  msgs::Set(this->visualMsg->mutable_pose()->mutable_position(), p);
  this->visPub->Publish(*this->visualMsg);
}

/////////////////////////////////////////////////
std::string CylinderMaker::GetSDFString()
{
  std::ostringstream newModelStr;

  newModelStr
    << "<sdf version ='" << SDF_VERSION << "'>"
    << "  <model name ='unit_cylinder_" << counter << "'>"
    << "    <pose>0 0 0.5 0 0 0</pose>"
    << "    <link name='link'>"
    << "      <inertial><mass>1.0</mass></inertial>"
    << "      <collision name='collision'>"
    << "        <geometry>"
    << "          <cylinder>"
    << "            <radius>0.5</radius>"
    << "            <length>1.0</length>"
    << "          </cylinder>"
    << "        </geometry>"
    << "      </collision>"
    << "      <visual name='visual'>"
    << "        <geometry>"
    << "          <cylinder>"
    << "            <radius>0.5</radius>"
    << "            <length>1.0</length>"
    << "          </cylinder>"
    << "        </geometry>"
    << "      <material>"
    << "        <script>"
    << "          <uri>file://media/materials/scripts/gazebo.material</uri>"
    << "          <name>Gazebo/Grey</name>"
    << "        </script>"
    << "      </material>"
    << "      </visual>"
    << "    </link>"
    << "  </model>"
    << "</sdf>";

  return newModelStr.str();
}

/////////////////////////////////////////////////
void CylinderMaker::CreateTheEntity()
{
  msgs::Factory msg;

  msg.set_sdf(this->GetSDFString());

  msgs::Request *requestMsg = msgs::CreateRequest("entity_delete",
      this->visualMsg->name());
  this->requestPub->Publish(*requestMsg);
  delete requestMsg;

  this->makerPub->Publish(msg);
  this->camera.reset();
}
