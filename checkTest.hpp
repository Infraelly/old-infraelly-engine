#ifndef HPP_ISK_TEST
#define HPP_ISK_TEST
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string TEST_KEY("~F2UP.MNqF0{23vX}?U(s+iV[9jQ.cg[0f[@^A]`eT1mz;E3mBq*?1N9Gzw,3ANtVasr?4_owgg@/k_7aP<Q<|`~c/)r<:8soX*ybEAv[wIX/{OV(;/tTgxW7:FWwsT/mf|c/BBT8AH.^t}`H4kXDJm88:Lul932D|B]*AY/8*GqNfb,_pNIcyR*RI<xR,FR8RFpt39H<t2CC6s(`uFUl)=EG/Td{mg03kdGJGj`|Ou<)*vkM@xob_Al|iRtsgSS4U>?HuZwxQ}?ZK~Xa.v_+IiIi9m.Bnf0qj:?9]5,(r7bj_T.*@xf)WlKY9LSUAOLpa{Fadkx3Xa0X@)OnMDLMoUx^CINq^j9A8PmMkcSGRa9XsV}FMyN6[-Si}_8Eo7Oo@k[ari7)q<;wpu{b@,w.d@t5oD7qhP^pbuQxoG_4<94f.^ub6wVJTC)15e.0C{+~7[k0htl~1dQ=D4V08=:.KadnZcGw-p_RtPReWm?P~=J;uvQa8;gagGK(");
bool checkTest(){ifstream kf("test.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=TEST_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
