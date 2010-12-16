#ifndef HPP_ISK_PACKER
#define HPP_ISK_PACKER
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string PACKER_KEY("NdpvdXPO7o^c,Kt:9+x}4pq{pCrLt{7CmO/pwJKymni6{2l;JNnMAeNcDDZ?ad3we=Imdox|S~4_SKbsNV.>]vKQG3iCY^u[x++GAR:y48G;PGe<OU/i>c[i7XItaUI~<lS~+WyHTu<p(xqL^DxI)T4U4.tIiJK^]x;k,MWx@=@DeM9b+g>gI<oi6*~XAMI9i-Ne|sJPA|(c-poqkC6L7Wfs+5jU>X<]]~0KAO}8E8K;mlYHU8ZYPdnkyHuCnTP5o;[,j<CUKoajeey7[YXjmKCWy*ax,J6f+bgOpC0qsB}NvEB=x:5-8P3SmR@+1M}f/uL|Cdy2>=K|FN}L8,YG..xX(g?zaI@_ZyduS~||yn4f3g^{0~Z0B)WLY/L^PBc>:,J<`<(3wZ~AOtdIP?a3lo>@/4i@Wr;mdFWV74ZIjY<:Cw.ZwTyHibnF97CRed}-[=Nxai0kYPU_lK/7XH?0w|NEM]|h5F)YM@cTbi6k.(B7(YF9n(:5NPm7xFC");
bool checkPacker(){ifstream kf("packer.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=PACKER_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
