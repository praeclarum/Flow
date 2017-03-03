make

d=`pwd`
for sketch in `find $d/examples -name '*.ino'`
do
  cd /Applications/Arduino.app/Contents/Java
  echo $sketch
  ./arduino-builder -verbose -hardware ./hardware -tools ./hardware/tools/avr -tools ./tools-builder -libraries ./libraries -libraries $d/.. -fqbn arduino:avr:uno $sketch
done
