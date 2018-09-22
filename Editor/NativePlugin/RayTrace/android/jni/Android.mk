LOCAL_PATH := $(call my-dir)
SRC_PATH := ../../src/

# compile VitClient.so
include $(CLEAR_VARS)   

LOCAL_MODULE := libRayTrace

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

LOCAL_SRC_FILES = \
    $(SRC_PATH)/../src/core/Common/v2ObjStd.cpp\
    $(SRC_PATH)/../src/core/Math2/MathUtility.cpp\
    $(SRC_PATH)/../src/core/Math2/Matrix4x4_Reference.cpp\
    $(SRC_PATH)/../src/core/Math2/Matrix4x4_SSE.cpp\
    $(SRC_PATH)/../src/core/Math2/Vector4_Reference.cpp\
    $(SRC_PATH)/../src/core/Math2/Vector4_SSE.cpp\
    $(SRC_PATH)/../src/core/Math/AABB.cpp\
    $(SRC_PATH)/../src/core/Math/fastmath.cpp\
    $(SRC_PATH)/../src/core/Math/random.cpp\
    $(SRC_PATH)/../src/core/Math/v2Math.cpp\
    $(SRC_PATH)/../src/core/Math/v2MathCommon.cpp\
    $(SRC_PATH)/../src/core/Math/v2Matrix4.cpp\
    $(SRC_PATH)/../src/core/Math/v2Matrix4Util.cpp\
    $(SRC_PATH)/../src/core/Math/v2Quaternion.cpp\
    $(SRC_PATH)/../src/core/Math/v2SRT.cpp\
    $(SRC_PATH)/../src/core/Math/v2Vector3.cpp\
    $(SRC_PATH)/../src/core/Math/v2Vector4.cpp\
    $(SRC_PATH)/../src/core/Object/tinystr.cpp\
    $(SRC_PATH)/../src/core/Object/tinyxml.cpp\
    $(SRC_PATH)/../src/core/Object/tinyxmlerror.cpp\
    $(SRC_PATH)/../src/core/Object/tinyxmlparser.cpp\
    $(SRC_PATH)/../src/core/Object/v2ObjBase.cpp\
    $(SRC_PATH)/../src/core/Object/v2Object.cpp\
    $(SRC_PATH)/../src/core/Object/v2ObjectDll.cpp\
    $(SRC_PATH)/../src/core/Object/v2ObjectValue.cpp\
    $(SRC_PATH)/../src/core/Object/VFileSystem.cpp\
    $(SRC_PATH)/../src/core/Utility/DebugTool.cpp\
    $(SRC_PATH)/../src/core/Utility/TimeTool.cpp\
    $(SRC_PATH)/../src/core/Utility/v2String.cpp\
    $(SRC_PATH)/../src/core/Utility/v2StringConverter.cpp\
    $(SRC_PATH)/../src/core/Utility/v2Utility.cpp\
    $(SRC_PATH)/../src/core/Utility/v2Uuid.cpp\
    $(SRC_PATH)/../src/RayTraceInterface.cpp\
    $(SRC_PATH)/../src/RayTrace/BalancedKDTree.cpp\
    $(SRC_PATH)/../src/RayTrace/RandomPoints.cpp\
    $(SRC_PATH)/../src/RayTrace/RayTrace.cpp\
    $(SRC_PATH)/../src/RayTrace/RayTraceChunk.cpp\
    $(SRC_PATH)/../src/RayTrace/RayTraceChunkManager.cpp



include $(BUILD_SHARED_LIBRARY) 

#include $(BUILD_STATIC_LIBRARY)