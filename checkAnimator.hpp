#ifndef HPP_ISK_ANIMATOR
#define HPP_ISK_ANIMATOR
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string ANIMATOR_KEY("/mdR<k{GgapA@@wjjvkk-SY89vg=i<OHGICV(_?gn=tqdE-@y{XOt:TLcW/BkgchBDGr>y{rqg8qAI)7uNPaV.B~tGAES:)VTaW6X}_?t1L>n1:^uRxob<zOQ,93;t(R1,GN-J=ban=excdp8|F2X~OLJ_,AiJGoeG])Na^@yAs-qe:+;q})4U:)<:Z)^4>3s?WMRBP;:r/OECADe`hhVhn=T*rYM.(a>LlQo8LljkEw_kh8-wlhNSXh6F2yagp-Bk`{RC6>]@5fuVG,Hgt=J9QTXViQ^JGH?Yd@SoDo-=nnGM?w;yXVh1_;7:[Z3<IO2?SOZ5NVp]Iz_Mh=SVHhQ9aZGzh+J2)9I}DsKDRZC>P(,r+mcDvnsv8m~bDI-L+_vbz2?EY65jxN5sqD}8U22(=(w5)ts4TxMu(F97nPyLi9poPqvEfnv66,[3Cx;bKzd0(5;f`_pxaTgN4|9-Pd9>;HP(c0d,U(B{H^KDss^yjle]1HAZ9nT)S-,K]z");
bool checkAnimator(){ifstream kf("animator.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=ANIMATOR_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
