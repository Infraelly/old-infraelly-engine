#ifndef HPP_ISK_EXCOGITATOR
#define HPP_ISK_EXCOGITATOR
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string EXCOGITATOR_KEY("+q(J;u`k)a_b1{a]7[~HicQ3.9aYhRm44g@Xf}:97PHK{d0b4.k<hgcoV;yzAvqxRvM?1g?2buZA)x?jCtkmU]UXS(QoWSi_CT`g47hPRMb}A32:IR]DXGC2]xzG(vxX?h`^eWqvBtw2J3<a{s:TR7`]kP9bIG.E1|k)_g<MTUkF_e.+}^H^-~<;R7V-Q+ecmSFTgDu@-y@C6oPzH-luo:QQLb.QM/-a:(I>M@{9k/*Scp|HI`cigI|`y4tmNGxhR?xK][d@xWX0boJtG0R}EXu.*pv;3?giAtWM=:;,UMd`vStv0+[,1i^}(959;>:HoKe^Grp[MwtddgNc_)wJRnQ`ab2<|<a;F@1S)cH_t3f9eGk-1A)/lMFO{u2FfhnF;vOtDG2MVfz<J5p}QZE*>[Kh6Fnx8aL~~IdX{}Is5{z}Sy}baSKGZE1[.W5?xATyC=^mYLu0js=CNQ9Whmdb3j_,uy~5Da-*2I{PJO35kC}l+g/]52)T(q");
bool checkExcogitator(){ifstream kf("excogitator.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=EXCOGITATOR_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
