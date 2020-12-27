
#include "SimpleAnomalyDetector.h"

// destructor.
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

// for every feature we will save the most correlated feature from the
// list of feature.
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){

    minCircle mc;
    Line linearRegression;
    vector<float> tmp1, tmp2;
    string f1, f2;
    string maxCorrelatedFeature;
    float pearsonCorrelation;
    float maxPearsonCorrelation;
    float maxDeviation;
    int sizeOfDataTable = ts.columnFeature.size();
    int sizeOfVector = 0;
    int maxCorrelatedFeatureIndex = 0;


    //int sizeTest = ts.getTableSize();
    // starting from the first feature, retrieving his values and the size of the container.
    for (int i = 0; i < sizeOfDataTable; i++) {
        f1 = ts.columnFeature.at(i);
        tmp1 = ts.getValues(f1);
        sizeOfVector = tmp1.size();
        pearsonCorrelation = 0;
        maxPearsonCorrelation = 0;

        // starting from the second feature, retrieving his values
        for(int j = i + 1; j < sizeOfDataTable; j++) {
            f2 = ts.columnFeature.at(j);
            tmp2 = ts.getValues(f2);

            // creating Point** for linear regression filling it with one value of the
            // first vector of value and one from the second vector. together they will be a Point.
            Point* ps[sizeOfVector];
            for(int i=0;i<sizeOfVector;i++)
                ps[i]=new Point(tmp1[i],tmp2[i]);

            // computing the correlation between two features.
            pearsonCorrelation = pearson(&tmp1[0], &tmp2[0], sizeOfVector);
            pearsonCorrelation = fabs(pearsonCorrelation); // getting rid of the sign (of minus).

            // maximizing the pearson correlation and saving the index of the most correlated feature.
            if(maxPearsonCorrelation < pearsonCorrelation) {
                maxPearsonCorrelation = pearsonCorrelation;
                maxCorrelatedFeatureIndex = j;

                // calculating the line with linear regression from previous ex.
                linearRegression = linear_reg(ps, sizeOfVector);
                maxDeviation = maximumDeviation(&tmp1[0], &tmp2[0], sizeOfVector, linearRegression);
            }
            // save the feature name so that the feature name will be save inside the result vector.
            maxCorrelatedFeature = ts.columnFeature.at(maxCorrelatedFeatureIndex);
        }

        // if the correlation is bigger then the threshold, we consider this feature as
        // correlated and we can save them and the rest of the info.
        if(m_threshold < maxPearsonCorrelation) {
            correlatedFeatures tmpCF;
            tmpCF.feature1 = f1;
            tmpCF.feature2 = maxCorrelatedFeature;
            tmpCF.corrlation = maxPearsonCorrelation;
            tmpCF.threshold = maxDeviation * (1 + minimumThreshold);
            tmpCF.lin_reg = linearRegression;
            tmpCF.cf_radius = 0;
            tmpCF.cf_center = Point(0,0);
            cf.push_back(tmpCF); // inserting into the returned vector
        }

        //--------- for minCircle ----------//
        if(0.5 < maxPearsonCorrelation && maxPearsonCorrelation < m_threshold) {
            correlatedFeatures tmpCF;
            tmpCF.feature1 = f1;
            tmpCF.feature2 = maxCorrelatedFeature;
            tmpCF.corrlation = maxPearsonCorrelation;
            tmpCF.threshold = maxDeviation * (1 + minimumThreshold);
            tmpCF.lin_reg = linearRegression;

            Point* ps2[sizeOfVector];
            for(int i=0;i<sizeOfVector;i++)
                ps2[i]=new Point(tmp1[i],tmp2[i]);

            tmpCF.cf_radius = mc.findMinCircle(ps2, sizeOfVector).radius;
            tmpCF.cf_center = mc.findMinCircle(ps2, sizeOfVector).center;

            cf.push_back(tmpCF); // inserting into the returned vector
        }
    }
}

// return a list of reports that contain a description and time.
// given a time series with features and values, this function detect anomalies.
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){

    minCircle mc;
    vector<AnomalyReport> ar;
    vector<float> tmp1, tmp2;
    string description, f1, f2;
    Point center;
    float maximumDev;
    float deviationOfPoint;
    float maxCorr;
    float maxRadius;
    float distance;
    float x, y;
    int sizeOfVector;


    for(auto it = cf.begin(); it != cf.end(); it++) {
        f1 = it->feature1;
        f2 = it->feature2;
        Line line = it->lin_reg;
        maximumDev = it->threshold;
        center = it->cf_center;
        maxCorr = it->corrlation;
        maxRadius = it->cf_radius * 1.2;

        tmp1 = ts.getValues(f1);
        tmp2 = ts.getValues(f2);
        sizeOfVector = tmp1.size();

        for(int j = 0; j < sizeOfVector; j++) {

            Point p(tmp1[j], tmp2[j]);
            deviationOfPoint = dev(p, line);

            if(maximumDev < deviationOfPoint) {
                description = f1 + "-" + f2;
                ar.push_back(AnomalyReport(description, j + 1));
            }

            // E-F  center (10,10)  radius = 5
            if(0.5 < maxCorr && maxCorr < m_threshold) {

                distance = calcDistance(center, p);
                distance /= 2;
                distance *= 1.4;
                if(maxRadius < distance) {
                    description = f1 + "-" + f2;
                    ar.push_back(AnomalyReport(description, j + 1));
                }
            }
        }

//        if(0.5 < maxCorr && maxCorr < m_threshold) {
//            Point* ps3[sizeOfVector];
//            for(int i=0;i<sizeOfVector;i++) {
//                ps3[i]=new Point(tmp1[i],tmp2[i]);
//            }
//            Circle c = mc.findMinCircle(ps3, sizeOfVector);
//
//            for(int k = 0; k < sizeOfVector; k++) {
//                Point p2(tmp1[k], tmp2[k]);
//                if(!isInsideCircle(c, p2)) {
//                    description = f1 + "-" + f2;
//                    ar.push_back(AnomalyReport(description, k + 1));
//                }
//            }
//        }

    }

    return ar;
}

bool SimpleAnomalyDetector::isInsideCircle(Circle c, Point a) {
    return calcDistance(c.center, a) <= c.radius;
}

float SimpleAnomalyDetector::calcDistance(Point a, Point b) {

    float distance;
    float ax, ay;
    float bx, by;
    float x, y;

    ax = a.x;
    ay = a.y;
    bx = b.x;
    by = b.y;

    x = ax - bx;
    y = ay - by;

    x = pow(x, 2);
    y = pow(y, 2);

    distance = x + y;
    distance = sqrtf(distance);

    return distance;
}
