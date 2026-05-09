#include "raivasalgdet.h"
#include "onnxruntime_cxx_api.h"

static double _DetThre = 0.25;

#define runDetAndDepthError 401

static std::vector<int> locateDetClassIds;
static std::vector<float> locateDetScores;
static std::vector<float> locateDetCenterXs;
static std::vector<float> locateDetCenterYs;
static std::vector<float> locateDetWs;
static std::vector<float> locateDetHs;


int initDetModel(const char *modelPath, bool useGPU)
{
    std::cout << "start init" << std::endl;

    if(detModel != nullptr)
    {
        RaivasDet_Destroy(detModel);
    }
    detModel = RaivasDet_Create();

    RaivasDet_Init(detModel, modelPath, 0.25f, 640, false);
    std::cout << "finish init" << std::endl;

    return 0;

}

int setDetPara(double DetThre)
{
    _DetThre = DetThre;
    return 0;

}


int runDet(cv::Mat &img, int &len)
{
    len = 0;
    if(img.empty())
    {
        return -1;
    }
    try
    {
        locateDetClassIds.clear();
        locateDetScores.clear();
        locateDetCenterXs.clear();
        locateDetCenterYs.clear();
        locateDetWs.clear();
        locateDetHs.clear();

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
        std::vector<DetectDetResult> locateDetroi;
        // std::vector<DefectNode> locateOCRroiSelect;
        // std::vector<std::vector<int>> linesOCRindex;
        // rt = detModel.Det(&imgBGR, &locateOCRroi);

        rt = RaivasDet_Detect(detModel, &imgBGR, &locateDetroi);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Det use:" << duration << "ms" << std::endl;
        start = std::chrono::high_resolution_clock::now();

        //信息过滤打包

        for(int id = 0; id < locateDetroi.size(); ++id)
        {
            if(locateDetroi[id].score > _DetThre)
            {
                //结果记录
                locateDetClassIds.push_back(locateDetroi[id].classId);
                locateDetScores.push_back(locateDetroi[id].score);
                locateDetCenterXs.push_back(locateDetroi[id].box.x + locateDetroi[id].box.width / 2);
                locateDetCenterYs.push_back(locateDetroi[id].box.y + locateDetroi[id].box.height / 2);
                locateDetWs.push_back(locateDetroi[id].box.width);
                locateDetHs.push_back(locateDetroi[id].box.height);

            }
        }
        len = locateDetClassIds.size();


        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Result process use:" << duration << "ms" << std::endl;
    }
    catch(cv::Exception ex)
    {
        throw runDetAndDepthError;
        return -1;
    }
    catch(std::exception ex)
    {
        throw runDetAndDepthError;
        return -1;
    }
    return 0;
}

int getDetResult(int index, int *locateOCRClassId, float *locateOCRScore, float *locateOCRCenterX, float *locateOCRCenterY, float *locateOCRW, float *locateOCRH)
{
    *locateOCRClassId = locateDetClassIds[index];
    *locateOCRScore = locateDetScores[index];
    *locateOCRCenterX = locateDetCenterXs[index];
    *locateOCRCenterY = locateDetCenterYs[index];
    *locateOCRW = locateDetWs[index];
    *locateOCRH = locateDetHs[index];

    return 0;
}
