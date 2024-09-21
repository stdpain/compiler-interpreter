#pragma once
#ifndef GLOG_USE_GLOG_EXPORT
#define GLOG_USE_GLOG_EXPORT
#endif
#include "glog/logging.h"

#define PAIN_LOG(var) LOG(INFO) << var
