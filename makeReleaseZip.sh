DATE=`date +%Y_%m_%d`
fn=Infraelly_$DATE

7z u -aoa -t7z -y $fn InfraellyLancher.sh docs infraelly.run *.ipf -mx9 -ms=on -mf=on -mmt=on -mtc=on
cp -f $fn.7z ../
rm -f $fn.7z
echo
echo ../$fn.7z created
echo
exit 0
