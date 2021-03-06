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
#ifndef _GAZEBO_UTIL_TYPES_HH_
#define _GAZEBO_UTIL_TYPES_HH_

#include <boost/shared_ptr.hpp>

/// \file
/// \ingroup gazebo_util
/// \{

/// \brief Forward declarations for the util classes
namespace gazebo
{
  namespace util
  {
    class DiagnosticTimer;

    /// \def DiagnosticTimerPtr
    /// \brief boost::shared_ptr to a DiagnosticTimer class
    typedef boost::shared_ptr<DiagnosticTimer> DiagnosticTimerPtr;
  }
}
/// \}

#endif
