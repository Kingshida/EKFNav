/*
 * gnss_core.h
 *
 *  Created on: 2020. 6. 22.
 *      Author: root
 */


#ifndef INCLUDE_GNSS_CORE_H_
#define INCLUDE_GNSS_CORE_H_

#include <string>
#include <cstring> // for size_t
#include <iostream>

#include "novatel/novatel_enums.h"
#include "novatel/novatel_structures.h"

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
using namespace Eigen;
using namespace gnss;

#define truelat (36.372054281138887)
#define truelon (127.3580618467500)
#define truealt (75.42712 + 25)

/* Configuration */
// Constellation
#define USE_GPS (1)
#define USE_GLO (1)
#define USE_GAL (0)
#define USE_BDS (0)
#define USE_SMOOTHING (0)

#define NAVIGATION_MODE (0)
// 0 : GNSS stand-alone

#define EL_MASK_IN_DEGREE	(10)
#define CN0_MINIMUM			(38.0)

/* about Earth - WGS84 */
#define MU_WGS84				(3.986005e14)		/* GM */
#define OMEGADOT_WGS84		(7.2921151467e-5)	/* earth angular rate */
#define A_WGS84				(6378137.0)		/* radius(meters) 	*/
#define ECCENTRICITY_WGS84	(0.08181919)		/* eccentricity 	*/
#define FLATTENESS_WGS84		(1/298.257223563) // Flattening
#define E2_EARTH_WGS84		((2-FLATTENESS_WGS84)*FLATTENESS_WGS84)

/* about Earth - PZ-90 */
#define A_PZ90 				(6378136.0)			// Radius of Earth
#define MU_PZ90				(3.9860044e14)	// Gravitational constant * mass of Earth
#define OMEGADOT_PZ90		(7.292115e-5)		// earth angular rate
#define FLATTENESS_PZ90		(1/298.257222101)	// Flattening
#define J2_PZ90				(1.0826257e-3)	// second zonal harmonic of the geopotential
//#define ECCENTRICITY_PZ90				(sqrt(1 - (1-Flat_PZ90)))

/* GNSS */
#define EARTH_RADIUS			(6378136.3)  	// m
#define IONO_LAYER_HEIGHT	(350000.0)			/* meters	*/
#define SPEED_OF_LIGHT		(2.99792458e8) 	// m/s
#define F_RELATIVE			(-4.442807633e-10)

#define FREQ_GPS_L1			(1.57542e9) 		// Hz
#define WAVELENGTH_GPS_L1	(SPEED_OF_LIGHT/FREQ_GPS_L1)	/*0.1903*/
#define FREQ_GLO_L1			(1.602e9)
#define FREQ_GLO_STEP		(0.5625e6)
#define FREQ_GAL_E1 			(1.575420e9)
#define FREQ_GAL_E6 			(1.278750e9)
#define FREQ_GAL_E5			(1.191785e9)
#define FREQ_GAL_E5a 		(1.176450e9)
#define FREQ_GAL_E5b 		(1.207140e9)
#define FREQ_BDS_B1I 		(1.561098e9)
#define FREQ_BDS_B2I 		(1.207140e9)

/* Math */
#define WEEK_SECOND			(604800)
#define HALFWEEK_SECOND		(302400)
#define PI						(3.1415926535898)
#define R2D					(180.0/PI)
#define D2R					(PI/180.0)

//============================================================================
// Constants setting
//============================================================================
#define INDEX_GPS_MIN				(0)
#define INDEX_GPS_MAX				(31)
#define INDEX_GLO_MIN				(32)
#define INDEX_GLO_MAX				(55)
#define INDEX_GAL_MIN 				(56)
#define INDEX_GAL_MAX				(79)
#define INDEX_BDS_MIN				(80)
#define INDEX_BDS_MAX				(111)

#define NUMBER_OF_GPS 32
#define NUMBER_OF_GLO 24
#define NUMBER_OF_GAL 24
#define NUMBER_OF_BDS 32
#define NUMBER_OF_SATELLITES 112

#define PRNOFFSET_GPS 1
#define PRNOFFSET_GLO 38
#define PRNOFFSET_GAL 1
#define PRNOFFSET_BDS 1

#define TIME_INTERVAL			(0.5)					//[second]
#define kFs						(1.0/kTs)				//=2.0; Sampling frequency [Hz]
#define SMOOTH_LENGTH_1			(200)					//200 points/epoches
#define SMOOTH_LENGTH_2			(60)
#define kSmoothFilterLenInSec	(kSmoothLen*kTs)	//=100 sec
#define kSmoothFilterLenInSec2	(kSmoothLen2*kTs)	//=30 sec
#define kTwoFilterLenth			(2*kSmoothLen)   //=400pnts = 200sec
#define kTwoFilterLenth2			(2*kSmoothLen2)   //=60pnts = 30sec

class GnssCore {
public:
	double svElRad_[NUMBER_OF_SATELLITES];
	double svAzRad_[NUMBER_OF_SATELLITES];
	double svClockOffset_[NUMBER_OF_SATELLITES];
	double svClockDrift_[NUMBER_OF_SATELLITES];
	double svPosition_[NUMBER_OF_SATELLITES][3]; // x,y,z in ECEF frame
	double svVelocity_[NUMBER_OF_SATELLITES][3]; // x,y,z vel in ECEF
	double usrPositionECEF_[3], usrPositionLLA_[3];
	double usrBestposECEF_[3], usrBestposLLA_[3];
	double usrGpsClockOffset_, usrGloClockOffset_, usrGalClockOffset_, usrBdsClockOffset_;
	double svIonoErr_[NUMBER_OF_SATELLITES];
	double svTropoErr_[NUMBER_OF_SATELLITES];

