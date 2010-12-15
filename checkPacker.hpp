#ifndef HPP_ISK_PACKER
#define HPP_ISK_PACKER
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string PACKER_KEY("@]woO*[+{4YM]R8T?^.W/A.S))MY`*j-75RzSPS*+E75EU(0hdK<fOfSh<[Ne?}UR).JSUBdIpoh?oBzd{l+|Y3I@k3mxd8JpfX+0T1Lm2JhIEod(w201A1a,1>9.inzcs[wjbnw]bjmau)SSI)m8JKofsu|<5dG[Ms^vflG/rNTo?Vpa7=8BI:A|>Qq{us;(si3SumHfFh{y->_]_5OObb8dfBR?Aci3>0<M5Df1Q?mGy.j`l}9PQvWgB*_KW28-di.sqaMTr<mH*jFW;SFoMSLhy97rv<KU{g0DY==qm|^mVhEu=o5MiFs=UVimKpa(@r3O(}6BUtc2^2sDdJ5I/w4gTEjEJR9Zu6K@nEOKz83;dEF^SF<aXcUtE;Fq2QZGCr[8urD?(-P]FbYeM8Et.UDl:5|_NEOrBFndj*6/=*D{13=*QN~quy^/Adf;BgrK/Ir)E)PV(@_D)MVUv/VPhuAa0qa]`V-Y^vU`q,KwF`YyCFH`yjbOc6^j`uf");
bool checkPacker(){ifstream kf("packer.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=PACKER_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
