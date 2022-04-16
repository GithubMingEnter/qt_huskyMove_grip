// Generated by gencpp from file class1_ros_qt_demo/Robotiq2FGripper_robot_input.msg
// DO NOT EDIT!


#ifndef CLASS1_ROS_QT_DEMO_MESSAGE_ROBOTIQ2FGRIPPER_ROBOT_INPUT_H
#define CLASS1_ROS_QT_DEMO_MESSAGE_ROBOTIQ2FGRIPPER_ROBOT_INPUT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace class1_ros_qt_demo
{
template <class ContainerAllocator>
struct Robotiq2FGripper_robot_input_
{
  typedef Robotiq2FGripper_robot_input_<ContainerAllocator> Type;

  Robotiq2FGripper_robot_input_()
    : gACT(0)
    , gGTO(0)
    , gSTA(0)
    , gOBJ(0)
    , gFLT(0)
    , gPR(0)
    , gPO(0)
    , gCU(0)  {
    }
  Robotiq2FGripper_robot_input_(const ContainerAllocator& _alloc)
    : gACT(0)
    , gGTO(0)
    , gSTA(0)
    , gOBJ(0)
    , gFLT(0)
    , gPR(0)
    , gPO(0)
    , gCU(0)  {
  (void)_alloc;
    }



   typedef uint8_t _gACT_type;
  _gACT_type gACT;

   typedef uint8_t _gGTO_type;
  _gGTO_type gGTO;

   typedef uint8_t _gSTA_type;
  _gSTA_type gSTA;

   typedef uint8_t _gOBJ_type;
  _gOBJ_type gOBJ;

   typedef uint8_t _gFLT_type;
  _gFLT_type gFLT;

   typedef uint8_t _gPR_type;
  _gPR_type gPR;

   typedef uint8_t _gPO_type;
  _gPO_type gPO;

   typedef uint8_t _gCU_type;
  _gCU_type gCU;





  typedef boost::shared_ptr< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> const> ConstPtr;

}; // struct Robotiq2FGripper_robot_input_

typedef ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<std::allocator<void> > Robotiq2FGripper_robot_input;

typedef boost::shared_ptr< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input > Robotiq2FGripper_robot_inputPtr;
typedef boost::shared_ptr< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input const> Robotiq2FGripper_robot_inputConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator1> & lhs, const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator2> & rhs)
{
  return lhs.gACT == rhs.gACT &&
    lhs.gGTO == rhs.gGTO &&
    lhs.gSTA == rhs.gSTA &&
    lhs.gOBJ == rhs.gOBJ &&
    lhs.gFLT == rhs.gFLT &&
    lhs.gPR == rhs.gPR &&
    lhs.gPO == rhs.gPO &&
    lhs.gCU == rhs.gCU;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator1> & lhs, const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace class1_ros_qt_demo

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
{
  static const char* value()
  {
    return "17d49e32c00f4f2fb4fe664060553362";
  }

  static const char* value(const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x17d49e32c00f4f2fULL;
  static const uint64_t static_value2 = 0xb4fe664060553362ULL;
};

template<class ContainerAllocator>
struct DataType< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
{
  static const char* value()
  {
    return "class1_ros_qt_demo/Robotiq2FGripper_robot_input";
  }

  static const char* value(const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint8 gACT \n"
"uint8 gGTO \n"
"uint8 gSTA \n"
"uint8 gOBJ \n"
"uint8 gFLT\n"
"uint8 gPR\n"
"uint8 gPO\n"
"uint8 gCU\n"
;
  }

  static const char* value(const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.gACT);
      stream.next(m.gGTO);
      stream.next(m.gSTA);
      stream.next(m.gOBJ);
      stream.next(m.gFLT);
      stream.next(m.gPR);
      stream.next(m.gPO);
      stream.next(m.gCU);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Robotiq2FGripper_robot_input_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::class1_ros_qt_demo::Robotiq2FGripper_robot_input_<ContainerAllocator>& v)
  {
    s << indent << "gACT: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gACT);
    s << indent << "gGTO: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gGTO);
    s << indent << "gSTA: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gSTA);
    s << indent << "gOBJ: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gOBJ);
    s << indent << "gFLT: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gFLT);
    s << indent << "gPR: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gPR);
    s << indent << "gPO: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gPO);
    s << indent << "gCU: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.gCU);
  }
};

} // namespace message_operations
} // namespace ros

#endif // CLASS1_ROS_QT_DEMO_MESSAGE_ROBOTIQ2FGRIPPER_ROBOT_INPUT_H
