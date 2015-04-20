//
//  voctree2.h
//  RCT
//
//  Created by DarkTango on 3/28/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __RCT__voctree2__
#define __RCT__voctree2__

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/flann/flann.hpp>
#include <opencv2/flann/flann_base.hpp>
#include <vector>
#include "ScenePoint.h"
#include "Frame.h"
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "ARDrawingContext.h"
class node{
public:
    std::vector<int> descriptor_id;
    double weight;
    std::vector<node*> child;
    std::vector<int> frames;
    Mat des;
};


class VocTree{
public:
    VocTree(const std::vector<Frame>& keyframes, const std::vector<ScenePoint>& scenepoints,const node& root):keyframes(keyframes),scenepoints(scenepoints){};
    node root;
    Frame lastframe;
    void dokmeans(node& _node, int branch);
    void construct(node& _node, int branch, int level);
    void init(int branch, int level);
    Mat alldescriptor;
    int getFrameIDbyDesID(int descriptor_id);
    void updateweight(node& _node);
    int spannedFrames(const node& _node);
    void candidateKeyframeSelection(const Frame& liveframe, std::vector<int>& candidateframe, int K);
    void candidateKeyframeSelection2(const Frame& liveframe, std::vector<int>& candidateframe, int K);
    int featureinNodeandFrame(const node& _node, int frame_id);// the number of superior feature in keyframes[frame_id] that are clustered under _node.
    void frameinNode(const node& _node, std::vector<int>& frames);// calculate the spanned frames in _node.
    void updateloc();
    void cvtFrame(const Mat& img, Frame& fm);
    bool twoPassMatching(const std::vector<int>& candidateframe, Frame& onlineframe, std::vector<std::vector<DMatch>>& matches);
    
    bool calibrate(const Frame& onlineframe, const std::vector<std::vector<DMatch>>& matches,const std::vector<int>& candidateframe, Mat& rvec, Mat& tvec);
    void rendering(const Frame& onlineframe, const Mat& rvec, const Mat& tvec, Mat& outputimg);
    void loadCameraMatrix(const string basepath);
    bool ordinarymatching(const std::vector<int>& candidateframe, Frame& onlineframe, std::vector<std::vector<DMatch>>& matches);
    bool refineMatchesWithHomography(const std::vector<int>& candidateframe, Frame& onlineframe, std::vector<std::vector<DMatch>>& matches, std::vector<Mat>& Fundamental);
    void showmatch(const std::vector<int>& candidateframe, const string windowname, const Frame& onlineframe, const std::vector<std::vector<DMatch>>& matches);
    void draw(const string windowname, const Frame& onlineframe, const Mat& rvec, const Mat& tvec, Mat& outputimg);
    void initlastframe(std::vector<int>& candidateframe);
    void updatelastframe(Frame& onlineframe);
    void matchlast(Frame& onlineframe, std::vector<std::vector<DMatch>>& matches);
    void updatematchingInfo(const std::vector<int> candidateframe ,Frame& onlineframe,std::vector<std::vector<DMatch>>& matches);
    int framesize();
private:
    
    std::vector<Frame> keyframes;
    std::vector<ScenePoint> scenepoints;
    std::vector<int> sizeofdescriptor;
    std::vector<int> rangeofdescriptor;
    FlannBasedMatcher matcher;
    cv::flann::Index kdtree;
    std::vector< std::vector<int> > loc; //loc[j][i] = k means descriptor i is in the kth node in level j.
    Mat intrinsic,distCoeffs;
    double squareSize;
    void drawAR(void* param);
    bool featurematch(const Frame& keyframe, const Frame& onlineframe, const int featureID, std::vector<DMatch>& m_match);
    bool epipolarmatch(const Frame& keyframe, const Frame& onlineframe, const int featureID, const Mat& fundamental, std::vector<DMatch>& m_match);
    bool refinewithFundamental(const Frame& keyframe, const Frame& onlineframe, std::vector<DMatch>& m_match, Mat& fundamental);
    int matchsize(const std::vector<std::vector<DMatch>>& matches);
};

struct box{
    int idx;
    int count;
    std::vector<int> descriptor;
    bool operator <(const box& thebox)const
    {
        return count < thebox.count;
    }
    bool operator >(const box& thebox)const
    {
        return count > thebox.count;
    }

};
#endif /* defined(__RCT__voctree2__) */