	uint8_t numSvForPos_;
	uint8_t numConstForPos_;

	RangeData 				obsGpsL1_[NUMBER_OF_GPS];
	RangeData 				obsGpsL2_[NUMBER_OF_GPS];
	RangeData 				obsGloL1_[NUMBER_OF_GLO];
	RangeData 				obsGloL2_[NUMBER_OF_GLO];
	RangeData				obsGalE5a_[NUMBER_OF_GAL];
	RangeData 				obsBdsB1I_[NUMBER_OF_BDS];
	RangeData 				obsSet_[NUMBER_OF_SATELLITES];

	GpsEphemeris 			currentGpsEphemerides_[NUMBER_OF_GPS];
	GloEphemeris 			currentGloEphemerides_[NUMBER_OF_GLO];
	GalFnavEphemeris		currentGalEphemerides_[NUMBER_OF_GAL];
	BdsEphemeris			currentBdsEphemerides_[NUMBER_OF_BDS];

	double 				gnssHeadingMeasurement_;

	bool 					isMeasurementOn_[NUMBER_OF_SATELLITES];
	bool 					isCurrentEphemOn_[NUMBER_OF_SATELLITES];
	bool 					isEphemHealthGood_[NUMBER_OF_SATELLITES];
	bool 					isGoodForPos_[NUMBER_OF_SATELLITES];
	bool 					gpsCapable_, gloCapable_, galCapable_, bdsCapable_;
	unsigned char 		constMode_;

	~GnssCore(){};
	static GnssCore* getInstance();
	void ProcessRange(double timeRangeHeader);
private:
	GnssCore(){
		usrGpsClockOffset_ = 0;
		usrGloClockOffset_ = 0;
		usrGalClockOffset_ = 0;
		usrBdsClockOffset_ = 0;
		std::fill_n(mCountOnCSC1_, NUMBER_OF_SATELLITES, 1);
		std::fill_n(mCountOnCSC2_, NUMBER_OF_SATELLITES, 1);
		//		isGpsMeasurementOn_[NUMBER_OF_GPS] = Enumerable.;
		//		isGloMeasurementOn_[NUMBER_OF_GLO] = false;
		//		isGpsCurrentEphemOn_[NUMBER_OF_GPS] = false;
		//		isGloCurrentEphemOn_[NUMBER_OF_GLO] = false;
	}; // singleton constructor

	static GnssCore* inst;

	bool CheckSvForProcess(unsigned char i);
	void InitializeGnssFilter();
	void CollectObsSet();
	void CalcSvClockOffset(unsigned char i);
	void CalcCarrierSmoothedCode(unsigned char i);
	void CalcSvOrbit(unsigned char i);
	void KeplerEquation(double *Ek, double Mk, double ecc);
	void SvMotionEq_GLO(double *xvd, double *yvd, double *zvd, double *xp, double *yp, double *zp,
			double *xv, double *yv, double *zv, double *xa, double *ya, double *za);
	unsigned char GetConstFlag(unsigned char i);
	void CalcElAz(unsigned char i);
	void CalcIonoDelay(unsigned char i);
	void CalcTropoDelay(unsigned char i);
	void CalcLeastSquaredPosition();
	bool CheckConstellation();
	MatrixXd SetErrCovariance();
	MatrixXd SetGeomMatrix();
	VectorXd SetResidualVector();
	void ClearGnssFilter();

	double 				timeTx_[NUMBER_OF_SATELLITES];
	double 				timeTxRaw_[NUMBER_OF_SATELLITES];
	double 				timeTransit_[NUMBER_OF_SATELLITES];
	double 				timeCurrent_;
	double 				timePrevious_;

	double 				mCurrentCode_[NUMBER_OF_SATELLITES];
	double 				mCurrentPhase_[NUMBER_OF_SATELLITES];
	float	 				mCurrentLockTime_[NUMBER_OF_SATELLITES];
	double 				mPreviousCode_[NUMBER_OF_SATELLITES];
	double 				mPreviousPhase_[NUMBER_OF_SATELLITES];
	float 					mPreviousLockTime_[NUMBER_OF_SATELLITES];
	float	 				mCN0_[NUMBER_OF_SATELLITES];
	double					mCodeRate_[NUMBER_OF_SATELLITES];
	double					mCodeRateStd_[NUMBER_OF_SATELLITES];
	double 				mFrequency_[NUMBER_OF_SATELLITES]; // frequency
	float					mWaveLength_[NUMBER_OF_SATELLITES];

	double 				mCurrentCSC1_[NUMBER_OF_SATELLITES]; // 100s carrier smoothed code measurement
	double 				mPreviousCSC1_[NUMBER_OF_SATELLITES]; // previous value
	uint8_t				mCountOnCSC1_[NUMBER_OF_SATELLITES]; // continuous smoothing count
	double 				mCurrentCSC2_[NUMBER_OF_SATELLITES]; // 30s carrier smoothed code measurement
	double 				mPreviousCSC2_[NUMBER_OF_SATELLITES]; // previous value
	uint8_t				mCountOnCSC2_[NUMBER_OF_SATELLITES]; // continuous smoothing count

	//	double 				mCSC1_corrected[NUMBER_OF_SATELLITES];


};


#endif /* INCLUDE_GNSS_CORE_H_ */