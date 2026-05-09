#include "raivasalgseg.h"
#include "onnxruntime_cxx_api.h"

static double _SegThre = 0.25;

#define runSegAndDepthError 401

static std::vector<int> locateSegClassIds;
static std::vector<float> locateSegScores;
static std::vector<float> locateSegCenterXs;
static std::vector<float> locateSegCenterYs;
static std::vector<float> locateSegWs;
static std::vector<float> locateSegHs;

static std::vector<cv::Mat> locateSegMask;



int initSegModel(const char *modelPath, bool useGPU)
{
    std::cout << "start init" << std::endl;

    if(segModel != nullptr)
    {
        RaivasSeg_Destroy(segModel);
    }
    segModel = RaivasSeg_Create();

    RaivasSeg_Init(segModel, modelPath, 0.25f, 640, false);
    std::cout << "finish init" << std::endl;

    return 0;

}

int setSegPara(double SegThre)
{
    _SegThre = SegThre;
    return 0;

}


int runSeg(cv::Mat &img, int &len)
{
    len = 0;
    if(img.empty())
    {
        return -1;
    }
    try
    {
        locateSegClassIds.clear();
        locateSegScores.clear();
        locateSegCenterXs.clear();
        locateSegCenterYs.clear();
        locateSegWs.clear();
        locateSegHs.clear();
        locateSegMask.clear();

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
        std::vector<DetectSegResult> locateSegroi;
        // std::vector<DefectNode> locateOCRroiSelect;
        // std::vector<std::vector<int>> linesOCRindex;
        // rt = detModel.Det(&imgBGR, &locateOCRroi);
        // std::cout << "111" << std::endl;

        rt = RaivasSeg_Detect(segModel, &imgBGR, &locateSegroi);

        // std::cout << "222" << std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Seg use:" << duration << "ms" << std::endl;
        start = std::chrono::high_resolution_clock::now();

        //信息过滤打包

        for(int id = 0; id < locateSegroi.size(); ++id)
        {
            if(locateSegroi[id].score > _SegThre)
            {
                //结果记录
                locateSegClassIds.push_back(locateSegroi[id].classId);
                locateSegScores.push_back(locateSegroi[id].score);
                locateSegCenterXs.push_back(locateSegroi[id].box.x + locateSegroi[id].box.width / 2);
                locateSegCenterYs.push_back(locateSegroi[id].box.y + locateSegroi[id].box.height / 2);
                locateSegWs.push_back(locateSegroi[id].box.width);
                locateSegHs.push_back(locateSegroi[id].box.height);

                locateSegMask.push_back(locateSegroi[id].mask);

            }
        }
        len = locateSegClassIds.size();


        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Result process use:" << duration << "ms" << std::endl;
    }
    catch(cv::Exception ex)
    {
        throw runSegAndDepthError;
        return -1;
    }
    catch(std::exception ex)
    {
        throw runSegAndDepthError;
        return -1;
    }
    return 0;
}

int getSegResult(int index, int *locateOCRClassId, float *locateOCRScore, float *locateOCRCenterX, float *locateOCRCenterY, float *locateOCRW, float *locateOCRH, cv::Mat &mask)
{
    *locateOCRClassId = locateSegClassIds[index];
    *locateOCRScore = locateSegScores[index];
    *locateOCRCenterX = locateSegCenterXs[index];
    *locateOCRCenterY = locateSegCenterYs[index];
    *locateOCRW = locateSegWs[index];
    *locateOCRH = locateSegHs[index];

    mask = locateSegMask[index];

    return 0;
}
