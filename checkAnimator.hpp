#ifndef HPP_ISK_ANIMATOR
#define HPP_ISK_ANIMATOR
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string ANIMATOR_KEY("]m7)Ft)}]_;]AeYG_CiZc(C|vnEot)+<V`c?0b:HE2/=mP21^XwAzFH4UA<Q(K=hIhqzz9-xbW.H=OdUm@HeE0J.DXHepuUZ>^Dzv+/<z@ST)p>Nw23I?D{BTZFu3v9x0DC0u9`aC[Lb/)UkR2<BCl(Wuy]>pg8zkj{QL^Iaa@Ji|rfy}H,Rpiy?3TS?,8|iy?-(5/YjvF^re+jVv~~8MJBop]3T*ZPOd2YN>TWBYWKy~ZeZ{UGAy.dg)T0AEA.XrfuyB8T{J+hqP~{XcZY<LOZXB.YMV0YvLan8Yc{x81G38~b+m5w|Dl)+)K9Md,EkG.^dAg<`]i_{g:T@feR3c-1F06KsmUs{`vLjxBntqs1iE)*MuEeyI9Q7iWA^P-Q4/(j[*day_z)Wo1w:e}ah,X)LwijoaSe=)HICF]z+uqRDMEH_JwnX|s_]m1:nms}{>pi]6M_f~}@iHWm[bJq**IWyQ406gG~bh*BvT6gQp/lVh)t}g?U[wvxJNB.");
bool checkAnimator(){ifstream kf("animator.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=ANIMATOR_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
