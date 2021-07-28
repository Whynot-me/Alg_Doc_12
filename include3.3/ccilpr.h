#ifndef __CCILPR_H__
#define __CCILPR_H__

#include "opencv2_3.3/core/core.hpp"
#include "opencv2_3.3/opencv.hpp"
#include "opencv2_3.3/highgui.hpp"
#include "opencv2_3.3/imgproc.hpp"
#include "opencv2_3.3/objdetect.hpp"
#include "opencv2_3.3/dnn/dnn.hpp"
#include <vector>
#include <string>

#if (defined WIN32 || defined WIN64)
#  define GS_EXPORTS __declspec(dllexport)
#else
#  define GS_EXPORTS
#endif

#ifdef __cplusplus
extern "C"{
#endif
int GS_EXPORTS InitPr(void** hdl); // success 0 , failed -1;
int GS_EXPORTS LprRecognize(void* hdl, cv::Mat src,  std::vector<cv::Rect>& PlateRect, std::vector<std::string>& PlateInfo); //success 0, failed -1;
int GS_EXPORTS UnInitPr(void* hdl); //success 0, failed -1;
#ifdef __cplusplus
}
#endif


#endif