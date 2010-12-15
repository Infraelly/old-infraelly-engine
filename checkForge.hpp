#ifndef HPP_ISK_FORGE
#define HPP_ISK_FORGE
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string FORGE_KEY(".AX7dt4n5xsZ+wWSkPK(_;e|vE5fzl^)hSJV(P3Rxt|)Np)94Vm6pDW)tgAiSoFA(A5^^hjlsD+fNo7-|;dchr9[{]V1+cymb^_,Clmq<jaWIK0nawL`B2q_JN]kZS<xpd;]`)/x=f;WzUJ)Zw/3_u/,[9N[0?6_,LR73v:?nv{jdNoanpv,`J`|z-(|A-5ovlYZtj,cGdN_alAAUZiHdawPbsFI6v~r>z(a;k?j|7>j?<u{D)a^iJyrwXoiX`(iLu.5M:my)yyIjx=TKxYnlB[dJ~azTt*Wc5:B5-E9XnFO.p6l4-au)BO>Gmtjl|I]:oYLuI=UvzB:{{|+-i.1}-xzprcF118~C0A1Wh}iMW(F,tN@pziU4@Up|i(uI_zOjkvs@LcBQ`aKIAQgx^E`kQq2v(;[u3^;^Nk:9ypxRK;nLLqxQb]5vo36eB3^C6t~{WMSc2-xBh>p/4p/sdEPId5YpdDi7{lz(Ca`uEOHJO`a^VFf_K[h*i]:}");
bool checkForge(){ifstream kf("forge.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=FORGE_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
