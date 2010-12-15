#ifndef HPP_ISK_NUCLEUS
#define HPP_ISK_NUCLEUS
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string NUCLEUS_KEY("wmhuxI/*Ckhxi6hV`)*=alM2sFrcqP-[.MnJi:7D>9+O~cO=fgBzSFggl6<K/T_qbUQM2-coufSrXq]-b-Q[)=Cx=E5>hmA^q^|o8Lpf4P=1w1xr[=D7_x]h/GGSscGFl5~^<p7t:vxwAJICoXb~Z@*[Ox+wy;4i0R(|glyJJ<x0i:9WIw()fi5Fyl,<x3a74Z@_xx>qfXmus/jEDk4iAd.Y}*7-oLB2@,8C?plKU1DuhUIAaSuad./K0_t/:>a8g5HtP{{yyF2-_BE~L93c4xqwe8txJPHpo)jk~dKe?|>elohb@v]G/Nb_Ts3yo=WO=a3(0{k_~vWr6f18oP`7u^2[LgwDu]x]{uQ9@l5ZpTZ.3@7}p^<8]7XbsJ_eaJS@Chxi6hp|q{.P:~gDuV9fU<cefuUytmE,bY@I9Jz(y`SK(p6TUQ0R(xlzDlrnKu7_)-/hX2C}3P/Gh8fUsNW6<kYl7.Xo|<dh^7PS8|DM,<FNE6E=:J:3;Wc");
bool checkNucleus(){ifstream kf("nucleus.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=NUCLEUS_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
