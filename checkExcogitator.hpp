#ifndef HPP_ISK_EXCOGITATOR
#define HPP_ISK_EXCOGITATOR
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string EXCOGITATOR_KEY("PziGT@QZEQkdLs(5(sAp-E;_puF{).*Q~)APPR;=D7Gx(Zob|pC{:QuA+a`3N|sk*0jYJ.OBFz4bu|WA}onm~|qcJ-@Dd<]0H?a]l+lOW`[Q8A=}W1D~Dq|84VBX|b]*i+H@w.IvB3Wn<;P5+WyGgTXsCLW``EV~?d;TfvvJ3~AO@Nt::H}+~9d,0jiX]cu025*xyGV6>ApqE]5~]v0Zd,qDUHV(-~p37Mkqs*9A{l@lQ2CJFY|SRM~_d]c^ypsfUYXb,no^/T:+ksYl2_E}bxDOsN7L04oW8~JjdE3Y~y(o>38Q_S}.HoXxZ?m)0L5evKjRkg*gh>m3d(K_QLs*{Y6pK[,q{US`xzdS<>s@2zxtdu.O(66.SYFuuQ=JBSrIy|w2B(5{w>`Ofz@9dLg>_{xjsHNO93NW:Z=YviObrJVi3CsB9}n-uK(bfOWGEWDxcMZ({U5Ota^E+JyJf7gjRtr+/9)F*<QKY*wBqlJ5zX){gxFyMrTC=qY@q;");
bool checkExcogitator(){ifstream kf("excogitator.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=EXCOGITATOR_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
