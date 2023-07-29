```sh
brew install make armmbed/formulae/arm-none-eabi-gcc dfu-util
cd DaisySP && make
cd libDaisy && make
```
first make DaisySP and/or libDaisy
next project / make
then dfu-util
```
dfu-util -R -a 0 -s 0x90040000:leave -D build/Merlin.bin -d ,0483:df11
```



manual make dfu-util v 0.9
```sh
git clone git://git.code.sf.net/p/dfu-util/dfu-util
## checkout 0.9
brew install automake
./autoconf
./configure --libdir=/opt/local/lib --includedir=/opt/local/include  # on MacOSX only
```