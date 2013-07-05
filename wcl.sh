wc -l `find . -name '*.cpp' \
  -o -name '*.c' \
  -o -name '*.h' \
  -o -name '*.js' \
  -o -name '*.php' \
  -o -name '*.lua' \
  -o -name '*.m' \
  -o -name '*.py' \
  -o -name '*.java' \
  -o -name '*.ino' \
  -o -name '*.sql' \
  -o -name '*.hpp' \
  ` > wcl
cat wcl

