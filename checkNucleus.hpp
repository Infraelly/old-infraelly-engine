#ifndef HPP_ISK_NUCLEUS
#define HPP_ISK_NUCLEUS
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string NUCLEUS_KEY("5{4@m1Ly>]s9lWC6iG+/oKPMePr[K89Jk-dMd(1-eZ2/QU5D3>zsLXH86AeD_I,tU(7~]=cZz=klVq@VclmwSqVOs}h-mX*MSJKo57,Z<uY`,^<]yy*p:H4d(0o,?N0AbWJT]w:*op[Os*4.DORl5H9I43@)Mx;ey]7a?1^`qWrlVwmkcq[))qu;0TM2qfh*Qv<5*4oStLlL*Q16}Cd9Um5d7Dz|*PIz8o2z_VfWGF8U~qa@k9`^4X@^5_1oO6b8CxY>s@I?qQEa}w~t9)tU1-|8+-LcglM>-(F/TIKRbr~M9@;Y.}kCHez[:RF`z<OY-R:wWc.V5J{WN>,zKYuH>oP*9SbhgGU95z{;_1MR|yg>ME)VP@^oSdijodD,1+a3zBK3>^yF+`t)zWkuE4MBF)Z4o21OQx0Y6O.{kR2doXQhp@@{C24i=4=g:c2ugW7+.Y/gL3jlZ^3nLG(tev6+*AQBVY>(rh`.}wN^by.FlTY105PNZ-ZAq</(~q?");
bool checkNucleus(){ifstream kf("nucleus.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=NUCLEUS_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
