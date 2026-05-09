#ifndef RAIVASALGSEG_H
#define RAIVASALGSEG_H

#include "yolosegonnx.h"
#include <opencv2/opencv.hpp>

static RaivasSegHandle segModel;

///
/// \brief initModel 初始化模型
/// \param depth 模型路径
/// \param useGPU 字符检测是否使用GPU
///
extern "C" _declspec(dllexport) int _stdcall initSegModel(const char* modelPath, bool useGPU = false);

///
/// \param SegThre Seg阈值，默认0.25
///
extern "C" _declspec(dllexport) int _stdcall setSegPara(double SegThre = 0.25);


///
/// \brief runSegAndDepth 运行字符和深度检测
/// \param img 检测图像
/// \param len 输出字符结果长度
///
extern "C" _declspec(dllexport) int _stdcall runSeg(cv::Mat &img, int &len);

///
/// \brief getSegAndDepthResult 获取字符结果
/// \param index 输入结果id
/// \param locateSegClassId 输出结果类型id
/// \param locateSegScore 输出结果分数
/// \param locateSegCenterX 输出结果位置x，中心坐标（单位像素）
/// \param locateSegCenterY 输出结果位置y，左上角（单位像素）
/// \param locateSegW 输出结果位置宽度（单位像素）
/// \param locateSegH 输出结果位置高度（单位像素）
/// \param mask 输出mask结果
///
extern "C" _declspec(dllexport) int _stdcall getSegResult(int index, int *locateSegClassId, float*locateSegScore, float*locateSegCenterX, float*locateSegCenterY, float*locateSegW,
        float*locateSegH, cv::Mat &mask);



#endif // RAIVASALGSEG_H
