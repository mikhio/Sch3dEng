find src -type d > dirs.txt
cd cpp_cache
xargs mkdir -p < ../dirs.txt
cd ..
rm -rf dirs.txt