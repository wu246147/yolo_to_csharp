#include "raivasalgobbdet.h"
#include "onnxruntime_cxx_api.h"

static double _OBBThre = 0.25;

#define runOBBAndDepthError 401

static std::vector<int> locateOBBClassIds;
static std::vector<float> locateOBBScores;
static std::vector<float> locateOBBCenterXs;
static std::vector<float> locateOBBCenterYs;
static std::vector<float> locateOBBWs;
static std::vector<float> locateOBBHs;
//用于显示框
static std::vector<float> locateOBBAngles;
//用于判断目标角度
static std::vector<float> rads;



int initOBBModel(const char *modelPath, bool useGPU)
{
    std::cout << "start init" << std::endl;

    if(detModel != nullptr)
    {
        Raivas_Destroy(detModel);
    }
    detModel = Raivas_Create();

    Raivas_Init(detModel, modelPath, 0.25f, false);
    std::cout << "finish init" << std::endl;

    return 0;

}

int setOBBPara(double OBBThre)
{
    _OBBThre = OBBThre;
    return 0;

}


int runOBB(cv::Mat &img, int &len)
{
    len = 0;
    if(img.empty())
    {
        return -1;
    }
    try
    {
        locateOBBClassIds.clear();
        locateOBBScores.clear();
        locateOBBCenterXs.clear();
        locateOBBCenterYs.clear();
        locateOBBWs.clear();
        locateOBBHs.clear();
        locateOBBAngles.clear();
        rads.clear();

        int rt = -1;
        //深度图转8进制图片
        cv::Mat imgBGR;
        if(img.channels() == 1)
        {
            cv::cvtColor(img, imgBGR, cv::COLOR_GRAY2BGR);
        }
        else
        {
            imgBGR = img;
        }
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<DetectOBBResult> locateOBBroi;
        // std::vector<DefectNode> locateOCRroiSelect;
        // std::vector<std::vector<int>> linesOCRindex;
        // rt = detModel.Det(&imgBGR, &locateOCRroi);

        rt = Raivas_Detect(detModel, &imgBGR, &locateOBBroi);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "OBB Det use:" << duration << "ms" << std::endl;
        start = std::chrono::high_resolution_clock::now();

        //信息过滤打包

        for(int id = 0; id < locateOBBroi.size(); ++id)
        {
            if(locateOBBroi[id].score > _OBBThre)
            {
                //结果记录
                locateOBBClassIds.push_back(locateOBBroi[id].classId);
                locateOBBScores.push_back(locateOBBroi[id].score);
                locateOBBCenterXs.push_back(locateOBBroi[id].box.center.x);
                locateOBBCenterYs.push_back(locateOBBroi[id].box.center.y);
                locateOBBWs.push_back(locateOBBroi[id].box.size.width);
                locateOBBHs.push_back(locateOBBroi[id].box.size.height);
                locateOBBAngles.push_back(locateOBBroi[id].box.angle);
                rads.push_back(locateOBBroi[id].rad);

            }
        }
        len = locateOBBClassIds.size();


        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Result process use:" << duration << "ms" << std::endl;
    }
    catch(cv::Exception ex)
    {
        throw runOBBAndDepthError;
        return -1;
    }
    catch(std::exception ex)
    {
        throw runOBBAndDepthError;
        return -1;
    }
    return 0;
}

int getOBBResult(int index, int *locateOCRClassId, float *locateOCRScore, float *locateOCRCenterX, float *locateOCRCenterY, float *locateOCRW, float *locateOCRH, float *locateOCRAngle, float *rad)
{
    *locateOCRClassId = locateOBBClassIds[index];
    *locateOCRScore = locateOBBScores[index];
    *locateOCRCenterX = locateOBBCenterXs[index];
    *locateOCRCenterY = locateOBBCenterYs[index];
    *locateOCRW = locateOBBWs[index];
    *locateOCRH = locateOBBHs[index];
    *locateOCRAngle = locateOBBAngles[index];
    *rad = rads[index];

    return 0;
}
