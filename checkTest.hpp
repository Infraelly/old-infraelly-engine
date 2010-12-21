#ifndef HPP_ISK_TEST
#define HPP_ISK_TEST
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string TEST_KEY("ED.K2+SmKkw)Av>QWPyC/[o62IOkCPuF6:-clILz,SZ;ruDw9.V~|MZ5cS/G([c{,3qLqL393>9J*)U:rx:pV<<F*UvOu9f>A5m0l^UBi+zFmjdz99MryOFM_[ZhMdR5@?5c?kUCxhX-vX;H:3JQHfZ},.h0Bo]MxrV(QCo6MJ:SJ1[LaOc_FY8m7u5,[pIZE_Im2UWy5ezq7n{g=L3rG:R`O/?f+IHzfARF;mJ/FQe3rzB3GS)0eP8GqqyFSl5,[5245-R[z:SRg.)^Thhmds`0F|PcEx(Y)HYS9,ts+@DKHL7;OzMt^QvFkX^:e2+ROeABipF?*4ZM.AkB33BOh]s=QI}64Kg9]AZZ@Y,x~7)Y:U@Q)>A?k3q,ZkD=u}ZVI1`o*L>ua4]5y/(FB*MN@-}6pp>E9Fr_`8USF]LsWyhy:Mz^C0(ROsT>,we4F|d/gP4xhf|qhKGWr?=W(STXy]uTwejC]F:yTFdJ9ezYLsfqf6b=>>/d3?-M6Mg");
bool checkTest(){ifstream kf("test.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=TEST_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif
