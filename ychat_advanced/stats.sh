echo Lines of code in .h and .cpp files: $(cat *{cpp,h} mods/*/*cpp | wc -l)
echo
echo Lines of code in .html files: $(cat html/*html|wc -l)
echo
echo Lines of text including html templates, sources and readmes: $(cat *{cpp,h} mods/*/*cpp html/*html *txt README COPYING INSTALL| wc -l)
echo
echo Amount of files total: $(find . | wc -l) 
echo
echo Amount of .cpp files: $(find . | grep "\.cpp" | wc -l) 
echo
echo Amount of .h files: $(find . | grep "\.h" | wc -l) 
echo
echo Amount of .so dynamic modules: $(find . | grep "\.so" | wc -l) 
echo
echo Total size: $(du -hs ../ychat_advanced)
echo
echo Size of the ychat executable: $(du -h ychat)
echo
echo Size of the html templates: $(du -sh html)

