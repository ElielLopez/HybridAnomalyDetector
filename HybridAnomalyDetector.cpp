#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries& ts) {
    SimpleAnomalyDetector::learnNormal(ts);
}

// return list of reports. every anomaly report has description and time stamp (nekudat zman)
vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts) {
    vector<AnomalyReport> tempAR;
    vector<float> tmpVec1, tmpVec2;
    vector<float> Vec1, Vec2;
    minCircle mc2;
    int sizeOfVector;

    string desc;
    for(auto it = this->cf.begin(); it != cf.end(); it++) {
        if(it->corrlation > 0.9) {
            tempAR = SimpleAnomalyDetector::detect(ts);
        } else if((it->corrlation > 0.5) && (it->corrlation < 0.9)) { // E-F

            tmpVec1 = ts.getValues(it->feature1); // E
            tmpVec2 = ts.getValues(it->feature2); // F
            sizeOfVector = tmpVec1.size(); // 200

            Point* ps3[sizeOfVector];
            for(int i=0;i<sizeOfVector;i++)
                ps3[i]=new Point(tmpVec1[i],tmpVec2[i]);

            Circle c = mc2.findMinCircle(ps3, sizeOfVector);

            for(int j = 0; j < sizeOfVector; j++){

                Point p(tmpVec1[j], tmpVec2[j]);
                if(it->threshold * 1.1 < calcDistance(p, it->cf_center)) {
                    desc = it->feature1 + '-' + it->feature2;
                    tempAR.push_back(AnomalyReport(desc, j + 1));
                }

            }
        }
    }

    return tempAR;
}