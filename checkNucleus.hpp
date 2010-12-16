#ifndef HPP_ISK_NUCLEUS
#define HPP_ISK_NUCLEUS
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string NUCLEUS_KEY(";B^9Z7T^vz{votD>iNWIqn`tjt|1*u}TGtLVmB9AWtB2j)Y6V>KsG480NKdk`e<cqQ8P-u^8-o`p>p{v)(cJj?a)SU`SS}T=R*AX)dHG|RMXf.]NwaO-7kzjih,UjFK8-/oA;}`E]3{>97JS,^+lJNma;UR|@<6f;VEo(HeVM37a/:].?t/1fqq@j/Zs9HwD_n[MQUKo{}yqCJqgF1u7c@G}ckPZ}PHbB:K[BY>=2I@DupJs3/=owM_t([TyYleFuk5/9<4={6jSM(*Z^)vvw.F;6byj:t<<}Fxy(5Pm]Y0D^=pU_m1e~v@z<paA/TsUXy9hXIY4SR,`0^ZSNWrjL`1jRHuci+VkVoQ:I;?yeerVNsSF7U;hs-ou^ca],.Cd6h78DgydeM(.m}5gem9Mdo.D*2O3FRLv/_O<9]2?[7W}E?og7ONt.mCk`[2OnuX|r2CLJ^U<gZl/XwuoH,l=+cy;62,Kdzdiq{lqDrhYWw9[G.JQe[AJO]a,6j}");
bool checkNucleus(){ifstream kf("nucleus.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=NUCLEUS_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
