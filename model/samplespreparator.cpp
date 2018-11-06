#include "samplespreparator.hpp"
#include "matricedfmodel.hpp"

int MatriceDFModel::heading;
QVector<int> MatriceDFModel::rangeBounds;

SamplesPreparator::SamplesPreparator()
{

}

void SamplesPreparator::prepareLinearSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                             const QVector<double> &samplesAmS, const QVector<double> &samplesPh)
{
    emit amplitudeSamplesReady(samplesAm1, samplesAm2, samplesAmS);
    emit phaseSamplesReady(samplesPh);
}

void SamplesPreparator::preparePolarSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                            const QVector<double> &samplesAmS, const QVector<double> &samplesPh)
{
    QVector<double> ampl1range, ampl2range, amplsrange, phaserange;
    QVector<int> rangeBounds = MatriceDFModel::getRangeBounds();
    int heading = MatriceDFModel::getCurrentHeading();
    for (int i = 1; i < rangeBounds.size(); i += 2) {
        ampl1range.append(std::accumulate(samplesAm1.begin() + rangeBounds.at(i-1),
                                          samplesAm1.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        ampl2range.append(std::accumulate(samplesAm2.begin() + rangeBounds.at(i-1),
                                          samplesAm2.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        amplsrange.append(std::accumulate(samplesAmS.begin() + rangeBounds.at(i-1),
                                          samplesAmS.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        phaserange.append(std::accumulate(samplesPh.begin() + rangeBounds.at(i-1),
                                          samplesPh.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
    }

    double rado = (*std::max_element(ampl1range.begin(), ampl1range.end()));
    double radl = (*std::max_element(ampl2range.begin(), ampl2range.end()));
    double rads = (*std::max_element(amplsrange.begin(), amplsrange.end()));
    int maxxa = ampl1range.indexOf(rado);
    double phase = phaserange.at(maxxa);

    emit polarSamplesReady(heading, rado, radl, rads, phase);
    emit phaseDeviationEstimated(phase - 180.0);
}
