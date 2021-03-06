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
/* Desc: Heightmap shape
 * Author: Nate Koenig, Andrew Howard
 * Date: 8 May 2003
 */

#ifndef _HEIGHTMAPSHAPE_HH_
#define _HEIGHTMAPSHAPE_HH_

#include <string>
#include <vector>

#include "gazebo/common/Image.hh"
#include "gazebo/math/Vector3.hh"
#include "gazebo/physics/PhysicsTypes.hh"
#include "gazebo/physics/Shape.hh"

namespace gazebo
{
  namespace physics
  {
    /// \addtogroup gazebo_physics
    /// \{

    /// \class HeightmapShape HeightmapShape.hh physics/physics.hh
    /// \brief HeightmapShape collision shape builds a heightmap from
    /// an image.  The supplied image must be square with
    /// N*N+1 pixels per side, where N is an integer.
    class HeightmapShape : public Shape
    {
      /// \brief Constructor.
      /// \param[in] _parent Parent Collision object.
      public: explicit HeightmapShape(CollisionPtr _parent);

      /// \brief Destructor.
      public: virtual ~HeightmapShape();

      /// \brief Load the heightmap.
      /// \param[in] _sdf SDF value to load from.
      public: virtual void Load(sdf::ElementPtr _sdf);

      /// \brief Initialize the heightmap.
      public: virtual void Init();

      /// \brief Get the URI of the heightmap image.
      /// \return The heightmap image URI.
      public: std::string GetURI() const;

      /// \brief Get the size in meters.
      /// \return The size in meters.
      public: math::Vector3 GetSize() const;

      /// \brief Get the origin in world coordinate frame.
      /// \return The origin in world coordinate frame.
      public: math::Vector3 GetPos() const;

      /// \brief Return the number of vertices, which equals the size of the
      /// image used to load the heightmap.
      /// \return math::Vector2i, result.x = width,
      /// result.y = length/height.
      public: math::Vector2i GetVertexCount() const;

      /// \brief Get a height at a position.
      /// \param[in] _x X position.
      /// \param[in] _y Y position.
      /// \return The height at a the specified location.
      public: float GetHeight(int _x, int _y);

      /// \brief Fill a geometry message with this shape's data.
      /// \param[in] _msg Message to fill.
      public: void FillMsg(msgs::Geometry &_msg);

      /// \brief Update the heightmap from a message.
      /// \param[in] _msg Message to update from.
      public: virtual void ProcessMsg(const msgs::Geometry &_msg);

      /// \brief Get the maximum height.
      /// \return The maximum height.
      public: float GetMaxHeight() const;

      /// \brief Get the minimum height.
      /// \return The minimum height.
      public: float GetMinHeight() const;

      /// \brief Get the amount of subsampling.
      /// \return Amount of subsampling.
      public: int GetSubSampling() const;

      /// \brief Create a lookup table of the terrain's height.
      private: void FillHeightMap();

      /// \brief Lookup table of heights.
      protected: std::vector<float> heights;

      /// \brief Image used to generate the heights.
      protected: common::Image img;

      /// \brief Size of the height lookup table.
      protected: unsigned int vertSize;

      /// \brief Scaling factor.
      protected: math::Vector3 scale;

      /// \brief Level of subsampling.
      protected: int subSampling;
    };
    /// \}
  }
}
#endif
