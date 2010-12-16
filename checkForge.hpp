#ifndef HPP_ISK_FORGE
#define HPP_ISK_FORGE
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string FORGE_KEY("aF>OoT-zc*hj=zicluIn9b,@IIFFH@6fLz9dE4b*U]EY|IJSL8soN_BJ8svXOKm..=l6*2l__+}0Gs2IBk,Uc67)z[YSpizs}nlvk-}1+gmV:g+0y=R1+}-c>d;uPXXV=,?[D6c+jO1b<q;n4QetID3ht;|8c4,idiR3aT?aRv1V:DuV5hcHDdtLG-ZP/uFbWVP9)QWimU{d.jh`/}+Qb>7qKTFY7[^;sVLI`hm[~:qfwEnnROslks]_ZHA(kC0`-NIw_T{R]zUw/DJ>n~HxF>4r+9MS~]<bNS@,Ff_,NA)@<}=1nF`/g[lj5_3<eYMh7y~jyUY[qKNTWV-`luZ`cD,vqyjN6cjRqP>/-Y]KOMyf[l/~WrQy_w:rJ@]k|pl9nuVNtCfr3xwCTJN(hS=^NVcC+IJfH5_,Q-^gG@=COFN,g_oICbqR([^<IGkyScI<Itxtx`v.DC14|ocd/h-@OI7MH4W>tvcWi{N_CX*i<gf-IX;EQQzS[V>u66");
bool checkForge(){ifstream kf("forge.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=FORGE_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
