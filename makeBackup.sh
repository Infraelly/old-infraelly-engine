DATE=`date +%Y_%m_%d`
fn=$DATE

echo "Removing temp files"
rm -f ./stderr.txt
rm -f ./stdout.txt
rm -f ./*.o
rm -f ./*.tmp
rm -f ./gmon.out
rm -f ./*.h*.save
rm -f ./*.c*.save
rm -f ./LibBuild/guichanbuild/src/*.o
rm -f ./LibBuild/guichanbuild/src/allegro/*.o
rm -f ./LibBuild/guichanbuild/src/contrib/allegro/*.o
rm -f ./LibBuild/guichanbuild/src/contrib/opengl/*.o
rm -f ./LibBuild/guichanbuild/src/contrib/sdl/*.o
rm -f ./LibBuild/guichanbuild/src/contrib/widgets/*.o
rm -f ./LibBuild/guichanbuild/src/contrib/*.o
rm -f ./LibBuild/guichanbuild/src/hge/*.o
rm -f ./LibBuild/guichanbuild/src/opengl/*.o
rm -f ./LibBuild/guichanbuild/src/openlayer/*.o
rm -f ./LibBuild/guichanbuild/src/sdl/*.o
rm -f ./LibBuild/guichanbuild/src/widgets/*.o

rmdir -p ./.objs
rmdir -p ./LibBuild/cryptoppbuild/Win32
rmdir -p ./LibBuild/cryptoppbuild/Nix
rmdir -p ./LibBuild/guichanbuild/.objs
rmdir -p ./LibBuild/net2build/obj
rmdir -p ./LibBuild/sdl_gfxbuild/.objs
rmdir -p ./LibBuild/sdl_gfxbuild/Test/obj
rmdir -p ./LibBuild/tinyxmlbuild/obj
rmdir -p ./LibBuild/zlibbuild/contrib/vstudio/vc9/x86/ZlibDllReleaseWithoutAsm Win32/Tmp


7z u -aoa -t7z -y $fn * -mx9 -ms=on -mf=on -mmt=on -mtc=on -o../Backups -x!.git -x!*.ipf

if [ ! -d ../Inf_Backups ]
then
    mkdir ../Inf_Backups
fi

cp -f $fn.7z ../Inf_Backups/$fn.7z
rm -f $fn.7z
echo

if [ -f ../Inf_Backups/$fn.7z ]
then 
    echo "../Inf_Backups/$fn.7z created"
else 
    echo "Failed"
fi

exit 0
