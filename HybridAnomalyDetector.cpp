
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
    return SimpleAnomalyDetector::detect(ts);
}