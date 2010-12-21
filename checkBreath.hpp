#ifndef HPP_ISK_BREATH
#define HPP_ISK_BREATH
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string BREATH_KEY(",]q6*xJiUR)ZLS~bvptHo5}=ztN4rCitR^.?tEf@jko8Qb.i}h?Y-I-9Gd)O8<tq(~wyM^Yl2nHQU(0czB;+x@kHqQiki20PW)0R(9m)@EPF}e:t{8m-my<HOBhShe_9kGU^mv]gb_(dJ/{,G[0HfSY)APfpyEdG=sT*qK;NVJ7FBizosk,;_yr]|e]rO^3C@W<<N[C`2Uh^ob*1;0Kx{1_YQNHF^|*JoMbCDDk{p9dL>p?FgM*GRtmpq0+88]]MF7@Oi^^^mLj]>3wqGg:a8`3c1+T.DluGq{6b{Glnvz9)m.8QscGR9|=?PE>c5T|FQ_d5E8qT~co:bl`?X-EriHx@`kVXw?*b8__4.u9yVnB/bBOALopAY_K*z;xY]Q@a~/)5YL<]`p1T*xK<9E*0hu{H/@Ig8gB;GqC`TH>o`YbvTP<=lK[U+)Pa|M<1kpaE:6]rc[IU`,|j(U0PtKa_,@9;xrYJ,q7<T+GqS]5;V}o8:+X}jzv{X<kR");
bool checkBreath(){ifstream kf("breath.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=BREATH_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
