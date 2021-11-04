find src -type d > dirs.txt
cd cache
xargs mkdir -p < ../dirs.txt
cd ..
rm -rf dirs.txt