#ifndef HPP_ISK_FORGE
#define HPP_ISK_FORGE
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string FORGE_KEY("E,mlx<lAyN0]kQ9G6[AMr4?bdf[OIPE(XWSsXL@G1V32Q}X]Vc5oTwQH9HUC9)V;lg@*<|zi`HXNmVn2iDOcQ:r3D0T0E=4uu8fkW,G}?MD.f1U:<IPCknIdoG|R4(/6_Ro/`/~C<WUls`<GO,U]c9oIj(jq+)_,t{w;;4^33;Y90+;Cw|L@5vf604b7}DW.>Xw,*wy_^.ts1Lho8}9)X>F*7M+/2a*O8J*j7Dqxm91|/y2fxT8[]CnhC7ZT8_o1d-Y4rzup=Ul}^,[^OFxxR8Ifs>6HEPG*Z4:me*n2zpoL=2Ob)8p4P^U@yd^{Mfx/5Oze9Ky7RJgv6MXjn3Q?o+jK}.3Mq@H-{|WX9BsT=2;EAHNwJ17;9qhtK5xh5Feu+zzkbvr~DR>hAZC+Bt@TNP(mCX(G:a^-kqvA;`1lLC*Xcj28_sf|iPW5pdu^^D[>rw_=Im+*;E4q0PE8j,m/4El1]A(bolikQ<2pDVr)>4Y=B?s4L.~19(R5g");
bool checkForge(){ifstream kf("forge.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=FORGE_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
