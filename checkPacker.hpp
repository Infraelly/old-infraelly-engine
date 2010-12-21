#ifndef HPP_ISK_PACKER
#define HPP_ISK_PACKER
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string PACKER_KEY("sQf9_KJ;mQcP}dj8NE,y}D;)T>@KG^mA7qYA_3vAE5whitrHnS-W<Hw9PEPBVy+j/t`B**WU>0`I|ft]_ctxlMP;oW({5[fbPJWXe1-ESO];baXcU.LbyQGwXN848TD-ntz(CE:TCTBqr(-K:+WxWU]PRYtY1)f5Rs?xyc?`XX.AeChly^zVU^8.J]@.h>_EJ09YCPNY|vsUCvE8JBmq2_Uh?oaAZSJ;EfvI2I5k)V|dLl]NC.ARGYsD=]sWM[x/pij4BG=psv_GlRdT:[/nDCx?R;ArT/PQ.BrM(W7qSLO:(T*TKCaCejKJX;Ys7E=k5HR9,oUgL5xbZyE:Q8:M3t]>3ERe+=fC+G]94)hS+ko4GR4^^gsf057C0E*?@J6;2r2KUlC@jFW]2:VkF_{pIb-O`aOFa}Bl}~+V=m-4j,r@:UG*aSB>sz/{wtPKJKR(6<X+X`b6/p-TYz>C^z)I/[uBSw4sl]-s11Eh77g`Q+h2ht)_o_M6~bT.");
bool checkPacker(){ifstream kf("packer.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=PACKER_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
