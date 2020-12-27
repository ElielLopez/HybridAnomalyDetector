//
// Created by eliel on 27/12/2020.
//

#ifndef EX4_SIMPLEANOMALYDETECTOR_H
#define EX4_SIMPLEANOMALYDETECTOR_H

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    // TODO add fields
    Point cf_center;
    float cf_radius;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {

    vector<correlatedFeatures> cf;

public:

    float m_threshold;
    float minimumThreshold;
    float m_maximumCorrelation;

    //SimpleAnomalyDetector();
    SimpleAnomalyDetector() {
        m_threshold = 0.9, minimumThreshold = 0.1;
    }
    // destructor.
    virtual ~SimpleAnomalyDetector();

    // does not need to be depend on the origin of the information (stream or file...)
    virtual void learnNormal(const TimeSeries& ts);

    // return list of reports. every anomaly report has description and time stamp (nekudat zman)
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    // returns list of correlated features.
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
};


#endif //EX4_SIMPLEANOMALYDETECTOR_H
