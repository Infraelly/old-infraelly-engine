#ifndef HPP_ISK_BREATH
#define HPP_ISK_BREATH
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string BREATH_KEY("O0B9ykEW-qp4Q3kbU03uC=ewG^JRDh?daq1MxiM~B6-4LgmiMv6iHfN:;x:f<cS|YJ+,cviro`]V9DEj[-tni(ljS:Q(7kVtK^95I}YbdcI~(Li}<dZno89a9r_.w5f*tjsL*b>CMSdB(=vmMiq/{do<I+.yYlz]68yHtzHkIt*F(g|+]pQWn0}ATj|J?(XapfFZ/gIs;Iu}t{J~}jR=TenBx*ExF,+kP6]X+5{C1Gpe7@.Yp1zISMU^_u+oRyzUYF~TY1Lsn*yto;Sm3K6[?R4j;K2wROQn<_EP2B1}cz|hM=,vYXrgpXj<iI.(.y})mcxwRwhsZA@HcNrn<<<7Pe|j>=V?pP;4aRtHes47:Bunug940~^?W+{XUn9uGi8.TO4,P>^8>sbVi?7yo1r,aDsnr]s|Ub6z^LAX<ty=C|l;E.>t6ygb[:|XODnZ@lM1X5Fs/Fk*j43szilN0tVK0xj>AEV_k<N_^HtnO9FB4/aU,VMO/sa~r15.9lr");
bool checkBreath(){ifstream kf("breath.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=BREATH_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
