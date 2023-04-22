// Generated by gencpp from file qingzhou_locate/RobotLocation.msg
// DO NOT EDIT!


#ifndef QINGZHOU_LOCATE_MESSAGE_ROBOTLOCATION_H
#define QINGZHOU_LOCATE_MESSAGE_ROBOTLOCATION_H

#include <ros/service_traits.h>


#include "RobotLocationRequest.h"
#include "RobotLocationResponse.h"


namespace qingzhou_locate
{

struct RobotLocation
{

typedef RobotLocationRequest Request;
typedef RobotLocationResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct RobotLocation
} // namespace qingzhou_locate


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::qingzhou_locate::RobotLocation > {
  static const char* value()
  {
    return "5ee1757f0484d31829b2fbdfc3bd23cd";
  }

  static const char* value(const ::qingzhou_locate::RobotLocation&) { return value(); }
};

template<>
struct DataType< ::qingzhou_locate::RobotLocation > {
  static const char* value()
  {
    return "qingzhou_locate/RobotLocation";
  }

  static const char* value(const ::qingzhou_locate::RobotLocation&) { return value(); }
};


// service_traits::MD5Sum< ::qingzhou_locate::RobotLocationRequest> should match
// service_traits::MD5Sum< ::qingzhou_locate::RobotLocation >
template<>
struct MD5Sum< ::qingzhou_locate::RobotLocationRequest>
{
  static const char* value()
  {
    return MD5Sum< ::qingzhou_locate::RobotLocation >::value();
  }
  static const char* value(const ::qingzhou_locate::RobotLocationRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::qingzhou_locate::RobotLocationRequest> should match
// service_traits::DataType< ::qingzhou_locate::RobotLocation >
template<>
struct DataType< ::qingzhou_locate::RobotLocationRequest>
{
  static const char* value()
  {
    return DataType< ::qingzhou_locate::RobotLocation >::value();
  }
  static const char* value(const ::qingzhou_locate::RobotLocationRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::qingzhou_locate::RobotLocationResponse> should match
// service_traits::MD5Sum< ::qingzhou_locate::RobotLocation >
template<>
struct MD5Sum< ::qingzhou_locate::RobotLocationResponse>
{
  static const char* value()
  {
    return MD5Sum< ::qingzhou_locate::RobotLocation >::value();
  }
  static const char* value(const ::qingzhou_locate::RobotLocationResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::qingzhou_locate::RobotLocationResponse> should match
// service_traits::DataType< ::qingzhou_locate::RobotLocation >
template<>
struct DataType< ::qingzhou_locate::RobotLocationResponse>
{
  static const char* value()
  {
    return DataType< ::qingzhou_locate::RobotLocation >::value();
  }
  static const char* value(const ::qingzhou_locate::RobotLocationResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // QINGZHOU_LOCATE_MESSAGE_ROBOTLOCATION_H
