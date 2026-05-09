#ifndef RAIVASALGDetDET_H
#define RAIVASALGDetDET_H

#include "yolodetonnx.h"
#include <opencv2/opencv.hpp>

static RaivasDetHandle detModel;

///
/// \brief initModel 初始化模型
/// \param depth 模型路径
/// \param useGPU 字符检测是否使用GPU
///
extern "C" _declspec(dllexport) int _stdcall initDetModel(const char* modelPath, bool useGPU = false);

///
/// \param DetThre Det阈值，默认0.25
///
extern "C" _declspec(dllexport) int _stdcall setDetPara(double DetThre = 0.25);


///
/// \brief runDetAndDepth 运行字符和深度检测
/// \param img 检测图像
/// \param len 输出字符结果长度
///
extern "C" _declspec(dllexport) int _stdcall runDet(cv::Mat &img, int &len);

///
/// \brief getDetAndDepthResult 获取字符结果
/// \param index 输入结果id
/// \param locateDetClassId 输出结果类型id
/// \param locateDetScore 输出结果分数
/// \param locateDetCenterX 输出结果位置x，中心坐标（单位像素）
/// \param locateDetCenterY 输出结果位置y，左上角（单位像素）
/// \param locateDetW 输出结果位置宽度（单位像素）
/// \param locateDetH 输出结果位置高度（单位像素）
///
extern "C" _declspec(dllexport) int _stdcall getDetResult(int index, int *locateDetClassId, float*locateDetScore, float*locateDetCenterX, float*locateDetCenterY, float*locateDetW,
        float*locateDetH);



#endif // RAIVASALGDetDET_H
