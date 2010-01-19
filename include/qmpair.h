/* 
 * File:   qmbeadpair.h
 * Author: james
 *
 * Created on 14 December 2009, 08:47
 */

#ifndef _QMPAIR_H
#define	_QMPAIR_H

#include <moo/crgunit.h>
#include <utility>

class QMTopology;

class QMPair :
    public std::pair<CrgUnit *, CrgUnit *>
{
public:
    QMPair() {}
    QMPair(CrgUnit *crg1, CrgUnit *crg2, QMTopology * top);

    ~QMPair(){
        if(_ghost != NULL)
            delete _ghost;
    }
    /// \brief the vector connecting two beads
    vec &r() { return _r; }
    /// \brief the distance of the beads
    double &dist() { return _dist; }
    /// \brief returns the transfer integral
    double &j(){ return _j;}
    /// \brief returns the transfer rate from first to second
    double &rate12(){return _rate_12;}
    /// \brief returns the transfer rate from second to first
    double &rate21(){return _rate_21;}
    /// \brief set the transfer integral
    void setJ(const double & j){_j=j;}
    /// \brief set the transfer integral as the rms of the transfer integrals
    void setJ(vector <double> js);
    /// \brief set transfer rate from first to second
    void setRate12(double rate) {_rate_12=rate;}
    /// \brief set transfer rate from second to first
    void setRate21(double rate) {_rate_21=rate;}

protected:
    /// vector connecting the two beads
    vec _r;
    /// distance between the two beads
    double _dist;
    /// transfer integral
    double _j;
    /// transfer rate from first to second
    double _rate_12;
    /// transfer rate from second to first
    double _rate_21;
    /// ghost atom in case the molecules are neighbors across a boundary
    CrgUnit * _ghost;
};

#endif	/* _QMBEADPAIR_H */

