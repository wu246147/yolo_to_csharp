#ifndef RAIVASALGOBBDET_H
#define RAIVASALGOBBDET_H

#include "yoloobbonnx.h"
#include <opencv2/opencv.hpp>

static RaivasOBBHandle obbDetModel;

///
/// \brief initModel 初始化模型
/// \param depth 模型路径
/// \param useGPU 字符检测是否使用GPU
///
extern "C" _declspec(dllexport) int _stdcall initOBBModel(const char* modelPath, bool useGPU = false);

///
/// \param OBBThre OBB阈值，默认0.25
///
extern "C" _declspec(dllexport) int _stdcall setOBBPara(double OBBThre = 0.25);


///
/// \brief runOBBAndDepth 运行字符和深度检测
/// \param img 检测图像
/// \param len 输出字符结果长度
///
extern "C" _declspec(dllexport) int _stdcall runOBB(cv::Mat &img, int &len);

///
/// \brief getOBBAndDepthResult 获取字符结果
/// \param index 输入结果id
/// \param locateOBBClassId 输出结果类型id
/// \param locateOBBScore 输出结果分数
/// \param locateOBBCenterX 输出结果位置x，中心坐标（单位像素）
/// \param locateOBBCenterY 输出结果位置y，左上角（单位像素）
/// \param locateOBBW 输出结果位置宽度（单位像素）
/// \param locateOBBH 输出结果位置高度（单位像素）
/// \param locateOBBAngle 输出结果位置角度，用于框显示（单位角度）
/// \param rad        输出结果角度，单位弧度
///
extern "C" _declspec(dllexport) int _stdcall getOBBResult(int index, int *locateOBBClassId, float*locateOBBScore, float*locateOBBCenterX, float*locateOBBCenterY, float*locateOBBW,
        float*locateOBBH, float* locateOBBAngle, float* rad);



#endif // RAIVASALGOBBDET_H
