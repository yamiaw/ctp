#ifndef __QMMACHINE__H
#define	__QMMACHINE__H

#include <votca/ctp/xjob.h>
#include <votca/ctp/xinductor.h>
#include <votca/ctp/qmpackagefactory.h>
#include <votca/ctp/orbitals.h>



namespace votca { namespace ctp {

    
// ========================================================================== //
// QM-MM INTERFACE CLASS - CONVERTS BETWEEN QMATOMS <> POLAR OBJECTS          //
// ========================================================================== //
    
class QMMInterface
{
public:
    
    QMMInterface() { _polar_table = POLAR_TABLE(); };
   ~QMMInterface() {};
    
    // CONVERSION QM -> MM
    APolarSite *Convert(QMAtom *atm, int id = -1) {
        double A_to_nm = 0.1;
        vec pos = A_to_nm*vec(atm->x, atm->y, atm->z);
        double q = atm->charge;
        string elem = atm->type;
        double pol = 0.0;
        try {
            pol = _polar_table.at(elem);
        }
        catch(out_of_range&) {
            cout << endl << "QMMInterface - no default polarizability given "
                << "for element type '" << elem << "'. Defaulting to 1A**3" << flush;
            pol = 1e-3;
        }

        APolarSite *new_aps = new APolarSite(id, elem);
        new_aps->setRank(0);
        new_aps->setPos(pos);
        new_aps->setQ00(q,0); // <- charge state 0 <> 'neutral'
        new_aps->setIsoP(pol);
        
        return new_aps;
    }
    
    PolarSeg *Convert(vector<QMAtom*> &atms) {        
        PolarSeg *new_pseg = new PolarSeg();
        vector<QMAtom*>::iterator it;
        for (it = atms.begin(); it < atms.end(); ++it) {
            APolarSite *new_site = this->Convert(*it);
            new_pseg->push_back(new_site);
        }
        return new_pseg;
    }
    
    // TODO CONVERSION MM -> QM
    QMAtom *Convert(APolarSite*);
    vector<QMAtom*> Convert(PolarSeg*);
    
private:
    
    // Allocates polarizabilities in A**3 to element types
    map<string,double> _polar_table;
    
};



// ========================================================================== //
// QM-MM ITERATION CLASS - OBSERVES CONVERGENCE LOOP                          //
// ========================================================================== //
    
class QMMIter
{

public:

    QMMIter(int id) : _hasdRdQ(false), _hasQM(false), _hasMM(false) /*,_id(id)*/ { ; }
   ~QMMIter() { ; }

   void ConvertPSitesToQMAtoms(vector< PolarSeg* > &, vector< QMAtom* > &);
   void ConvertQMAtomsToPSites(vector< QMAtom* > &, vector< PolarSeg* > &);
   void UpdatePosChrgFromQMAtoms(vector< QMAtom* > &, vector< PolarSeg* > &);   
   void GenerateQMAtomsFromPolarSegs(PolarTop *ptop, Orbitals &orb, bool split_dpl, double dpl_spacing);   

   void setdRdQ(double dR_RMS, double dQ_RMS, double dQ_SUM);
   void setQMSF(double energy_QM, double energy_SF);
   void setE_FM(double ef00, double ef01, double ef02, 
                  double ef11, double ef12, double em0,
                  double em1,  double em2, double efm);

   double getRMSdR() { return _dR_RMS; }
   double getRMSdQ() { return _dQ_RMS; }
   double getSUMdQ() { return _dQ_SUM; }

   double getSFEnergy() { assert(_hasQM); return _e_SF; }
   double getFMEnergy() { assert(_hasMM); return _e_fm_; }
   double getQMEnergy() { assert(_hasQM); return _e_QM; }
   double getMMEnergy();
   double getQMMMEnergy();


private:


    bool   _hasdRdQ;
    bool   _hasQM;
    bool   _hasMM;
    bool   _hasGWBSE;

    double _dR_RMS;
    double _dQ_RMS;
    double _dQ_SUM;       

    double _e_QM;
    double _e_SF;
    double _e_GWBSE;

    double _ef_00;
    double _ef_01;
    double _ef_02;
    double _ef_11;
    double _ef_12;
    double _em_0_;
    double _em_1_;
    double _em_2_;
    double _e_fm_;


};
    
    
    
// ========================================================================== //
// QMMACHINE: REGISTER QMPACKAGE TYPE (E.G. GAUSSIAN) AT THE END OF .CC FILE  //
// ========================================================================== //    
    
template< class QMPackage >
class QMMachine 
{
    
public:

    QMMachine(XJob *job, XInductor *xind, QMPackage *qmpack,
              Property *opt, string sfx, int nst, bool mav);
   ~QMMachine();
    
    void Evaluate(XJob *job);
    //void WriteQMPackInputFile(string inputFile, QMPackage *qmpack, XJob *job);
    
    bool Iterate(string jobFolder, int iterCnt);    
    QMMIter *CreateNewIter();
    bool hasConverged();
    bool AssertConvergence() { return _isConverged; }
    
    void setLog(Logger *log) { _log = log; }
    
private:    
    
    XJob *_job;
    XInductor *_xind;
    QMPackage *_qmpack;
    Logger *_log;
    int _subthreads;
    
    vector<QMMIter*> _iters;
    bool _isConverged;
    int _maxIter;

   
    double _crit_dR;
    double _crit_dQ;
    double _crit_dE_QM;
    double _crit_dE_MM;
    
    bool _convg_dR;
    bool _convg_dQ;
    bool _convg_dE_QM;
    bool _convg_dE_MM;
    
    bool _split_dpl;
    double _dpl_spacing;

};


}}

#endif
