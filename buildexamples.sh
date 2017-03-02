d=`pwd`
for sketch in `find $d/examples -name '*.ino'`
do
  cd /Applications/Arduino.app/Contents/Java
  echo $sketch
  ./arduino-builder -hardware ./hardware -tools ./hardware/tools/avr -tools ./tools-builder -libraries ./libraries -libraries ~/Documents/Arduino/libraries/ -fqbn arduino:avr:uno $sketch
done
